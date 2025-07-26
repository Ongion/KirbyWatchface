#include <pebble.h>
#include "commonTypes.h"
#include "viewdefs.h"

GFont s_fontBig;
GFont s_fontSmall;

void load_custom_fonts()
{
	s_fontBig = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_LECO_REGULAR_20));
	s_fontSmall = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_LECO_REGULAR_16));
}

void unload_custom_fonts()
{
	fonts_unload_custom_font(s_fontBig);
	fonts_unload_custom_font(s_fontSmall);
}