#include <pebble.h>
#include "main.h"

#if PBL_DISPLAY_HEIGHT == 228
#include "main_large.h"
#else
#include "main_small.h"
#endif

// Add animation for steps goal being reached
// Fix memory fragmentation

static void load_background_layer(Layer* window_layer)
{
	s_pBitmapBackground = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND);
	s_pLayerBackground = bitmap_layer_create(layer_get_frame(window_layer));
	bitmap_layer_set_compositing_mode(s_pLayerBackground, GCompOpSet);
	bitmap_layer_set_bitmap(s_pLayerBackground, s_pBitmapBackground);
	layer_add_child(window_layer, bitmap_layer_get_layer(s_pLayerBackground));
}

void step_layer_update_callback(Layer* layer, GContext* ctx)
{
	uint16_t steps_per_px = settings.stepsGoal / 50;
	graphics_context_set_compositing_mode(ctx, GCompOpAssign);
	GColor8 stepColor = GColorWhite;
	graphics_context_set_stroke_color(ctx, stepColor);
	graphics_context_set_fill_color(ctx, stepColor);
	graphics_fill_rect(ctx, GRect(50 - (steps / steps_per_px), 0, (steps / steps_per_px), 10), 0, GCornerNone);
}

static void load_step_layer(Layer* window_layer)
{
	s_pLayerSteps = layer_create(GRect(85, 13, 50, 10));
	layer_set_update_proc(s_pLayerSteps, &step_layer_update_callback);
	layer_add_child(window_layer, s_pLayerSteps);
}

static void update_weather_layer_text()
{
	int finalTemp;
	static char weather_layer_buffer[10];

	if (settings.scalePreference == FAHRENHEIT)
	{
		finalTemp = (s_temperature - 273.15) * 1.8 + 32;
	}
	else // (settings.scalePreference == CELSIUS)
	{
		finalTemp = s_temperature - 273.15;
	}

	snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%d°", finalTemp);
	text_layer_set_text(s_pTextLayerWeather, weather_layer_buffer);
}

static void load_weather_layer(Layer* window_layer)
{
	s_pTextLayerWeather = text_layer_create(DATE_TEMPERATURE_RECT);
	text_layer_set_background_color(s_pTextLayerWeather, GColorClear);
	text_layer_set_text_color(s_pTextLayerWeather, GColorBlack);
	update_weather_layer_text();
	text_layer_set_font(s_pTextLayerWeather, FONT);
	text_layer_set_text_alignment(s_pTextLayerWeather, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(s_pTextLayerWeather));
	layer_set_hidden(text_layer_get_layer(s_pTextLayerWeather), true);
}

void battery_layer_update_callback(Layer* layer, GContext* ctx)
{
	graphics_context_set_compositing_mode(ctx, GCompOpAssign);
	GColor8 batteryColor = GColorRed;
	graphics_context_set_stroke_color(ctx, batteryColor);
	graphics_context_set_fill_color(ctx, batteryColor);
	graphics_draw_bitmap_in_rect(ctx, s_pBitmapBatteryBar, GRect(0, 0, (battery_level * BATTERY_LAYER_RECT.size.w) / 100, 10));
}

static void load_battery_resource()
{
	if (s_pBitmapBatteryBar)
	{
		gbitmap_destroy(s_pBitmapBatteryBar);
		s_pBitmapBatteryBar = NULL;
	}

	if (battery_plugged)
	{
		s_pBitmapBatteryBar = gbitmap_create_with_resource(RESOURCE_ID_HEALTH_CHARGE);
	}
	else
	{
		s_pBitmapBatteryBar = gbitmap_create_with_resource(RESOURCE_ID_HEALTH_DISCHARGE);
	}
}

static void load_battery_layer(Layer* window_layer)
{
	BatteryChargeState initial = battery_state_service_peek();
	battery_level = initial.charge_percent;
	battery_plugged = initial.is_plugged;
	load_battery_resource();
	s_pLayerBattery = layer_create(BATTERY_LAYER_RECT);
	layer_set_update_proc(s_pLayerBattery, &battery_layer_update_callback);
	layer_add_child(window_layer, s_pLayerBattery);
}

