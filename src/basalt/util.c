#include <pebble.h>
#include "commonTypes.h"
#include "viewdefs.h"

GFont s_fontDate;

void load_custom_fonts()
{
	s_fontDate = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_LECO_REGULAR_16));
}

void unload_custom_fonts()
{
	fonts_unload_custom_font(s_fontDate);
}

void format_weather_layer_text(char* weather_layer_buffer, size_t sz_weather_layer_buffer, int x100temperature, TemperatureScale scalePreference)
{
	int x10temperature = -((x100temperature + 5)/10);
	char scale = scalePreference == FAHRENHEIT ? 'F' : 'C';
	snprintf(weather_layer_buffer, sz_weather_layer_buffer, "%d.%d°%c", x10temperature/10, abs(x10temperature) % 10, scale);
}