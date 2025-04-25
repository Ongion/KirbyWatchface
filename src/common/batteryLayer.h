#pragma once
#include <pebble.h>

Layer* g_pLayerBattery;

void update_battery_data();
void battery_layer_update_callback(Layer* layer, GContext* ctx);
void load_battery_layer(Layer* parent_layer);
void unload_battery_layer();