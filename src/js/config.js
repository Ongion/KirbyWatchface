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
                "type": "input",
                "label": "API Key",
                "description": "An API key is required to featch OpenWeatherMap data. These can be freely obtained from OpenWeatherMap.org.",
                "messageKey": "OpenWeatherAPIKey"
            },
            {
                "type": "radiogroup",
                "label": "Temperature Scale",
                "messageKey": "ScalePreference",
                "defaultValue": "C",
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
            },
            {
                "type": "input",
                "label": "City",
                "description": "'&lt;City&gt;, &lt;State&gt;, &lt;Country&gt;'. Leave blank for GPS location.",
                "messageKey": "City"
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
            }
        ]
    },
    {
        "type": "submit",
        "defaultValue": "Save"
    }
]