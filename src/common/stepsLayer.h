#pragma once
#include <pebble.h>
#include "commonTypes.h"

extern ClaySettings g_settings;

extern int g_steps;
extern Layer* g_pLayerSteps;
extern bool g_fStepGoalMet;

void steps_layer_update_callback(Layer* layer, GContext* ctx);
void load_steps_layer(Layer* parent_layer);
void unload_steps_layer();