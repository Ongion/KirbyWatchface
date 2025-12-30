(function () {
	'use strict';

	var XHRRequest = require('./XHRRequest');
	var Constants = require('./constants');
	var SunCalc = require('./suncalc');

	function lerpByTimestamps(start, end, startTime, endTime, currentTime) {
		var amount = (currentTime - startTime) / (endTime - startTime);

		return start + (end - start) * amount;
	}

	function findCurrentValue(values) {
		var now = new Date();
		for (var i = 0; i < values.length; i++) {
			var validTimeStart = values[i].validTime.split("/")[0];
			if (now > Date.parse(validTimeStart)) {
				continue;
			}

			var startValue = values[i - 1].value;
			var endValue = values[i].value;
			var startTime = Date.parse(values[i - 1].validTime.split("/")[0]);
			var endTime = Date.parse(values[i].validTime.split("/")[0]);

			return lerpByTimestamps(startValue, endValue, startTime, endTime, now);
		}
	}

	function getWeatherCondition(skyCoverPct, precipPct, precipMM, snowfallMM, thunderPct) {
		if (thunderPct >= 40) {
			// Thunderstorm
			return Constants.WEATHER_LIGHTNING;
		}

		if ((precipPct / 100) * precipMM >= 1) {
			if (snowfallMM > 0) {
				// Snowing
				return Constants.WEATHER_SNOW;
			}

			// Raining
			return Constants.WEATHER_RAIN;
		}

		if (skyCoverPct >= 50) {
			// Cloudy
			return Constants.WEATHER_CLOUDY;
		}

		// Clear
		return Constants.WEATHER_CLEAR;
	}

	function locationSuccess(pos) {
		var pointsURL = "https://api.weather.gov/points/" + pos.coords.latitude + "," + pos.coords.longitude;
		console.log("Getting gridpoint URL from points API");

		// Send points request to US NWS
		XHRRequest.request(pointsURL, 'GET',
			function (pointsResponse) {
				var pointsJSON = JSON.parse(pointsResponse);

				var gridpointsURL = pointsJSON.properties.forecastGridData;
				console.log(gridpointsURL);
				XHRRequest.request(gridpointsURL, 'GET',
					function (gridpointsResponse) {
						var gridpointsJSON = JSON.parse(gridpointsResponse);
						var temperatureC = findCurrentValue(gridpointsJSON.properties.temperature.values);
						var skyCoverPct = findCurrentValue(gridpointsJSON.properties.skyCover.values);
						var precipPct = findCurrentValue(gridpointsJSON.properties.probabilityOfPrecipitation.values);
						var precipMM = findCurrentValue(gridpointsJSON.properties.quantitativePrecipitation.values);
						var snowfallMM = findCurrentValue(gridpointsJSON.properties.snowfallAmount.values);
						var thunderPct = findCurrentValue(gridpointsJSON.properties.probabilityOfThunder.values);

						// Return temperature in Kelvin * 100 (for increased rounding precision)
						var temperature = Math.round((temperatureC + 273.15) * 100);
						var condition = getWeatherCondition(skyCoverPct, precipPct, precipMM, snowfallMM, thunderPct)
						var times = SunCalc.getTimes(new Date(), pos.coords.latitude, pos.coords.longitude)
						var sunrise = Math.round(times.sunrise.getTime() / 1000);
						var sunset = Math.round(times.sunset.getTime() / 1000);

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
		);
	}

	var USNWS = {}

	USNWS.getWeather = function () {
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
	if (typeof exports === 'object' && typeof module !== 'undefined') module.exports = USNWS;
	else if (typeof define === 'function' && define.amd) define(USNWS);
	else window.USNWS = USNWS;

}());