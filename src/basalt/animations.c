#include "animations.h"

const AbilityAnimation BEAM_ANIMATIONS[] =
{
	{.type = AT_APNG, .animation.APNGAnimation = {RESOURCE_ID_KIRBY_BEAM_ANIM, {0, 53}}}
};

const AbilityAnimation CUTTER_ANIMATIONS[] =
{
	{.type = AT_APNG, .animation.APNGAnimation = {RESOURCE_ID_KIRBY_CUTTER_THROW_ANIM, {1, 82}}}
};

const AbilityAnimation FIRE_ANIMATIONS[] =
{
	{.type = AT_APNG, .animation.APNGAnimation = {RESOURCE_ID_KIRBY_FIRE_ANIM, {0, 67}}}
};

const AbilityAnimation HAMMER_ANIMATIONS[] =
{
	{.type = AT_APNG, .animation.APNGAnimation = {RESOURCE_ID_KIRBY_HAMMER_POUND_ANIM, {5, 69}}}
};

const AbilityAnimation MIKE_ANIMATIONS[] =
{
	{.type = AT_APNG, .animation.APNGAnimation = {RESOURCE_ID_KIRBY_MIKE_ANIM_1, {0, 80}}},
	{.type = AT_APNG, .animation.APNGAnimation = {RESOURCE_ID_KIRBY_MIKE_ANIM_2, {2, 84}}},
	{.type = AT_APNG, .animation.APNGAnimation = {RESOURCE_ID_KIRBY_MIKE_ANIM_3, {5, 52}}},
};

const AbilityAnimation SLEEP_ANIMATIONS[] =
{
	{.type = AT_APNG, .animation.APNGAnimation = {RESOURCE_ID_KIRBY_SLEEP_ANIM, {11, 59}}}
};

const AbilityAnimation SWORD_ANIMATIONS[] =
{
	{.type = AT_APNG, .animation.APNGAnimation = {RESOURCE_ID_KIRBY_SWORD_ANIM, {0, 69}}}
};

const unsigned int NUM_ABILITY_ANIMATIONS[] =
{
	1,  // Beam
	1,  // Cutter
	1,  // Fire
	1,  // Hammer
	3,  // Mike
	1,  // Sleep
	1,  // Sword
};

const AbilityAnimation* ABILITY_ANIMATION_SETS[] =
{
	BEAM_ANIMATIONS,
	CUTTER_ANIMATIONS,
	FIRE_ANIMATIONS,
	HAMMER_ANIMATIONS,
	MIKE_ANIMATIONS,
	SLEEP_ANIMATIONS,
	SWORD_ANIMATIONS,
};