#pragma once

#include <pebble.h>
#include "commonTypes.h"

#define STORAGE_KEY_ClaySettingsVersion 1
#define STORAGE_KEY_ClaySettings 2
#define STORAGE_KEY_LastSeenTemperature 3
#define STORAGE_KEY_LastSeenWeatherCondition 4
#define STORAGE_KEY_LastTimeRecievedWeather 5

ClaySettings g_settings;
static bool initiate_watchface = true;
static bool daytime;

static bool pebbleKitReady = false;

static GColor s_bgColorTime;

static TextLayer* s_pTextLayerWeather;
static TextLayer* s_pTextLayerTime;
static TextLayer* s_pTextLayerDate;

static bool bt_connected;

static GBitmap* s_pBitmapBackground;
static BitmapLayer* s_pLayerBackground;

static GBitmap* s_pBitmapHUDKirby;
static BitmapLayer* s_pLayerHUDKirby;

static GBitmap* s_pBitmapHUDBoss;
static BitmapLayer* s_pLayerHUDBoss;

static int s_temperature = 255;
static int s_weatherCondition = -1;
static time_t s_lastWeatherTime = 0;

static unsigned int abilityIdx = 0;

static AppTimer* s_pKirbyAnimationTimer;
static AppTimer* s_pShowDateTimer;
static AppTimer* s_pWeatherTimeoutTimer;

static GBitmap* s_pBitmapAbilityName;
static BitmapLayer* s_pLayerAbilityName;

static GBitmap* s_pBitmapBossName;
static BitmapLayer* s_pLayerBossName;

static GBitmap* s_pBitmapBoss;
static BitmapLayer* s_pLayerBoss;

static GBitmap* s_pBitmapKirby;
static GBitmapSequence* s_pBitmapSequenceKirby;
static BitmapLayer* s_pLayerKirby;

void format_weather_layer_text(char* weather_layer_buffer, size_t sz_weather_layer_buffer, int temperature, TemperatureScale scalePreference);
void update_weather_layer_text();
void step_layer_update_callback(Layer* layer, GContext* ctx);
void load_custom_fonts();
void unload_custom_fonts();
void update_steps();

#define TIME_STALE_WEATHER 60*30

Window* s_WindowMain;