// Must be manually kept in sync with corresponding constants in c code

(function () { 'use strict';
	var Constants = {};

	Constants.WEATHER_CLEAR = 0;
	Constants.WEATHER_CLOUDY = 1;
	Constants.WEATHER_RAIN = 2;
	Constants.WEATHER_LIGHTNING = 3;
	Constants.WEATHER_SNOW = 4;

	// export as Node module / AMD module / browser variable
	if (typeof exports === 'object' && typeof module !== 'undefined') module.exports = Constants;
	else if (typeof define === 'function' && define.amd) define(Constants);
	else window.Constants = Constants;
	
}())