static void load_time_text_layer(Layer* window_layer)
{
	s_pTextLayerTime = text_layer_create(TIME_LAYER_RECT);
	text_layer_set_text_alignment(s_pTextLayerTime, GTextAlignmentCenter);
	text_layer_set_text_color(s_pTextLayerTime, GColorBlack);
	text_layer_set_background_color(s_pTextLayerTime, GColorClear);
	text_layer_set_font(s_pTextLayerTime, FONT);
	layer_add_child(window_layer, text_layer_get_layer(s_pTextLayerTime));
}

static void load_date_text_layer(Layer* window_layer)
{
	s_pTextLayerDate = text_layer_create(DATE_TEMPERATURE_RECT);
	text_layer_set_text_alignment(s_pTextLayerDate, GTextAlignmentCenter);
	text_layer_set_text_color(s_pTextLayerDate, GColorBlack);
	text_layer_set_background_color(s_pTextLayerDate, GColorClear);
	text_layer_set_font(s_pTextLayerDate, FONT);
	layer_add_child(window_layer, text_layer_get_layer(s_pTextLayerDate));
}

static void set_container_image(GBitmap** bmp_image, BitmapLayer* bmp_layer, const int resource_id, GPoint origin)
{
	GBitmap* old_image = *bmp_image;

	*bmp_image = gbitmap_create_with_resource(resource_id);
	GRect frame = (GRect){
		.origin = origin,
		.size = gbitmap_get_bounds(*bmp_image).size };
	bitmap_layer_set_bitmap(bmp_layer, *bmp_image);
	layer_set_frame(bitmap_layer_get_layer(bmp_layer), frame);

	if (old_image != NULL)
	{
		gbitmap_destroy(old_image);
	}
}

static void update_boss_layer()
{
	if (200 <= s_weatherCondition && s_weatherCondition < 300)
	{
		set_container_image(&s_pBitmapBoss, s_pLayerBoss, RESOURCE_ID_KRACKO_LIGHTNING, GPoint(64, 24));
	}
	else if ((300 <= s_weatherCondition && s_weatherCondition < 600) || s_weatherCondition > 800)
	{
		set_container_image(&s_pBitmapBoss, s_pLayerBoss, RESOURCE_ID_KRACKO, GPoint(64, 36));
	}
	else if (600 <= s_weatherCondition && s_weatherCondition < 700)
	{
		set_container_image(&s_pBitmapBoss, s_pLayerBoss, RESOURCE_ID_MR_FROSTY, GPoint(96, 69));
	}
	else if (daytime)
	{
		set_container_image(&s_pBitmapBoss, s_pLayerBoss, RESOURCE_ID_MR_BRIGHT, GPoint(94, 56));
	}
	else
	{
		set_container_image(&s_pBitmapBoss, s_pLayerBoss, RESOURCE_ID_MR_SHINE, GPoint(97, 69));
	}
}

static void kirby_animation_timer_handler(void* context)
{
	uint32_t next_delay;

	// Advance to the next APNG frame, and get the delay for this frame
	if (gbitmap_sequence_update_bitmap_next_frame(s_pBitmapSequenceKirby, s_pBitmapKirby, &next_delay))
	{

		// Set the new frame into the BitmapLayer
		bitmap_layer_set_bitmap(s_pLayerKirby, s_pBitmapKirby);

		// Timer for next frame
		s_pKirbyAnimationTimer = app_timer_register(next_delay, kirby_animation_timer_handler, NULL);
	}
	else
	{
		// There were no more frames
		s_pKirbyAnimationTimer = NULL;
	}
}

