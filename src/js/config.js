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
                "id": "KeyOpenWeatherMapAPIKey"
            },
            {
                "type": "radiogroup",
                "label": "Temperature Scale",
                "messageKey": "KEY_SCALE",
                "defaultValue": "celcius",
                "options": [
                    {
                        "label": "Celcius",
                        "value": "celcius"
                    },
                    {
                        "label": "Fahrenheit",
                        "value": "fahrenheit"
                    },
                ]
            },
            {
                "type": "input",
                "label": "City",
                "description": "Leave blank for GPS location.",
                "id": "KeyCity"
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
                "label": "Daily Steps Goal",
                "defaultValue": 5000,
                "min": 0,
                "max": 20000,
                "step": 1000
            }
        ]
    },
    {
        "type": "submit",
        "defaultValue": "Save"
    }
]