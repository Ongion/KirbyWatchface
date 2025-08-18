#include <pebble.h>
#include "stepsLayer.h"

Layer* g_pLayerSteps;
int g_steps;
bool g_fStepGoalMet;

void steps_layer_update_callback(Layer* layer, GContext* ctx)
{
	if (g_steps < g_settings.stepsGoal)
	{
		int stepsPct = (g_settings.stepsGoal - g_steps) * 100 / g_settings.stepsGoal;
		graphics_context_set_stroke_color(ctx, GColorDarkCandyAppleRed);
		graphics_context_set_stroke_width(ctx, 7);

		int32_t wedgeAngle =  stepsPct * 9 * TRIG_MAX_ANGLE / (10 * 360);
		int32_t wedgeMiddle = TRIG_MAX_ANGLE * 1/4;
		graphics_draw_arc(ctx, GRect(-90,1,178,178), GOvalScaleModeFillCircle, wedgeMiddle-wedgeAngle, wedgeMiddle+wedgeAngle);
	}
}

void load_steps_layer(Layer* parent_layer)
{
	g_pLayerSteps = layer_create(GRect(90, 0, 90, 180));
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
