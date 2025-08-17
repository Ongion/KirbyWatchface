#include <pebble.h>
#include "hudLayers.h"
#include "viewdefs.h"


GBitmap* s_pBitmapHUDKirby;
BitmapLayer* s_pLayerHUDKirby;

GBitmap* s_pBitmapHUDBoss;
BitmapLayer* s_pLayerHUDBoss;

void load_HUD_layers(Layer* parent_layer)
{
	s_pBitmapHUDKirby = gbitmap_create_with_resource(RESOURCE_ID_HUD_KIRBY);
	s_pLayerHUDKirby = bitmap_layer_create(HUD_KIRBY_LAYER_RECT);
	bitmap_layer_set_compositing_mode(s_pLayerHUDKirby, GCompOpSet);
	bitmap_layer_set_bitmap(s_pLayerHUDKirby, s_pBitmapHUDKirby);
	layer_add_child(parent_layer, bitmap_layer_get_layer(s_pLayerHUDKirby));
	
	s_pBitmapHUDBoss = gbitmap_create_with_resource(RESOURCE_ID_HUD_BOSS);
	s_pLayerHUDBoss = bitmap_layer_create(HUD_BOSS_LAYER_RECT);
	bitmap_layer_set_compositing_mode(s_pLayerHUDBoss, GCompOpSet);
	bitmap_layer_set_bitmap(s_pLayerHUDBoss, s_pBitmapHUDBoss);
	layer_add_child(parent_layer, bitmap_layer_get_layer(s_pLayerHUDBoss));
}

void unload_HUD_layers()
{
	if (s_pLayerHUDBoss)
	{
		layer_remove_from_parent(bitmap_layer_get_layer(s_pLayerHUDBoss));
		bitmap_layer_destroy(s_pLayerHUDBoss);
	}

	if (s_pLayerHUDKirby)
	{
		layer_remove_from_parent(bitmap_layer_get_layer(s_pLayerHUDKirby));
		bitmap_layer_destroy(s_pLayerHUDKirby);
	}

	if (s_pBitmapHUDBoss)
	{
		gbitmap_destroy(s_pBitmapHUDBoss);
	}

	if (s_pBitmapHUDKirby)
	{
		gbitmap_destroy(s_pBitmapHUDKirby);
	}
}

