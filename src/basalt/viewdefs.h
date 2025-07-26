#pragma once
#include <pebble.h>
#include "commonTypes.h"

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

#define ABILITY_NAME_LAYER_ORIGIN GPoint(14, 26)

#define HUD_KIRBY_LAYER_RECT GRect(1, 1, 66, 25)
#define HUD_BOSS_LAYER_RECT GRect(77, 1, 66, 25)
#define TIME_LAYER_RECT GRect(9, 130, 56, 24)
#define DATE_RECT GRect(79, 139, 60, 20)
#define DAY_OF_WEEK_RECT GRect(79, 126, 60, 20)
#define TEMPERATURE_RECT_BIG GRect(79, 130, 60, 24)
#define TEMPERATURE_RECT_SMALL GRect(79, 133, 60, 24)

#define ZEROTWO_ORIGIN GPoint(33, 23)
#define KING_ORIGIN GPoint(82, 53)
#define KING_DEFEAT_ORIGIN GPoint(77, 60)
#define KRACKO_ORIGIN GPoint(64, 26)
#define MR_BRIGHT_ORIGIN GPoint(88, 56)
#define MR_BRIGHT_DEFEAT_ORIGIN GPoint(87, 56)
#define MR_SHINE_ORIGIN GPoint(91, 71)
#define MR_SHINE_DEFEAT_ORIGIN GPoint(91, 67)
#define MR_FROSTY_ORIGIN GPoint(92, 69)

#define ZEROTWO_NAME_ORIGIN GPoint(86, 26)
#define KING_NAME_ORIGIN GPoint(89, 26)
#define KRACKO_NAME_ORIGIN GPoint(89, 26)
#define MR_BRIGHT_NAME_ORIGIN GPoint(88, 26)
#define MR_SHINE_NAME_ORIGIN GPoint(90, 26)
#define MR_FROSTY_NAME_ORIGIN GPoint(86, 26)

#define INITIAL_ANIMATION_GETTER get_random_ability_animation()
#define FONT_TIME fonts_get_system_font(FONT_KEY_LECO_20_BOLD_NUMBERS)
#define FONT_DATE s_fontDate
#define FONT_DAY_OF_WEEK s_fontDate