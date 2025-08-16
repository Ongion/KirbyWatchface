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
	{.type = AT_APNG, .animation.APNGAnimation = {RESOURCE_ID_KIRBY_BEAM_ANIM, {18, 53}}}
};

const AbilityAnimation CUTTER_ANIMATIONS[] =
{
	{.type = AT_APNG, .animation.APNGAnimation = {RESOURCE_ID_KIRBY_CUTTER_THROW_ANIM, {19, 82}}}
};

const AbilityAnimation FIRE_ANIMATIONS[] =
{
	{.type = AT_APNG, .animation.APNGAnimation = {RESOURCE_ID_KIRBY_FIRE_ANIM, {18, 67}}}
};

const AbilityAnimation HAMMER_ANIMATIONS[] =
{
	{.type = AT_APNG, .animation.APNGAnimation = {RESOURCE_ID_KIRBY_HAMMER_POUND_ANIM, {23, 69}}}
};

const AbilityAnimation MIKE_ANIMATIONS[] =
{
	{.type = AT_APNG, .animation.APNGAnimation = {RESOURCE_ID_KIRBY_MIKE_ANIM_1, {18, 80}}},
	{.type = AT_APNG, .animation.APNGAnimation = {RESOURCE_ID_KIRBY_MIKE_ANIM_2, {20, 84}}},
	{.type = AT_APNG, .animation.APNGAnimation = {RESOURCE_ID_KIRBY_MIKE_ANIM_3, {23, 52}}},
};

const AbilityAnimation SLEEP_ANIMATIONS[] =
{
	{.type = AT_APNG, .animation.APNGAnimation = {RESOURCE_ID_KIRBY_SLEEP_ANIM, {29, 59}}}
};

const AbilityAnimation SWORD_ANIMATIONS[] =
{
	{.type = AT_APNG, .animation.APNGAnimation = {RESOURCE_ID_KIRBY_SWORD_ANIM, {18, 69}}}
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