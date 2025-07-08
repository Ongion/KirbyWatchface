var Clay = require('pebble-clay');
var clayConfig = require('./config');
var customClay = require('./custom-clay');
var SunCalc = require('./suncalc');
var clay = new Clay(clayConfig, customClay);

var api_key;
var city = "";

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function openWeatherAPILocationSuccess(pos) {
  getWeatherOpenWeatherAPIWithLatLong(pos.coords.latitude, pos.coords.longitude);
}

function getWeatherOpenWeatherAPIWithLatLong(lat, long)
{
  var url = "http://api.openweathermap.org/data/2.5/weather?lat=" + lat + "&lon=" + long + '&appid=' + api_key;

  console.log(url)
  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET', 
    function(text) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(text);
      
      var city_called = json.name;
      console.log("City in response is " + city_called);

      // Return temperature in Kelvin * 100 (for increased rounding precision)
      var temperature = Math.round(json.main.temp * 100);
      var condition = json.weather[0].id;
      var sunrise = json.sys.sunrise;
      var sunset = json.sys.sunset;
      
      console.log("Temperature is " + temperature/100);
      console.log(condition);
      console.log("Sunrise: " + sunrise);
      console.log("Sunset: " + sunset);

      // Assemble dictionary using our keys
      var dictionary = {
        "Temperature": temperature,
        "WeatherCondition": condition,
        "Sunrise": sunrise,
        "Sunset": sunset
      };

      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log("Weather info sent to Pebble successfully!");
        },
        function(e) {
          console.log("Error sending weather info to Pebble!");
        }
      );}      
  );  
}

function getWeatherOpenWeatherAPIWithCity()
{
  console.log("Getting weather by city")
  var url = "https://api.openweathermap.org/geo/1.0/direct?q="+city+"&limit=1&appid="+api_key;

  xhrRequest(url, 'GET', 
    function(text) {
      // responseText contains a JSON object with location info
      var json = JSON.parse(text);

      getWeatherOpenWeatherAPIWithLatLong(json[0].lat, json[0].lon);
    }
  );
}

function openWeatherAPILocationError(err) {
  console.log("Error requesting location!");
  getWeatherOpenWeatherAPIWithCity()
}

function getWeatherOpenWeatherAPI() {
  if (city === "") {
  navigator.geolocation.getCurrentPosition(
    openWeatherAPILocationSuccess,
    openWeatherAPILocationError,
    {timeout: 30000, maximumAge: 300000}
  );
  }else{
    getWeatherOpenWeatherAPIWithCity();
  }
}

function findCurrentValue(values)
{
  var now = new Date();
  for (var i = 0; i < values.length; i++)
  {
    validTimeStart = values[i].validTime.split("/")[0];
    if (now > Date.parse(validTimeStart))
    {
      continue;
    }

    return values[i-1].value;
  }
}

function getWeatherCondition(skyCoverPct, precipPct, snowfallMM, thunderPct)
{
  if (precipPct > 50)
  {
    if (snowfallMM > 0)
    {
      // Snowing
      return 600;
    }

    if (thunderPct > 50)
    {
      // Thunderstorm
      return 200;
    }
    
    // Raining
    return 300;
  }

  if (skyCoverPct > 50)
  {
    if (thunderPct > 50)
    {
      // Thunderstorm
      return 200;
    };

    // Cloudy
    return 803;
  }

  // Clear
  return 800;
}

function usNWSLocationSuccess(pos){
  var pointsURL = "https://api.weather.gov/points/"+pos.coords.latitude+","+pos.coords.longitude;
  console.log(pointsURL);

  // Send points request to US NWS
  xhrRequest(pointsURL, 'GET',
    function(pointsResponse) {
      var pointsJSON = JSON.parse(pointsResponse);

      var gridpointsURL = pointsJSON.properties.forecastGridData;
      console.log(gridpointsURL);
      xhrRequest(gridpointsURL, 'GET',
        function(gridpointsResponse) {
          var gridpointsJSON = JSON.parse(gridpointsResponse);
          var temperatureC = findCurrentValue(gridpointsJSON.properties.temperature.values);
          var skyCoverPct = findCurrentValue(gridpointsJSON.properties.skyCover.values);
          var precipPct = findCurrentValue(gridpointsJSON.properties.probabilityOfPrecipitation.values);
          var snowfallMM = findCurrentValue(gridpointsJSON.properties.snowfallAmount.values);
          var thunderPct = findCurrentValue(gridpointsJSON.properties.probabilityOfThunder.values);

          // Return temperature in Kelvin * 100 (for increased rounding precision)
          var temperature = Math.round((temperatureC + 273.15) * 100);
          var condition = getWeatherCondition(skyCoverPct, precipPct, snowfallMM, thunderPct)
          var times = SunCalc.getTimes(new Date(), pos.coords.latitude, pos.coords.longitude)
          var sunrise = Math.round(times.sunrise.getTime()/1000);
          var sunset = Math.round(times.sunset.getTime()/1000);

          console.log("Temperature is " + temperature/100);
          console.log(condition);
          console.log("Sunrise: " + sunrise);
          console.log("Sunset: " + sunset);

          // Assemble dictionary using our keys
          var dictionary = {
            "Temperature": temperature,
            "WeatherCondition": condition,
            "Sunrise": sunrise,
            "Sunset": sunset
          };

          // Send to Pebble
          Pebble.sendAppMessage(dictionary,
            function(e) {
              console.log("Weather info sent to Pebble successfully!");
            },
            function(e) {
              console.log("Error sending weather info to Pebble!");
            }
          );
        });
    }
  );
}

function usNWSLocationError(err) {
  console.log("Error requesting location!");
}

function getWeatherUSNWS() {
  navigator.geolocation.getCurrentPosition(
    usNWSLocationSuccess,
    usNWSLocationError,
    {timeout: 30000, maximumAge: 300000}
  );
}

Pebble.addEventListener('ready', function() {
  console.log('PebbleKit JS Ready!');
  city = localStorage.city;

  Pebble.sendAppMessage({'PebbleKitReady': 0
	}, function(e) {
      console.log('Sent ready message!');
  }, function(e) {
      console.log('Failed to send ready message');
			console.log(e);
  });
});

  // Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    var dict = e.payload;

    console.log("GOT APP MESSAGE");
    console.log("Req: " + dict['RequestWeather']);
    console.log("Source: " + dict['WeatherSource']);
    if (dict['RequestWeather'])
    {
      if (dict['WeatherSource'] == 0)
      {
        // OpenWeatherAPI
        if (dict['OpenWeatherAPIKey'])
        {
          api_key = dict['OpenWeatherAPIKey'];
          city = dict['City']

          console.log("Requesting Weather from OpenWeatherAPI!");
          getWeatherOpenWeatherAPI();  
        }
      }
      else if (dict['WeatherSource'] == 1)
      {
        // US NWS
        console.log("Requesting Weather from US NWS!");
        getWeatherUSNWS();
      }
    }
  });