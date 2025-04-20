#pragma once
#include <pebble.h>

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

static const AbilityAnimation BEAM_ANIMATIONS[] =
{
	{RESOURCE_ID_KIRBY_BEAM_ANIM, {24, 52}}
};

static const AbilityAnimation CUTTER_ANIMATIONS[] =
{
	{RESOURCE_ID_KIRBY_CUTTER_THROW_ANIM, {27, 110}},
	{RESOURCE_ID_KIRBY_CUTTER_COMBO_ANIM, {17, 41}}
};

static const AbilityAnimation FIRE_ANIMATIONS[] =
{
	{RESOURCE_ID_KIRBY_FIRE_ANIM, {26, 101}}
};

static const AbilityAnimation HAMMER_ANIMATIONS[] =
{
	{RESOURCE_ID_KIRBY_HAMMER_POUND_ANIM, {12, 99}},
	{RESOURCE_ID_KIRBY_HAMMER_FIRE_ANIM, {0, 73}}
};

static const AbilityAnimation MIKE_ANIMATIONS[] =
{
	{RESOURCE_ID_KIRBY_MIKE_ANIM_1, {20, 110}}
};

static const AbilityAnimation SLEEP_ANIMATIONS[] =
{
	{RESOURCE_ID_KIRBY_SLEEP_ANIM, {22, 83}}
};

static const AbilityAnimation SWORD_ANIMATIONS[] =
{
	{RESOURCE_ID_KIRBY_SWORD_ANIM, {9, 81}}
};

static const unsigned int NUM_ABILITY_ANIMATIONS[] =
{
	1,  // Beam
	2,  // Cutter
	1,  // Fire
	2,  // Hammer
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

#define ABILITY_NAME_LAYER_ORIGIN GPoint(14, 25)

#define BATTERY_LAYER_RECT GRect(14,7,72,10)
#define TIME_LAYER_RECT GRect(10, 175, 82, 38)
#define DATE_TEMPERATURE_RECT GRect(109, 175, 82, 38)

#define KING_ORIGIN GPoint(117, 90)

//CHANGE ME
#define KRACKO_ORIGIN GPoint(94, 66)
#define KRACKO_RAIN_ORIGIN GPoint(94, 36)

#define FONT s_font