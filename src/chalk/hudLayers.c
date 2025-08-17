#include <pebble.h>
#include "hudLayers.h"
#include "viewdefs.h"

GBitmap* s_pBitmapDateTimeBkgd;

BitmapLayer* s_pLayerTimeBkgd;
BitmapLayer* s_pLayerDateBkgd;

void load_HUD_layers(Layer* parent_layer)
{
	s_pBitmapDateTimeBkgd = gbitmap_create_with_resource(RESOURCE_ID_HUD_CHALK);
	
	s_pLayerTimeBkgd = bitmap_layer_create(GRect(28, 122, 60, 28));
	bitmap_layer_set_compositing_mode(s_pLayerTimeBkgd, GCompOpSet);
	bitmap_layer_set_bitmap(s_pLayerTimeBkgd, s_pBitmapDateTimeBkgd);
	layer_add_child(parent_layer, bitmap_layer_get_layer(s_pLayerTimeBkgd));
	
	s_pLayerDateBkgd = bitmap_layer_create(GRect(92, 122, 60, 28));
	bitmap_layer_set_compositing_mode(s_pLayerDateBkgd, GCompOpSet);
	bitmap_layer_set_bitmap(s_pLayerDateBkgd, s_pBitmapDateTimeBkgd);
	layer_add_child(parent_layer, bitmap_layer_get_layer(s_pLayerDateBkgd));
}

void unload_HUD_layers()
{
	if (s_pLayerDateBkgd)
	{
		layer_remove_from_parent(bitmap_layer_get_layer(s_pLayerDateBkgd));
		bitmap_layer_destroy(s_pLayerDateBkgd);
	}

	if (s_pLayerTimeBkgd)
	{
		layer_remove_from_parent(bitmap_layer_get_layer(s_pLayerTimeBkgd));
		bitmap_layer_destroy(s_pLayerTimeBkgd);
	}

	if (s_pBitmapDateTimeBkgd)
	{
		gbitmap_destroy(s_pBitmapDateTimeBkgd);
	}
}

