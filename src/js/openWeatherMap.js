(function () {
	'use strict';

	var XHRRequest = require('./XHRRequest');
	var Constants = require('./constants');

	var apiKey;
	var city = "";

	function getWeatherCondition(condition) {
		if (200 <= condition && condition < 300) {
			return Constants.WEATHER_LIGHTNING;
		}
		else if (300 <= condition && condition < 600) {
			return Constants.WEATHER_RAIN;
		}
		else if (600 <= condition && condition < 700) {
			return Constants.WEATHER_SNOW;
		}
		else if (condition > 800) {
			return Constants.WEATHER_CLOUDY;
		}
		else {
			return Constants.WEATHER_CLEAR;
		}
	}

	function getWeatherFromCity() {
		console.log("Using geolocation API to find location of city");
		var url = "https://api.openweathermap.org/geo/1.0/direct?q=" + city + "&limit=1&appid=" + apiKey;

		XHRRequest.request(url, 'GET',
			function (text) {
				// responseText contains a JSON object with location info
				var json = JSON.parse(text);

				getWeatherFromLatLong(json[0].lat, json[0].lon);
			}
		);
	}

	function getWeatherFromLatLong(lat, long) {
		console.log("Getting weather at latitude " + lat + ", longitude " + long);
		var url = "http://api.openweathermap.org/data/2.5/weather?lat=" + lat + "&lon=" + long + "&appid=" + apiKey;

		// Send request to OpenWeatherMap
		XHRRequest.request(url, 'GET',
			function (text) {
				// responseText contains a JSON object with weather info
				var json = JSON.parse(text);

				var city_called = json.name;
				console.log("City in response is " + city_called);

				// Return temperature in Kelvin * 100 (for increased rounding precision)
				var temperature = Math.round(json.main.temp * 100);
				var condition = getWeatherCondition(json.weather[0].id);
				var sunrise = json.sys.sunrise;
				var sunset = json.sys.sunset;

				console.log("Temperature: " + (temperature / 100) + "K");
				console.log("Condition  : " + condition);
				console.log("Sunrise    : " + sunrise);
				console.log("Sunset     : " + sunset);

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

	var OpenWeatherMap = {}

	OpenWeatherMap.setAPIKey = function (api_key) {
		apiKey = api_key;
	};

	OpenWeatherMap.setCity = function (the_city) {
		city = the_city;
	};

	OpenWeatherMap.getWeather = function () {
		if (city === "") {
			navigator.geolocation.getCurrentPosition(
				function (pos) {
					getWeatherFromLatLong(pos.coords.latitude, pos.coords.longitude);
				},
				function (err) {
					console.warn("Error requesting location!");
					console.warn("ERROR(" + err.code + "}): " + err.message);
				},
				{ timeout: 30000, maximumAge: 300000 }
			);
		}
		else {
			getWeatherFromCity();
		}
	};

	// export as Node module / AMD module / browser variable
	if (typeof exports === 'object' && typeof module !== 'undefined') module.exports = OpenWeatherMap;
	else if (typeof define === 'function' && define.amd) define(OpenWeatherMap);
	else window.OpenWeatherMap = OpenWeatherMap;

}());