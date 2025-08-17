#pragma once

#include <pebble.h>
#include "commonTypes.h"

#define STORAGE_KEY_ClaySettingsVersion 1
#define STORAGE_KEY_ClaySettings 2
#define STORAGE_KEY_LastSeenTemperature 3
#define STORAGE_KEY_LastSeenWeatherCondition 4
#define STORAGE_KEY_LastTimeRecievedWeather 5
#define STORAGE_KEY_LastSeenSunriseTime 6
#define STORAGE_KEY_LastSeenSunsetTime 7

#define ONE_HOUR (1*SECONDS_PER_HOUR)
#define TWO_HOURS (2*SECONDS_PER_HOUR)

extern GFont s_fontBig;
extern GFont s_fontSmall;

void format_weather_layer_text(char* weather_layer_buffer, size_t sz_weather_layer_buffer, int temperature, TemperatureScale scalePreference, bool showTenthsDigit);
void update_weather_layer_text();
void step_layer_update_callback(Layer* layer, GContext* ctx);
void load_custom_fonts();
void unload_custom_fonts();
void update_steps();

#define WEATHER_REFRESH_RATE_MIN 20
#define TIME_STALE_WEATHER 60*(WEATHER_REFRESH_RATE_MIN+1)
#define TIME_REFRESH_WEATHER 60*WEATHER_REFRESH_RATE_MIN

Window* s_WindowMain;