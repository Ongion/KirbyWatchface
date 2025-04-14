var Clay = require('pebble-clay');
var clayConfig = require('./config');
var clay = new Clay(clayConfig);

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
  // Construct URL
  var url;
  
  if (pos !== undefined) {
    url = "http://api.openweathermap.org/data/2.5/weather?lat=" + pos.coords.latitude + "&lon=" + pos.coords.longitude + '&appid=' + api_key;
  }
  else{
    url = 'http://api.openweathermap.org/data/2.5/weather?&q=' + city + '&appid=' + api_key;
  }
  
  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET', 
    function(text) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(text);
      
      var city_called = json.name;
			console.log("City in response is " + city_called);
      

      // // Temperature in Kelvin requires adjustment
      var temperature = Math.round(json.main.temp);
      var icon = iconFromWeatherId(json.weather[0].id);
      
      console.log("Temperature is " + temperature);
      console.log(icon);

      // Assemble dictionary using our keys
      var dictionary = {
        "Temperature": temperature,
        "Icon": icon
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

function iconFromWeatherId(weatherId) {
  if (weatherId < 300 || weatherId < 600 || weatherId < 700 || weatherId > 800) {
    return 1;
  } else {
    return 0;
  }
}

function locationError(err) {
  console.log("Error requesting location!");
}

function getWeather() {
  if (city === "") {
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
  }else{
    locationSuccess();
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

      console.log("Requesting Weather!");
      getWeather();
    }
  });