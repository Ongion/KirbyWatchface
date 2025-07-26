#include <pebble.h>
#include "commonTypes.h"
#include "viewdefs.h"

GFont s_fontBig;
GFont s_fontSmall;

void load_custom_fonts()
{
	s_fontBig = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_HELVETICA_CUSTOM_BLACK_26));
	s_fontSmall = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_HELVETICA_CUSTOM_BLACK_20));
}

void unload_custom_fonts()
{
	fonts_unload_custom_font(s_fontSmall);
	fonts_unload_custom_font(s_fontBig);
}