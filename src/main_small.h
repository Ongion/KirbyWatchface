#pragma once
#include <pebble.h>

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

static const AbilityAnimation BEAM_ANIMATIONS[] =
{
  {RESOURCE_ID_KIRBY_BEAM_ANIM, {0, 53}}
};

static const AbilityAnimation CUTTER_ANIMATIONS[] =
{
  {RESOURCE_ID_KIRBY_CUTTER_ANIM, {1, 82}}
};

static const AbilityAnimation FIRE_ANIMATIONS[] =
{
  {RESOURCE_ID_KIRBY_FIRE_ANIM, {0, 67}}
};

static const AbilityAnimation HAMMER_ANIMATIONS[] =
{
  {RESOURCE_ID_KIRBY_HAMMER_POUND_ANIM, {5, 69}}
};

static const AbilityAnimation MIKE_ANIMATIONS[] =
{
  {RESOURCE_ID_KIRBY_MIKE_ANIM_1, {0, 80}}
};

static const AbilityAnimation SLEEP_ANIMATIONS[] =
{
  {RESOURCE_ID_KIRBY_SLEEP_ANIM, {11, 59}}
};

static const AbilityAnimation SWORD_ANIMATIONS[] =
{
  {RESOURCE_ID_KIRBY_SWORD_ANIM, {0, 69}}
};

static const unsigned int NUM_ABILITY_ANIMATIONS[] =
{
  1,  // Beam
  1,  // Cutter
  1,  // Fire
  1,  // Hammer
  1,  // Mike
  1,  // Sleep
  1,  // Sword
};

static const AbilityAnimation* ABILITY_ANIMATION_SETS[] = 
{
  BEAM_ANIMATIONS,
  CUTTER_ANIMATIONS,
  FIRE_ANIMATIONS,
  HAMMER_ANIMATIONS,
  MIKE_ANIMATIONS,
  SLEEP_ANIMATIONS,
  SWORD_ANIMATIONS,
};

#define NUM_ABILITIES 7

#define ABILITY_NAME_LAYER_ORIGIN GPoint(14, 26)

#define BATTERY_LAYER_RECT GRect(9,13,50,10)
#define TIME_LAYER_RECT GRect(1, 130, 72, 26)
#define DATE_TEMPERATURE_RECT GRect(72, 130, 72, 26)

#define FONT fonts_get_system_font(FONT_KEY_LECO_20_BOLD_NUMBERS)