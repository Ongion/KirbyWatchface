#include <pebble.h>
#include "commonTypes.h"
#include "viewdefs.h"

GFont s_font; 

void load_custom_fonts()
{
	s_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_HELVETICA_CUSTOM_BLACK_26));
}

void unload_custom_fonts()
{
	fonts_unload_custom_font(s_font);
}

void format_weather_layer_text(char* weather_layer_buffer, size_t sz_weather_layer_buffer, int temperature, TemperatureScale scalePreference)
{
	char scale = scalePreference == FAHRENHEIT ? 'F' : 'C';
	snprintf(weather_layer_buffer, sz_weather_layer_buffer, "%d°%c", temperature, scale);
}