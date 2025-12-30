(function () {
	'use strict';

	var XHRRequest = require('./XHRRequest');
	var Constants = require('./constants');

	function getWeatherCondition(weather_code) {
		if (2 <= weather_code && weather_code < 61) {
			return Constants.WEATHER_CLOUDY;
		}
		else if ((61 <= weather_code && weather_code < 71) || (80 <= weather_code && weather_code < 85)) {
			return Constants.WEATHER_RAIN;
		}
		else if ((71 <= weather_code && weather_code < 80) || (85 <= weather_code && weather_code < 95)) {
			return Constants.WEATHER_SNOW;
		}
		else if (95 <= weather_code && weather_code < 100) {
			return Constants.WEATHER_LIGHTNING;
		}
		else {
			return Constants.WEATHER_CLEAR;
		}
	}

	function locationSuccess(pos) {
		var url = "https://api.open-meteo.com/v1/forecast?latitude=" + pos.coords.latitude + "&longitude=" + pos.coords.longitude + "&daily=sunrise,sunset&current=weather_code,temperature_2m&timezone=GMT&forecast_days=1";

		// Send request to Open-Meteo
		XHRRequest.request(url, 'GET',
			function (response) {
				var json = JSON.parse(response);

				var temperatureC = json.current.temperature_2m;
				var condition = getWeatherCondition(json.current.weather_code);
				var sunrise = Date.parse(json.daily.sunrise[0] + "Z") / 1000;
				var sunset = Date.parse(json.daily.sunset[0] + "Z") / 1000;

				// Return temperature in Kelvin * 100 (for increased rounding precision)
				var temperature = Math.round((temperatureC + 273.15) * 100);

				console.log("Temperature : " + (temperature / 100) + "K");
				console.log("Condition   : " + condition);
				console.log("Sunrise     : " + sunrise);
				console.log("Sunset      : " + sunset);

				// Assemble dictionary using our keys
				var dictionary = {
					"Temperature": temperature,
					"WeatherCondition": condition,
					"Sunrise": sunrise,
					"Sunset": sunset
				};

				// Send to Pebble
				Pebble.sendAppMessage(dictionary,
					function (e) {
						console.log("Weather info sent to Pebble successfully!");
					},
					function (e) {
						console.log("Error sending weather info to Pebble!");
					}
				);
			}
		);
	}

	var OpenMeteo = {}

	OpenMeteo.getWeather = function () {
		navigator.geolocation.getCurrentPosition(
			locationSuccess,
			function (err) {
				console.warn("Error requesting location!");
				console.warn("ERROR(" + err.code + "}): " + err.message);
			},
			{ timeout: 30000, maximumAge: 300000 }
		);
	};

	// export as Node module / AMD module / browser variable
	if (typeof exports === 'object' && typeof module !== 'undefined') module.exports = OpenMeteo;
	else if (typeof define === 'function' && define.amd) define(OpenMeteo);
	else window.OpenMeteo = OpenMeteo;

}());