(function () { 'use strict';
	var XHRRequest = {};

	XHRRequest.request = function (url, type, callback) {
		var xhr = new XMLHttpRequest();
		xhr.onload = function() {
			callback(this.responseText);
		};
		xhr.open(type, url);
		xhr.send();
	};

	// export as Node module / AMD module / browser variable
	if (typeof exports === 'object' && typeof module !== 'undefined') module.exports = XHRRequest;
	else if (typeof define === 'function' && define.amd) define(XHRRequest);
	else window.XHRRequest = XHRRequest;
	
}())