static void load_random_ability_animation()
{
	// Load an animation sequence for this ability

	// Cancel any existing animation
	if (s_pKirbyAnimationTimer)
	{
		app_timer_cancel(s_pKirbyAnimationTimer);
		s_pKirbyAnimationTimer = NULL;
	}

	// Free old data
	if (s_pBitmapSequenceKirby)
	{
		gbitmap_sequence_destroy(s_pBitmapSequenceKirby);
	}

	if (s_pBitmapKirby)
	{
		gbitmap_destroy(s_pBitmapKirby);
	}

	unsigned int animationIdx = rand() % NUM_ABILITY_ANIMATIONS[abilityIdx];

	AbilityAnimation animation = ABILITY_ANIMATION_SETS[abilityIdx][animationIdx];

	// Create sequence
	s_pBitmapSequenceKirby = gbitmap_sequence_create_with_resource(animation.resourceID);

	// Create blank GBitmap using APNG frame size
	GSize frame_size = gbitmap_sequence_get_bitmap_size(s_pBitmapSequenceKirby);
	s_pBitmapKirby = gbitmap_create_blank(frame_size, GBitmapFormat8Bit);

	// Set Frame Location
	GRect frame = {
		.origin = animation.origin,
		.size = frame_size };

	layer_set_frame(bitmap_layer_get_layer(s_pLayerKirby), frame);

	kirby_animation_timer_handler(NULL);
}

static void update_date_time_layers()
{
	time_t temp = time(NULL);
	struct tm* tick_time = localtime(&temp);

	static char time_text[] = "00:00";
	static char date_text[] = "00/00";

	char* date_format;

	date_format = "%m/%d";

	if (clock_is_24h_style() == true)
	{
		strftime(time_text, sizeof("00:00"), "%H:%M", tick_time);
	}
	else
	{
		strftime(time_text, sizeof("00:00"), "%I:%M", tick_time);
	}

	strftime(date_text, sizeof(date_text), date_format, tick_time);
	if (time_text[0] == '0')
	{
		memmove(time_text, &time_text[1], sizeof(time_text) - 1);
	}
	text_layer_set_text(s_pTextLayerTime, time_text);
	text_layer_set_text(s_pTextLayerDate, date_text);
}

static void update_bg_color(struct tm* current_time)
{
	if (current_time->tm_hour >= 12 && current_time->tm_hour < 17)
	{
		window_set_background_color(s_WindowMain, GColorFromRGB(0, 170, 255));
		daytime = true;
	}
	else if (current_time->tm_hour >= 5 && current_time->tm_hour < 12)
	{
		window_set_background_color(s_WindowMain, GColorFromRGB(255, 0, 128));
		daytime = true;
	}
	else if (current_time->tm_hour >= 17 && current_time->tm_hour < 21)
	{
		window_set_background_color(s_WindowMain, GColorFromRGB(255, 170, 0));
		daytime = false;
	}
	else if (current_time->tm_hour >= 21 || current_time->tm_hour < 5)
	{
		window_set_background_color(s_WindowMain, GColorFromRGB(0, 0, 85));
		daytime = false;
	}
}

static void weather_ended()
{
	// If the weather can't be updated show the error icon
	// APP_LOG(APP_LOG_LEVEL_INFO, "Weather timer ended");

	if (s_pWeatherTimeoutTimer != NULL)
	{
		// APP_LOG(APP_LOG_LEVEL_INFO, "Weather timer is not NULL");
		set_container_image(&s_pBitmapBoss, s_pLayerBoss, RESOURCE_ID_KING, KING_ORIGIN);
		s_pWeatherTimeoutTimer = NULL;
	}
}

static void cancel_weather_timeout()
{
	// Cancel the timeout once weather is received
	if (s_pWeatherTimeoutTimer != NULL)
	{
		// APP_LOG(APP_LOG_LEVEL_INFO, "Cancelling weather timer");
		app_timer_cancel(s_pWeatherTimeoutTimer);
		s_pWeatherTimeoutTimer = NULL;
	}
}

static void request_weather_update()
{
	if (!pebbleKitReady)
	{
		return;
	}

	DictionaryIterator* iter;

	// Prepare the outbox buffer for this message
	AppMessageResult result = app_message_outbox_begin(&iter);

	if (result != APP_MSG_OK)
	{
		// Outbox couldn't be used? Why?
		APP_LOG(APP_LOG_LEVEL_ERROR, "Error preparing the outbox: %d", (int)result);
	}

	int value = 1;
	dict_write_int(iter, MESSAGE_KEY_RequestWeather, &value, sizeof(int), true /*isSigned*/);

	dict_write_cstring(iter, MESSAGE_KEY_OpenWeatherAPIKey, settings.openWeatherMapAPIKey);
	dict_write_cstring(iter, MESSAGE_KEY_City, settings.city);
	result = app_message_outbox_send();

	if (result != APP_MSG_OK)
	{
		APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the outbox: %d", (int)result);
	}

	s_pWeatherTimeoutTimer = app_timer_register(10000, weather_ended, NULL);
}

