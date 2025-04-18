#pragma once
#include <pebble.h>

#define NUM_ABILITIES 7

static GFont s_font;

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

static const int ABILITY_ANIMATIONS_RESOURCE_IDS[] = 
{
  RESOURCE_ID_KIRBY_BEAM_ANIM,
  RESOURCE_ID_KIRBY_CUTTER_ANIM,
  RESOURCE_ID_KIRBY_FIRE_ANIM,
  RESOURCE_ID_KIRBY_HAMMER_ANIM,
  RESOURCE_ID_KIRBY_MIKE_ANIM_1,
  RESOURCE_ID_KIRBY_SLEEP_ANIM,
  RESOURCE_ID_KIRBY_SWORD_ANIM,
};

static const GPoint KIRBY_ABILITIES_ORIGINS[] =
{
  {24, 52},  // Beam
  {27, 110},  // Cutter
  {26, 101},  // Fire
  {0, 73},  // Hammer
  {20, 110},  // Mike
  {22, 83},  // Sleep
  {9, 81},  // Sword
};

#define ABILITY_NAME_LAYER_ORIGIN GPoint(14, 25)

#define BATTERY_LAYER_RECT GRect(14,7,72,10)
#define TIME_LAYER_RECT GRect(10, 175, 82, 38)
#define DATE_TEMPERATURE_RECT GRect(108, 175, 82, 38)

#define FONT s_font