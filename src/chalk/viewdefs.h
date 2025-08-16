#pragma once
#include <pebble.h>
#include "commonTypes.h"

#define ABILITY_NAME_LAYER_ORIGIN GPoint(32, 26)

#define HUD_KIRBY_LAYER_RECT GRect(0, 0, 90, 180)
#define HUD_BOSS_LAYER_RECT GRect(95, 1, 66, 25)
#define TIME_LAYER_RECT GRect(25, 130, 58, 24)
#define DATE_RECT GRect(97, 139, 60, 20)
#define DAY_OF_WEEK_RECT GRect(97, 127, 58, 20)
#define TEMPERATURE_RECT_BIG GRect(97, 130, 58, 24)
#define TEMPERATURE_RECT_SMALL GRect(97, 133, 58, 24)

#define ZEROTWO_ORIGIN GPoint(51, 23)
#define KING_ORIGIN GPoint(100, 53)
#define KING_DEFEAT_ORIGIN GPoint(95, 60)
#define KRACKO_ORIGIN GPoint(82, 26)
#define MR_BRIGHT_ORIGIN GPoint(106, 56)
#define MR_BRIGHT_DEFEAT_ORIGIN GPoint(105, 56)
#define MR_SHINE_ORIGIN GPoint(109, 71)
#define MR_SHINE_DEFEAT_ORIGIN GPoint(109, 67)
#define MR_FROSTY_ORIGIN GPoint(110, 69)

#define ZEROTWO_NAME_ORIGIN GPoint(104, 26)
#define KING_NAME_ORIGIN GPoint(107, 26)
#define KRACKO_NAME_ORIGIN GPoint(107, 26)
#define MR_BRIGHT_NAME_ORIGIN GPoint(106, 26)
#define MR_SHINE_NAME_ORIGIN GPoint(108, 26)
#define MR_FROSTY_NAME_ORIGIN GPoint(104, 26)

#define INITIAL_ANIMATION_GETTER get_random_ability_animation()