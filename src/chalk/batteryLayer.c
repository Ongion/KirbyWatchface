#include <pebble.h>
#include "batteryLayer.h"

static uint8_t s_batteryLevel;
static bool s_fBatteryPlugged;

Layer* g_pLayerBattery;

#define BATTERY_LAYER_RECT GRect(0,0,90,180)

void update_battery_data(const BatteryChargeState* pState)
{
	if (!pState)
	{
		// We should never be able to get here?
		return;
	}
	
	s_batteryLevel = pState->charge_percent;
	s_fBatteryPlugged = pState->is_plugged;
}

void battery_layer_update_callback(Layer* layer, GContext* ctx)
{
	GColor8 batteryColor = s_fBatteryPlugged ? GColorGreen : GColorDarkCandyAppleRed;
	graphics_context_set_stroke_color(ctx, batteryColor);
	graphics_context_set_stroke_width(ctx, 7);

	int32_t wedgeAngle =  s_batteryLevel * 9 * TRIG_MAX_ANGLE / (10 * 360);
	int32_t wedgeMiddle = TRIG_MAX_ANGLE * 3/4;
	graphics_draw_arc(ctx, GRect(1,1,178,178), GOvalScaleModeFillCircle, wedgeMiddle-wedgeAngle, wedgeMiddle+wedgeAngle);
}

void load_battery_layer(Layer* parent_layer)
{
	BatteryChargeState initialState = battery_state_service_peek();
	update_battery_data(&initialState);

	g_pLayerBattery = layer_create(BATTERY_LAYER_RECT);
	layer_set_update_proc(g_pLayerBattery, &battery_layer_update_callback);
	layer_add_child(parent_layer, g_pLayerBattery);
}

void unload_battery_layer()
{
	if (g_pLayerBattery)
	{
		layer_remove_from_parent(g_pLayerBattery);
		layer_destroy(g_pLayerBattery);
	}
}
