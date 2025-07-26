module.exports = [
    {
        "type": "heading",
        "defaultValue": "Kirby Configuration"
    },
    {
        "type": "section",
        "items": [
            {
                "type": "heading",
                "defaultValue": "Weather"
            },
            {
                "type": "select",
                "messageKey": "WeatherSource",
                "defaultValue": "openWeather",
                "label": "Weather Source",
                "description": "OpenWeatherMap requires a (free) API key.<br>US NWS only works in the USA.",
                "options" : [
                    {
                        "label": "OpenWeatherMap",
                        "value": "openWeather"
                    },
                    {
                        "label": "US NWS",
                        "value": "weathergov"
                    }
                ]
            },
            {
                "type": "input",
                "label": "API Key",
                "description": "An API key is required to featch OpenWeatherMap data. These can be freely obtained from OpenWeatherMap.org.",
                "messageKey": "OpenWeatherAPIKey"
            },
            {
                "type": "input",
                "label": "City",
                "description": "'&lt;City&gt;, &lt;State&gt;, &lt;Country&gt;'. Leave blank for GPS location.",
                "messageKey": "City"
            },
            {
                "type": "radiogroup",
                "label": "Temperature Scale",
                "messageKey": "ScalePreference",
                "defaultValue": "F",
                "options": [
                    {
                        "label": "Celcius",
                        "value": "C"
                    },
                    {
                        "label": "Fahrenheit",
                        "value": "F"
                    },
                ]
            }
        ]
    },
    {
        "type": "section",
        "items": [
            {
                "type": "heading",
                "defaultValue": "Health"
            },
            {
                "type": "slider",
                "messageKey": "StepsGoal",
                "label": "Daily Steps Goal",
                "defaultValue": 5000,
                "min": 0,
                "max": 20000,
                "step": 1000
            }
        ]
    },
    {
        "type": "section",
        "items": [
            {
                "type": "heading",
                "defaultValue": "Options"
            },
            {
                "type": "toggle",
                "messageKey": "AnimateOnGlance",
                "label": "Animate On Glance",
                "description": "Animate when the watch is turned towards the user's face. May use more battery.",
                "defaultValue": false
            },
            {
                "type": "toggle",
                "messageKey": "TenthsDigitTemperature",
                "label": "Show Tenths Digit",
                "description": "Show tenths digit when showing temperature",
                "defaultValue": false
            }
        ]
    },
    {
        "type": "submit",
        "defaultValue": "Save"
    }
]