var Clay = require('pebble-clay');
var clayConfig = require('./config');
var customClay = require('./custom-clay');
var clay = new Clay(clayConfig, customClay);

var OpenWeatherMap = require('./openWeatherMap');
var USNWS = require('./USNWS');

Pebble.addEventListener('ready', function () {
	console.log('PebbleKit JS Ready!');

	Pebble.sendAppMessage({
		'PebbleKitReady': 0
	}, function (e) {
		console.log('Sent ready message!');
	}, function (e) {
		console.log('Failed to send ready message');
		console.log(e);
	});
});

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
	function (e) {
		var dict = e.payload;

		console.log("GOT APP MESSAGE");
		console.log("Req: " + dict['RequestWeather']);
		console.log("Source: " + dict['WeatherSource']);
		if (dict['RequestWeather']) {
			if (dict['WeatherSource'] == 0) {
				// OpenWeatherAPI
				if (dict['OpenWeatherAPIKey']) {
					OpenWeatherMap.setAPIKey(dict['OpenWeatherAPIKey']);
					OpenWeatherMap.setCity(dict['City']);

					console.log("Requesting Weather from OpenWeatherAPI!");
					OpenWeatherMap.getWeather();
				}
			}
			else if (dict['WeatherSource'] == 1) {
				// US NWS
				console.log("Requesting Weather from US NWS!");
				USNWS.getWeather();
			}
		}
	});