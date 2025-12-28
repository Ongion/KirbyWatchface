#pragma once
#include <pebble.h>
#include "commonTypes.h"

#define ABILITY_NAME_LAYER_ORIGIN GPoint(38, 110)

#define TIME_BACKGROUND_RECT GRect(28, 122, 60, 28)
#define DATE_BACKGROUND_RECT GRect(92, 122, 60, 28)
#define TIME_LAYER_RECT GRect(29, 123, 58, 24)
#define DATE_RECT GRect(93, 132, 60, 20)
#define DAY_OF_WEEK_RECT GRect(93, 120, 58, 20)
#define TEMPERATURE_RECT_BIG GRect(93, 123, 58, 24)
#define TEMPERATURE_RECT_SMALL GRect(93, 126, 58, 24)

#define ZEROTWO_ORIGIN GPoint(51, 18)
#define KING_ORIGIN GPoint(100, 49)
#define KING_DEFEAT_ORIGIN GPoint(95, 56)
#define KRACKO_ORIGIN GPoint(82, 18)
#define MR_BRIGHT_ORIGIN GPoint(106, 51)
#define MR_BRIGHT_DEFEAT_ORIGIN GPoint(105, 51)
#define MR_SHINE_ORIGIN GPoint(109, 66)
#define MR_SHINE_DEFEAT_ORIGIN GPoint(109, 60)
#define MR_FROSTY_ORIGIN GPoint(110, 64)
#define MR_FROSTY_DEFEAT_ORIGIN GPoint(110, 63)

#define ZEROTWO_NAME_ORIGIN GPoint(98, 110)
#define KING_NAME_ORIGIN GPoint(102, 110)
#define KRACKO_NAME_ORIGIN GPoint(101, 110)
#define MR_BRIGHT_NAME_ORIGIN GPoint(100, 110)
#define MR_SHINE_NAME_ORIGIN GPoint(102, 110)
#define MR_FROSTY_NAME_ORIGIN GPoint(98, 110)

#define INITIAL_ANIMATION_GETTER get_random_ability_animation()