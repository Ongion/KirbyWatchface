#include "commonTypes.h"
#include "viewdefs.h"

// Basalt does not use custom fonts
void load_custom_fonts() {};
void unload_custom_fonts() {};

void format_weather_layer_text(char* weather_layer_buffer, size_t sz_weather_layer_buffer, int temperature, TemperatureScale unusedScalePreference)
{
	(void)unusedScalePreference;	// Parameter not used on basalt
	snprintf(weather_layer_buffer, sz_weather_layer_buffer, "%d°", temperature);
}