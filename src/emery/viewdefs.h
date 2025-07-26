#pragma once
#include <pebble.h>
#include "commonTypes.h"

extern GFont s_fontTime;
extern GFont s_fontDate;

static const int ABILITIES_NAME_RESOURCE_IDS[] =
{
	RESOURCE_ID_BEAM,
	RESOURCE_ID_CUTTER,
	RESOURCE_ID_FIRE,
	RESOURCE_ID_HAMMER,
	RESOURCE_ID_MIKE,
	RESOURCE_ID_SLEEP,
	RESOURCE_ID_SWORD,
};

#define NUM_ABILITIES 7

#define ABILITY_NAME_LAYER_ORIGIN GPoint(14, 25)

#define HUD_KIRBY_LAYER_RECT GRect(0, 0, 96, 41)
#define HUD_BOSS_LAYER_RECT GRect(104, 0, 96, 41)
#define TIME_LAYER_RECT GRect(9, 175, 81, 36)
#define DATE_RECT GRect(110, 189, 80, 21)
#define DAY_OF_WEEK_RECT GRect(110, 172, 80, 21)
#define TEMPERATURE_RECT GRect(108, 175, 82, 38)

#define ZEROTWO_ORIGIN GPoint(50, 35)
#define KING_ORIGIN GPoint(117, 90)
#define KING_DEFEAT_ORIGIN GPoint(112, 98)
#define KRACKO_ORIGIN GPoint(94, 46)
#define MR_BRIGHT_ORIGIN GPoint(129, 93)
#define MR_BRIGHT_DEFEAT_ORIGIN GPoint(128, 93)
#define MR_SHINE_ORIGIN GPoint(132, 105)
#define MR_SHINE_DEFEAT_ORIGIN GPoint(132, 101)
#define MR_FROSTY_ORIGIN GPoint(132, 105)

#define ZEROTWO_NAME_ORIGIN GPoint(113, 23)
#define KING_NAME_ORIGIN GPoint(113, 23)
#define KRACKO_NAME_ORIGIN GPoint(137, 25)
#define MR_BRIGHT_NAME_ORIGIN GPoint(116, 25)
#define MR_SHINE_NAME_ORIGIN GPoint(127, 25)
#define MR_FROSTY_NAME_ORIGIN GPoint(115, 25)

#define INITIAL_ANIMATION_GETTER &(INTRO_ANIMATIONS[abilityIdx])
#define FONT_TIME s_fontTime
#define FONT_DATE s_fontDate
#define FONT_DAY_OF_WEEK s_fontDate