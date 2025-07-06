module.exports = function(minified) {
	var clayConfig = this;
	var _ = minified._;
	var $ = minified.$;
	var HTML = minified.HTML;

	function toggleAPIKeyVisibility() {
		if (this.get() == "openWeather") {
			clayConfig.getItemByMessageKey("OpenWeatherAPIKey").show();
			clayConfig.getItemByMessageKey("City").show();
		} else {
			clayConfig.getItemByMessageKey("OpenWeatherAPIKey").hide();
			clayConfig.getItemByMessageKey("City").hide();

		}
	}

	clayConfig.on(clayConfig.EVENTS.AFTER_BUILD, function() {
		var weatherSourceSelector = clayConfig.getItemByMessageKey("WeatherSource");
		toggleAPIKeyVisibility.call(weatherSourceSelector);
		weatherSourceSelector.on('change', toggleAPIKeyVisibility);
	})
}