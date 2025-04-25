#pragma once

#include <pebble.h>

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

typedef struct AbilityAnimation
{
	uint32_t resourceID;
	GPoint origin;
} AbilityAnimation;

typedef struct BossSet
{
	uint32_t bossResourceID;
	uint32_t bossNameResourceID;
	GPoint bossOrigin;
	GPoint nameOrigin;
} BossSet;
