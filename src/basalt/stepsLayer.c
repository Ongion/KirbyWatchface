#include <pebble.h>
#include "stepsLayer.h"

#define STEPS_LAYER_MAX_WIDTH 50

void steps_layer_update_callback(Layer* layer, GContext* ctx)
{
	uint16_t steps_per_px = g_settings.stepsGoal / STEPS_LAYER_MAX_WIDTH;
	GColor8 stepColor = GColorRed;
	graphics_context_set_fill_color(ctx, stepColor);
	graphics_fill_rect(ctx, GRect((g_steps / steps_per_px), 0, STEPS_LAYER_MAX_WIDTH - (g_steps / steps_per_px), 10), 0, GCornerNone);
}

void load_steps_layer(Layer* parent_layer)
{
	g_pLayerSteps = layer_create(GRect(8, 12, STEPS_LAYER_MAX_WIDTH, 10));
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
}
