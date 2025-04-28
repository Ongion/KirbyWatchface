#pragma once

#include <pebble.h>

typedef struct AnimationFrame
{
	uint32_t resourceID;
	GPoint pos;
	uint16_t delayMs;
} AnimationFrame;

typedef struct Animation
{
	uint16_t loops;
	uint16_t numFrames;
	AnimationFrame* pFrames;
} Animation;