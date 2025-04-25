#include <pebble.h>
#include "batteryLayer.h"

static uint8_t s_batteryLevel;
static bool s_fBatteryPlugged;

#define BATTERY_LAYER_RECT GRect(8,12,50,10)

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
	GColor8 batteryColor = s_fBatteryPlugged ? GColorGreen : GColorRed;
	graphics_context_set_fill_color(ctx, batteryColor);
	graphics_fill_rect(ctx, GRect(0, 0, (s_batteryLevel * BATTERY_LAYER_RECT.size.w) / 100, 10), 0 /*cornerRadius*/, GCornerNone);;
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
