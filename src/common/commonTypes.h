#pragma once

#include <pebble.h>

typedef enum weather_source
{
	OPENWEATHER,
	USNWS
} WeatherSource;

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
	bool animateOnGlance;
} ClaySettingsV1;

typedef struct ClaySettingsV2
{
	WeatherSource weatherSource;
	char openWeatherMapAPIKey[33];
	char city[86];
	TemperatureScale scalePreference;
	uint16_t stepsGoal;
	bool animateOnGlance;
} ClaySettingsV2;


typedef ClaySettingsV2 ClaySettings;

typedef struct BossSet
{
	uint32_t bossResourceID;
	uint32_t bossNameResourceID;
	GPoint bossOrigin;
	GPoint nameOrigin;
} BossSet;

typedef enum AnimationType
{
	AT_Manual,
	AT_APNG
} AnimationType;

typedef struct AnimationFrame
{
	uint32_t resourceID;
	GPoint pos;
	uint16_t delayMs;
} AnimationFrame;

typedef struct ManualAnimation
{
	uint16_t loops;
	uint16_t numFrames;
	AnimationFrame* pFrames;
} ManualAnimation;

typedef struct APNGAnimation
{
	uint32_t resourceID;
	GPoint origin;
} APNGAnimation;

typedef struct AbilityAnimation
{
	AnimationType type;
	union
	{
		const ManualAnimation manualAnimation;
		const APNGAnimation APNGAnimation;
	} animation;
} AbilityAnimation;