static void inbox_received_callback(DictionaryIterator* iter, void* context)
{
	bool updated_settings = false;
	bool recalc_weather_text = false;
	bool got_weather = false;

	// OpenWeatherAPIKey
	Tuple* openWeatherAPIKey_t = dict_find(iter, MESSAGE_KEY_OpenWeatherAPIKey);
	if (openWeatherAPIKey_t)
	{
		strncpy(settings.openWeatherMapAPIKey, openWeatherAPIKey_t->value->cstring, sizeof(settings.openWeatherMapAPIKey));
		updated_settings = true;
	}

	// City
	Tuple* city_t = dict_find(iter, MESSAGE_KEY_City);
	if (city_t)
	{
		strncpy(settings.city, city_t->value->cstring, sizeof(settings.city));
		APP_LOG(APP_LOG_LEVEL_DEBUG, "GOT CITY %s", settings.city);
		updated_settings = true;
	}

	// PebbleKitReady
	Tuple* pebblekit_ready_t = dict_find(iter, MESSAGE_KEY_PebbleKitReady);
	if (pebblekit_ready_t)
	{
		pebbleKitReady = true;
		APP_LOG(APP_LOG_LEVEL_DEBUG, "It is now %d", time(NULL));
		APP_LOG(APP_LOG_LEVEL_DEBUG, "last got weather at %d", s_lastWeatherTime);
		if (((unsigned int)time(NULL) - (unsigned int)s_lastWeatherTime) > 1800)
		{
			// Weather last checked over 30 minutes ago.
			request_weather_update();
		}
	}

	// ScalePreference
	Tuple* scale_preference_t = dict_find(iter, MESSAGE_KEY_ScalePreference);
	if (scale_preference_t)
	{
		if (strcmp(scale_preference_t->value->cstring, "F") == 0)
		{
			settings.scalePreference = FAHRENHEIT;
		}
		else if (strcmp(scale_preference_t->value->cstring, "C") == 0)
		{
			settings.scalePreference = CELSIUS;
		}

		updated_settings = true;
		recalc_weather_text = true;
	}

	// StepsGoal
	Tuple* stepsgoal_t = dict_find(iter, MESSAGE_KEY_StepsGoal);
	if (stepsgoal_t)
	{
		settings.stepsGoal = stepsgoal_t->value->uint16;
		updated_settings = true;
	}

	// Temperature
	Tuple* temperature_t = dict_find(iter, MESSAGE_KEY_Temperature);
	if (temperature_t)
	{
		s_temperature = temperature_t->value->int32;
		persist_write_int(STORAGE_KEY_LastSeenTemperature, s_temperature);
		recalc_weather_text = true;
		got_weather = true;
	}

	// WeatherCondition
	Tuple* icon_t = dict_find(iter, MESSAGE_KEY_WeatherCondition);
	if (icon_t)
	{
		s_weatherCondition = icon_t->value->int32;
		persist_write_int(STORAGE_KEY_LastSeenWeatherCondition, s_weatherCondition);
		got_weather = true;
	}

	if (got_weather)
	{
		cancel_weather_timeout();
		s_lastWeatherTime = time(NULL);
		persist_write_int(STORAGE_KEY_LastTimeRecievedWeather, s_lastWeatherTime);
		update_boss_layer();
	}

	if (recalc_weather_text)
	{
		update_weather_layer_text();
	}

	if (updated_settings)
	{
		persist_write_int(STORAGE_KEY_ClaySettingsVersion, 1);
		persist_write_data(STORAGE_KEY_ClaySettings, &settings, sizeof(settings));

		request_weather_update();
	}
}

