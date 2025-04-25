#pragma once
#include <pebble.h>
#include "commonTypes.h"

extern ClaySettings g_settings;

int g_steps;
Layer* g_pLayerSteps;

void steps_layer_update_callback(Layer* layer, GContext* ctx);
void load_steps_layer(Layer* parent_layer);
void unload_steps_layer();