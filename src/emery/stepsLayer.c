#include <pebble.h>
#include "stepsLayer.h"

#define STEPS_LAYER_MAX_WIDTH 72

static GBitmap* s_pBitmapStepsBar;

Layer* g_pLayerSteps;
int g_steps;
bool g_fStepGoalMet;

void steps_layer_update_callback(Layer* layer, GContext* ctx)
{
	uint16_t steps_per_px = g_settings.stepsGoal / STEPS_LAYER_MAX_WIDTH;
	graphics_context_set_compositing_mode(ctx, GCompOpAssign);
	graphics_draw_bitmap_in_rect(ctx, s_pBitmapStepsBar, GRect((g_steps / steps_per_px), 0, STEPS_LAYER_MAX_WIDTH - (g_steps / steps_per_px), 10));
}

void load_steps_layer(Layer* parent_layer)
{
	s_pBitmapStepsBar = gbitmap_create_with_resource(RESOURCE_ID_BOSS_HEALTH);

	g_pLayerSteps = layer_create(GRect(10, 7, STEPS_LAYER_MAX_WIDTH, 10));
	layer_set_update_proc(g_pLayerSteps, &steps_layer_update_callback);
	layer_add_child(parent_layer, g_pLayerSteps);
}

void unload_steps_layer()
{
	if (g_pLayerSteps)
	{
		layer_remove_from_parent(g_pLayerSteps);
		layer_destroy(g_pLayerSteps);
	}

	if (s_pBitmapStepsBar)
	{
		gbitmap_destroy(s_pBitmapStepsBar);
	}
}
