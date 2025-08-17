#include "animations.h"

const int ABILITIES_NAME_RESOURCE_IDS[] =
{
	RESOURCE_ID_BEAM,
	RESOURCE_ID_CUTTER,
	RESOURCE_ID_FIRE,
	RESOURCE_ID_HAMMER,
	RESOURCE_ID_MIKE,
	RESOURCE_ID_SLEEP,
	RESOURCE_ID_SWORD,
};

const AbilityAnimation BEAM_ANIMATIONS[] =
{
	{.type = AT_APNG, .animation.APNGAnimation = {RESOURCE_ID_KIRBY_BEAM_ANIM, {18, 48}}}
};

const AbilityAnimation CUTTER_ANIMATIONS[] =
{
	{.type = AT_APNG, .animation.APNGAnimation = {RESOURCE_ID_KIRBY_CUTTER_THROW_ANIM, {19, 74}}}
};

const AbilityAnimation FIRE_ANIMATIONS[] =
{
	{.type = AT_APNG, .animation.APNGAnimation = {RESOURCE_ID_KIRBY_FIRE_ANIM, {18, 63}}}
};

const AbilityAnimation HAMMER_ANIMATIONS[] =
{
	{.type = AT_APNG, .animation.APNGAnimation = {RESOURCE_ID_KIRBY_HAMMER_POUND_ANIM, {18, 63}}}
};

const AbilityAnimation MIKE_ANIMATIONS[] =
{
	{.type = AT_APNG, .animation.APNGAnimation = {RESOURCE_ID_KIRBY_MIKE_ANIM_1, {18, 74}}},
	{.type = AT_APNG, .animation.APNGAnimation = {RESOURCE_ID_KIRBY_MIKE_ANIM_2, {20, 78}}},
	{.type = AT_APNG, .animation.APNGAnimation = {RESOURCE_ID_KIRBY_MIKE_ANIM_3, {23, 46}}},
};

const AbilityAnimation SLEEP_ANIMATIONS[] =
{
	{.type = AT_APNG, .animation.APNGAnimation = {RESOURCE_ID_KIRBY_SLEEP_ANIM, {29, 54}}}
};

const AbilityAnimation SWORD_ANIMATIONS[] =
{
	{.type = AT_APNG, .animation.APNGAnimation = {RESOURCE_ID_KIRBY_SWORD_ANIM, {13, 63}}}
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