static void inbox_dropped_callback(AppMessageResult reason, void* context)
{
	// APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator* iterator, AppMessageResult reason, void* context)
{
	// APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator* iterator, void* context)
{
	// APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void handle_tick(struct tm* tick_time, TimeUnits units_changed)
{
	if ((units_changed & MINUTE_UNIT) != 0)
	{
		update_date_time_layers();

		if (tick_time->tm_min % 30 == 0)
		{
			request_weather_update();
		}
	}

	if ((units_changed & HOUR_UNIT) != 0)
	{
		// Change abilities
		abilityIdx = (rand() % NUM_ABILITIES);
		// APP_LOG(APP_LOG_LEVEL_DEBUG, "random character generated [#%d].", powerIdx);

		set_container_image(&s_pBitmapAbilityName, s_pLayerAbilityName, ABILITIES_NAME_RESOURCE_IDS[abilityIdx], ABILITY_NAME_LAYER_ORIGIN);
		load_random_ability_animation();
		update_bg_color(tick_time);
	}
}

static void handle_health(HealthEventType event, void* context)
{
	time_t start = time_start_of_today();
	time_t end = time(NULL);
	HealthServiceAccessibilityMask mask = health_service_metric_accessible(HealthMetricStepCount, start, end);

	if (mask & HealthServiceAccessibilityMaskAvailable)
	{
		// APP_LOG(APP_LOG_LEVEL_INFO, "Step data available!");
		steps = health_service_sum_today(HealthMetricStepCount);
		// APP_LOG(APP_LOG_LEVEL_INFO, "Steps: %d", steps);
	}
	else
	{
		// APP_LOG(APP_LOG_LEVEL_INFO, "Step data unavailable");
	}
}

void handle_battery(BatteryChargeState charge)
{
	battery_level = charge.charge_percent;
	battery_plugged = charge.is_plugged;
	load_battery_resource();
	layer_mark_dirty(s_pLayerBattery);
}

static void handle_bluetooth(bool connected)
{
	if (connected)
	{
		if (!initiate_watchface)
		{
			vibes_double_pulse();
		}
	}
	else
	{
		if (!initiate_watchface)
		{
			vibes_enqueue_custom_pattern((VibePattern) {
				.durations = (uint32_t[]){ 100, 100, 100, 100, 100 },
					.num_segments = 5
			});
		}
	}
}

static void show_date_timer_handler(void* context)
{
	layer_set_hidden(text_layer_get_layer(s_pTextLayerDate), false);
	layer_set_hidden(text_layer_get_layer(s_pTextLayerWeather), true);
	s_pShowDateTimer = NULL;
}

static void handle_tap(AccelAxisType axis, int32_t direction)
{
	// Cancel existing showDate timer
	if (s_pShowDateTimer)
	{
		app_timer_cancel(s_pShowDateTimer);
	}

	if (!s_pKirbyAnimationTimer)
	{
		load_random_ability_animation();
	}

	layer_set_hidden(text_layer_get_layer(s_pTextLayerDate), true);
	layer_set_hidden(text_layer_get_layer(s_pTextLayerWeather), false);
	s_pShowDateTimer = app_timer_register(2000, show_date_timer_handler, NULL);
}

static void main_window_load(Window* window)
{
	Layer* window_layer = window_get_root_layer(window);

	load_background_layer(window_layer);
	load_step_layer(window_layer);
	load_weather_layer(window_layer);
	load_battery_layer(window_layer);
	load_time_text_layer(window_layer);
	load_date_text_layer(window_layer);
}

static void main_window_unload(Window* window)
{
	gbitmap_destroy(s_pBitmapBackground);
	bitmap_layer_destroy(s_pLayerBackground);

	layer_destroy(s_pLayerSteps);

	text_layer_destroy(s_pTextLayerWeather);

	gbitmap_destroy(s_pBitmapBatteryBar);
	layer_destroy(s_pLayerBattery);

	text_layer_destroy(s_pTextLayerTime);

	text_layer_destroy(s_pTextLayerDate);

	layer_remove_from_parent(bitmap_layer_get_layer(s_pLayerAbilityName));
	gbitmap_destroy(s_pBitmapAbilityName);
	bitmap_layer_destroy(s_pLayerAbilityName);

	layer_remove_from_parent(bitmap_layer_get_layer(s_pLayerKirby));
	gbitmap_sequence_destroy(s_pBitmapSequenceKirby);
	gbitmap_destroy(s_pBitmapKirby);
	bitmap_layer_destroy(s_pLayerKirby);

	layer_remove_from_parent(bitmap_layer_get_layer(s_pLayerBoss));
	gbitmap_destroy(s_pBitmapBoss);
	bitmap_layer_destroy(s_pLayerBoss);
}

static void load_default_settings()
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Loading defaults....");
	settings.openWeatherMapAPIKey[0] = '\0';
	settings.city[0] = '\0';
	settings.scalePreference = CELSIUS;
	settings.stepsGoal = 5000;
}

void handle_init(void)
{
	load_default_settings();

	if (persist_exists(STORAGE_KEY_ClaySettings) && persist_exists(STORAGE_KEY_ClaySettingsVersion))
	{
		if (persist_read_int(STORAGE_KEY_ClaySettingsVersion) == 1)
		{
			persist_read_data(STORAGE_KEY_ClaySettings, &settings, sizeof(settings));
		}

		APP_LOG(APP_LOG_LEVEL_DEBUG, "LOADED CITY %s", settings.city);
	}

	if (persist_exists(STORAGE_KEY_LastSeenTemperature))
	{
		s_temperature = persist_read_int(STORAGE_KEY_LastSeenTemperature);
	}

	if (persist_exists(STORAGE_KEY_LastSeenWeatherCondition))
	{
		s_weatherCondition = persist_read_int(STORAGE_KEY_LastSeenWeatherCondition);
	}

	if (persist_exists(STORAGE_KEY_LastTimeRecievedWeather))
	{
		s_lastWeatherTime = persist_read_int(STORAGE_KEY_LastTimeRecievedWeather);
	}

	s_WindowMain = window_create();

#if PBL_DISPLAY_HEIGHT == 228
	s_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_HELVETICA_CUSTOM_BLACK_26));
#endif

	window_set_window_handlers(s_WindowMain, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload,
	});

	time_t now = time(NULL);
	struct tm* tick_time = localtime(&now);

	window_stack_push(s_WindowMain, true);

	GRect dummy_frame = { {0, 0}, {0, 0} };

	s_pLayerAbilityName = bitmap_layer_create(dummy_frame);
	layer_add_child(window_get_root_layer(s_WindowMain), bitmap_layer_get_layer(s_pLayerAbilityName));
	bitmap_layer_set_compositing_mode(s_pLayerAbilityName, GCompOpSet);

	s_pLayerBoss = bitmap_layer_create(dummy_frame);
	layer_add_child(window_get_root_layer(s_WindowMain), bitmap_layer_get_layer(s_pLayerBoss));
	bitmap_layer_set_compositing_mode(s_pLayerBoss, GCompOpSet);

	s_pLayerKirby = bitmap_layer_create(dummy_frame);
	layer_add_child(window_get_root_layer(s_WindowMain), bitmap_layer_get_layer(s_pLayerKirby));
	bitmap_layer_set_compositing_mode(s_pLayerKirby, GCompOpSet);

	handle_tick(tick_time, MINUTE_UNIT | HOUR_UNIT);

	tick_timer_service_subscribe(MINUTE_UNIT | HOUR_UNIT, handle_tick);

	health_service_events_subscribe(handle_health, NULL);
	battery_state_service_subscribe(&handle_battery);

	handle_bluetooth(bluetooth_connection_service_peek());
	bluetooth_connection_service_subscribe(&handle_bluetooth);

	accel_tap_service_subscribe(&handle_tap);

	// APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");

	initiate_watchface = false;

	update_bg_color(tick_time);
	update_date_time_layers();
	update_boss_layer();

	app_message_register_inbox_received(inbox_received_callback);
	app_message_register_inbox_dropped(inbox_dropped_callback);
	app_message_register_outbox_failed(outbox_failed_callback);
	app_message_register_outbox_sent(outbox_sent_callback);

	app_message_open(150, 150);
}

void handle_deinit(void)
{
#if PBL_DISPLAY_HEIGHT == 228
	fonts_unload_custom_font(s_font);
#endif

	window_destroy(s_WindowMain);
	battery_state_service_unsubscribe();
	accel_tap_service_unsubscribe();
	bluetooth_connection_service_unsubscribe();
	bluetooth_connection_service_peek();
	tick_timer_service_unsubscribe();
	health_service_events_unsubscribe();
}

int main(void)
{
	handle_init();
	app_event_loop();
	handle_deinit();
}