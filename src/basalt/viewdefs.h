#pragma once
#include <pebble.h>
#include "commonTypes.h"

#define ABILITY_NAME_LAYER_ORIGIN GPoint(14, 26)

#define HUD_KIRBY_LAYER_RECT GRect(1, 1, 66, 25)
#define HUD_BOSS_LAYER_RECT GRect(77, 1, 66, 25)
#define TIME_LAYER_RECT GRect(7, 130, 58, 24)
#define DATE_RECT GRect(79, 139, 60, 20)
#define DAY_OF_WEEK_RECT GRect(79, 127, 58, 20)
#define TEMPERATURE_RECT_BIG GRect(79, 130, 58, 24)
#define TEMPERATURE_RECT_SMALL GRect(79, 133, 58, 24)

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