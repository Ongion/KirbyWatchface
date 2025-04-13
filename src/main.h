#pragma once

#include<pebble.h>

#define STORAGE_KEY_ClaySettingsVersion 1
#define STORAGE_KEY_ClaySettings 2


typedef enum temperature_scales
{
  FAHRENHEIT,
  CELSIUS
} TemperatureScale;

typedef struct ClaySettingsV1
{
    char openWeatherMapAPIKey[33];
    char city[86];
    TemperatureScale scalePreference;
    uint16_t stepsGoal;
} ClaySettingsV1;

typedef ClaySettingsV1 ClaySettings;