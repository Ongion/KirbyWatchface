var Clay = require('pebble-clay');
var clayConfig = require('./config');
var customClay = require('./custom-clay');
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

function locationSuccess(pos) {
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

      // // Temperature in Kelvin requires adjustment
      var temperature = Math.round(json.main.temp);
      var condition = json.weather[0].id;
      var sunrise = json.sys.sunrise;
      var sunset = json.sys.sunset;
      
      console.log("Temperature is " + temperature);
      console.log(condition);

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
  var url = "http://api.openweathermap.org/geo/1.0/direct?q="+city+"&limit=1&appid="+api_key;

  xhrRequest(url, 'GET', 
    function(text) {
      // responseText contains a JSON object with location info
      var json = JSON.parse(text);

      getWeatherOpenWeatherAPIWithLatLong(json[0].lat, json[0].lon);
    }
  );
}

function locationError(err) {
  console.log("Error requesting location!");
  getWeatherOpenWeatherAPIWithCity()
}

function getWeatherOpenWeatherAPI() {
  if (city === "") {
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
  }else{
    getWeatherOpenWeatherAPIWithCity();
  }
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

    if (dict['RequestWeather'] && dict['OpenWeatherAPIKey'])
    {
      api_key = dict['OpenWeatherAPIKey'];
      city = dict['City']

      console.log("Requesting Weather from OpenWeatherAPI!");
      getWeatherOpenWeatherAPI();
    }
  });