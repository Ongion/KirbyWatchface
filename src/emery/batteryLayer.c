#include <pebble.h>
#include "batteryLayer.h"

static uint8_t s_batteryLevel;
static bool s_fBatteryPlugged;
static GBitmap* s_pBitmapBatteryBar;

Layer* g_pLayerBattery;

#define BATTERY_LAYER_RECT GRect(14,7,72,10)

void update_battery_resource(bool fIsPlugged)
{
	if (s_pBitmapBatteryBar)
	{
		gbitmap_destroy(s_pBitmapBatteryBar);
	}

	if (fIsPlugged)
	{
		s_pBitmapBatteryBar = gbitmap_create_with_resource(RESOURCE_ID_HEALTH_CHARGE);
	}
	else
	{
		s_pBitmapBatteryBar = gbitmap_create_with_resource(RESOURCE_ID_HEALTH_DISCHARGE);
	}
}

void update_battery_data(const BatteryChargeState* pState)
{
	if (!pState)
	{
		// We should never be able to get here?
		return;
	}
	
	if (s_fBatteryPlugged != pState->is_plugged || !s_pBitmapBatteryBar)
	{
		update_battery_resource(pState->is_plugged);
	}

	s_batteryLevel = pState->charge_percent;
	s_fBatteryPlugged = pState->is_plugged;
}

void battery_layer_update_callback(Layer* layer, GContext* ctx)
{
	graphics_context_set_compositing_mode(ctx, GCompOpAssign);
	graphics_draw_bitmap_in_rect(ctx, s_pBitmapBatteryBar, GRect(0, 0, (s_batteryLevel * BATTERY_LAYER_RECT.size.w) / 100, 10));
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

	if (s_pBitmapBatteryBar)
	{
		gbitmap_destroy(s_pBitmapBatteryBar);
	}
}
