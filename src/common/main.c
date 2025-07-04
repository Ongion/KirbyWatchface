#include <pebble.h>
#include "main.h"

#include "animations.h"
#include "commonTypes.h"

#include "batteryLayer.h"
#include "glancing_api.h"
#include "stepsLayer.h"
#include "viewdefs.h"

static const ManualAnimation* currentAnimation;
static uint16_t nextFrame;
static uint16_t loopsRemaining;

// Add animation for steps goal being reached

static void load_background_layer(Layer* parent_layer)
{
	s_pBitmapBackground = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND);
	GRect frame = gbitmap_get_bounds(s_pBitmapBackground);
	frame.origin = (GPoint){0, layer_get_frame(parent_layer).size.h - frame.size.h};
	s_pLayerBackground = bitmap_layer_create(frame);
	bitmap_layer_set_compositing_mode(s_pLayerBackground, GCompOpSet);
	bitmap_layer_set_bitmap(s_pLayerBackground, s_pBitmapBackground);
	layer_add_child(parent_layer, bitmap_layer_get_layer(s_pLayerBackground));
}

static void unload_background_layer()
{
	if (s_pLayerBackground)
	{
		layer_remove_from_parent(bitmap_layer_get_layer(s_pLayerBackground));
		bitmap_layer_destroy(s_pLayerBackground);
	}
	if (s_pBitmapBackground)
	{
		gbitmap_destroy(s_pBitmapBackground);
	}
}

static void load_HUD_layers(Layer* parent_layer)
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

static void unload_HUD_layers()
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

static void load_ability_name_layer(Layer* parent_layer)
{
	s_pLayerAbilityName = bitmap_layer_create(GRectZero);
	bitmap_layer_set_compositing_mode(s_pLayerAbilityName, GCompOpSet);
	layer_add_child(parent_layer, bitmap_layer_get_layer(s_pLayerAbilityName));
}

static void unload_ability_name_layer()
{
	if (s_pLayerAbilityName)
	{
		layer_remove_from_parent(bitmap_layer_get_layer(s_pLayerAbilityName));
		bitmap_layer_destroy(s_pLayerAbilityName);
	}

	if (s_pBitmapAbilityName)
	{
		gbitmap_destroy(s_pBitmapAbilityName);
	}
}

static void load_boss_name_layer(Layer* parent_layer)
{
	s_pLayerBossName = bitmap_layer_create(GRectZero);
	bitmap_layer_set_compositing_mode(s_pLayerBossName, GCompOpSet);
	layer_add_child(parent_layer, bitmap_layer_get_layer(s_pLayerBossName));
}

static void unload_boss_name_layer()
{
	if (s_pLayerBossName)
	{
		layer_remove_from_parent(bitmap_layer_get_layer(s_pLayerBossName));
		bitmap_layer_destroy(s_pLayerBossName);
	}

	if (s_pBitmapBossName)
	{
		gbitmap_destroy(s_pBitmapBossName);
	}
}

static void load_boss_layer(Layer* parent_layer)
{
	s_pLayerBoss = bitmap_layer_create(GRectZero);
	bitmap_layer_set_compositing_mode(s_pLayerBoss, GCompOpSet);
	layer_add_child(parent_layer, bitmap_layer_get_layer(s_pLayerBoss));
}

static void unload_boss_layer()
{
	if (s_pLayerBoss)
	{
		layer_remove_from_parent(bitmap_layer_get_layer(s_pLayerBoss));
		bitmap_layer_destroy(s_pLayerBoss);
	}

	if (s_pBitmapBoss)
	{
		gbitmap_destroy(s_pBitmapBoss);
	}
}

static void load_kirby_layer(Layer* parent_layer)
{
	s_pLayerKirby = bitmap_layer_create(GRectZero);
	bitmap_layer_set_compositing_mode(s_pLayerKirby, GCompOpSet);
	layer_add_child(parent_layer, bitmap_layer_get_layer(s_pLayerKirby));
}

static void unload_kirby_layer()
{
	if (s_pLayerKirby)
	{
		layer_remove_from_parent(bitmap_layer_get_layer(s_pLayerKirby));
		bitmap_layer_destroy(s_pLayerKirby);
	}

	if (s_pBitmapSequenceKirby)
	{
		gbitmap_sequence_destroy(s_pBitmapSequenceKirby);
	}

	if (s_pBitmapKirby)
	{
		gbitmap_destroy(s_pBitmapKirby);
	}
}

static void load_time_layer(Layer* parent_layer)
{
	s_pTextLayerTime = text_layer_create(TIME_LAYER_RECT);
	text_layer_set_text_alignment(s_pTextLayerTime, GTextAlignmentCenter);
	text_layer_set_text_color(s_pTextLayerTime, GColorBlack);
	text_layer_set_background_color(s_pTextLayerTime, GColorClear);
	text_layer_set_font(s_pTextLayerTime, FONT);
	layer_add_child(parent_layer, text_layer_get_layer(s_pTextLayerTime));
}

static void unload_time_layer()
{
	if (s_pTextLayerTime)
	{
		layer_remove_from_parent(text_layer_get_layer(s_pTextLayerTime));
		text_layer_destroy(s_pTextLayerTime);
	}
}

static void load_date_layer(Layer* parent_layer)
{
	s_pTextLayerDate = text_layer_create(DATE_RECT);
	text_layer_set_text_alignment(s_pTextLayerDate, GTextAlignmentCenter);
	text_layer_set_text_color(s_pTextLayerDate, GColorBlack);
	text_layer_set_background_color(s_pTextLayerDate, GColorClear);
	text_layer_set_font(s_pTextLayerDate, FONT);
	layer_add_child(parent_layer, text_layer_get_layer(s_pTextLayerDate));
}

static void unload_date_layer()
{
	if (s_pTextLayerDate)
	{
		layer_remove_from_parent(text_layer_get_layer(s_pTextLayerDate));
		text_layer_destroy(s_pTextLayerDate);
	}
}

static void load_weather_layer(Layer* parent_layer)
{
	s_pTextLayerWeather = text_layer_create(TEMPERATURE_RECT);
	text_layer_set_background_color(s_pTextLayerWeather, GColorClear);
	text_layer_set_text_color(s_pTextLayerWeather, GColorBlack);
	text_layer_set_font(s_pTextLayerWeather, FONT);
	text_layer_set_text_alignment(s_pTextLayerWeather, GTextAlignmentCenter);
	update_weather_layer_text();
	layer_add_child(parent_layer, text_layer_get_layer(s_pTextLayerWeather));
	layer_set_hidden(text_layer_get_layer(s_pTextLayerWeather), true);
}

static void unload_weather_layer()
{
	if (s_pTextLayerWeather)
	{
		layer_remove_from_parent(text_layer_get_layer(s_pTextLayerWeather));
		text_layer_destroy(s_pTextLayerWeather);
	}
}

void update_weather_layer_text()
{
	static char weather_layer_buffer[10];
	int finalTemp;

	if (g_settings.scalePreference == FAHRENHEIT)
	{
		finalTemp = (s_temperature - 273.15) * 1.8 + 32;
	}
	else // (g_settings.scalePreference == CELSIUS)
	{
		finalTemp = s_temperature - 273.15;
	}

	format_weather_layer_text(weather_layer_buffer, sizeof(weather_layer_buffer), finalTemp, g_settings.scalePreference);
	text_layer_set_text(s_pTextLayerWeather, weather_layer_buffer);
}

static void set_container_image(GBitmap** bmp_image, BitmapLayer* bmp_layer, const int resource_id, GPoint origin)
{
	if (*bmp_image != NULL)
	{
		gbitmap_destroy(*bmp_image);
	}

	*bmp_image = gbitmap_create_with_resource(resource_id);
	GRect frame = (GRect){
		.origin = origin,
		.size = gbitmap_get_bounds(*bmp_image).size };
	bitmap_layer_set_bitmap(bmp_layer, *bmp_image);
	layer_set_frame(bitmap_layer_get_layer(bmp_layer), frame);
}

static void update_boss()
{
	if (!bt_connected)
	{
		set_container_image(&s_pBitmapBoss, s_pLayerBoss, RESOURCE_ID_ZEROTWO, ZEROTWO_ORIGIN);
		set_container_image(&s_pBitmapBossName, s_pLayerBossName, RESOURCE_ID_BT_DISCONNECTED, ZEROTWO_NAME_ORIGIN);
	}
	else if (((unsigned int)time(NULL) - (unsigned int)s_lastWeatherTime) > TIME_STALE_WEATHER)
	{
		if (g_fStepGoalMet)
		{
			set_container_image(&s_pBitmapBoss, s_pLayerBoss, RESOURCE_ID_KING_DEFEAT, KING_DEFEAT_ORIGIN);
		}
		else
		{
			set_container_image(&s_pBitmapBoss, s_pLayerBoss, RESOURCE_ID_KING, KING_ORIGIN);
		}
		set_container_image(&s_pBitmapBossName, s_pLayerBossName, RESOURCE_ID_NO_WEATHER, KING_NAME_ORIGIN);
	}
	else if (200 <= s_weatherCondition && s_weatherCondition < 300)
	{
		if (g_fStepGoalMet)
		{
			set_container_image(&s_pBitmapBoss, s_pLayerBoss, RESOURCE_ID_KRACKO_LIGHTNING_DEFEAT, KRACKO_ORIGIN);
		}
		else
		{
			set_container_image(&s_pBitmapBoss, s_pLayerBoss, RESOURCE_ID_KRACKO_LIGHTNING, KRACKO_ORIGIN);
		}

		set_container_image(&s_pBitmapBossName, s_pLayerBossName, RESOURCE_ID_KRACKO_NAME, KRACKO_NAME_ORIGIN);
	}
	else if (300 <= s_weatherCondition && s_weatherCondition < 600)
	{
		if (g_fStepGoalMet)
		{
			set_container_image(&s_pBitmapBoss, s_pLayerBoss, RESOURCE_ID_KRACKO_RAIN_DEFEAT, KRACKO_ORIGIN);
		}
		else
		{
			set_container_image(&s_pBitmapBoss, s_pLayerBoss, RESOURCE_ID_KRACKO_RAIN, KRACKO_ORIGIN);
		}

		set_container_image(&s_pBitmapBossName, s_pLayerBossName, RESOURCE_ID_KRACKO_NAME, KRACKO_NAME_ORIGIN);
	}
	else if (600 <= s_weatherCondition && s_weatherCondition < 700)
	{
		if (g_fStepGoalMet)
		{
			set_container_image(&s_pBitmapBoss, s_pLayerBoss, RESOURCE_ID_MR_FROSTY_DEFEAT, MR_FROSTY_ORIGIN);
		}
		else
		{
			set_container_image(&s_pBitmapBoss, s_pLayerBoss, RESOURCE_ID_MR_FROSTY, MR_FROSTY_ORIGIN);
		}

		set_container_image(&s_pBitmapBossName, s_pLayerBossName, RESOURCE_ID_MR_FROSTY_NAME, MR_FROSTY_NAME_ORIGIN);
	}
	else if (s_weatherCondition > 800)
	{
		if (g_fStepGoalMet)
		{
			set_container_image(&s_pBitmapBoss, s_pLayerBoss, RESOURCE_ID_KRACKO_DEFEAT, KRACKO_ORIGIN);
		}
		else
		{
			set_container_image(&s_pBitmapBoss, s_pLayerBoss, RESOURCE_ID_KRACKO, KRACKO_ORIGIN);
		}

		set_container_image(&s_pBitmapBossName, s_pLayerBossName, RESOURCE_ID_KRACKO_NAME, KRACKO_NAME_ORIGIN);
	}
	else if (daytime)
	{
		if (g_fStepGoalMet)
		{
			set_container_image(&s_pBitmapBoss, s_pLayerBoss, RESOURCE_ID_MR_BRIGHT_DEFEAT, MR_BRIGHT_DEFEAT_ORIGIN);
		}
		else
		{
			set_container_image(&s_pBitmapBoss, s_pLayerBoss, RESOURCE_ID_MR_BRIGHT, MR_BRIGHT_ORIGIN);
		}

		set_container_image(&s_pBitmapBossName, s_pLayerBossName, RESOURCE_ID_MR_BRIGHT_NAME, MR_BRIGHT_NAME_ORIGIN);
	}
	else
	{
		if (g_fStepGoalMet)
		{
			set_container_image(&s_pBitmapBoss, s_pLayerBoss, RESOURCE_ID_MR_SHINE_DEFEAT, MR_SHINE_DEFEAT_ORIGIN);
		}
		else
		{
			set_container_image(&s_pBitmapBoss, s_pLayerBoss, RESOURCE_ID_MR_SHINE, MR_SHINE_ORIGIN);
		}

		set_container_image(&s_pBitmapBossName, s_pLayerBossName, RESOURCE_ID_MR_SHINE_NAME, MR_SHINE_NAME_ORIGIN);
	}
}

static void manual_animation_timer_handler(void* context)
{
	if (nextFrame == currentAnimation->numFrames && loopsRemaining > 0)
	{
		loopsRemaining -= 1;
		nextFrame = 0;
	}

	if (nextFrame < currentAnimation->numFrames)
	{
		if (s_pBitmapKirby)
		{
			gbitmap_destroy(s_pBitmapKirby);
		}
	
		AnimationFrame* pAnimFrame = &(currentAnimation->pFrames[nextFrame]);
		s_pBitmapKirby = gbitmap_create_with_resource(pAnimFrame->resourceID);
		GRect frame = gbitmap_get_bounds(s_pBitmapKirby);
		frame.origin = pAnimFrame->pos;
		layer_set_frame(bitmap_layer_get_layer(s_pLayerKirby), frame);
		bitmap_layer_set_bitmap(s_pLayerKirby, s_pBitmapKirby);

		nextFrame++;
		s_pKirbyAnimationTimer = app_timer_register(pAnimFrame->delayMs, manual_animation_timer_handler, NULL);
	}
	else
	{
		s_pKirbyAnimationTimer = NULL;
	}
}

static void apng_animation_timer_handler(void* context)
{
	uint32_t next_delay;

	// Advance to the next APNG frame, and get the delay for this frame
	if (gbitmap_sequence_update_bitmap_next_frame(s_pBitmapSequenceKirby, s_pBitmapKirby, &next_delay))
	{

		// Set the new frame into the BitmapLayer
		bitmap_layer_set_bitmap(s_pLayerKirby, s_pBitmapKirby);

		// Timer for next frame
		s_pKirbyAnimationTimer = app_timer_register(next_delay, apng_animation_timer_handler, NULL);
	}
	else
	{
		// There were no more frames
		s_pKirbyAnimationTimer = NULL;
	}
}

const AbilityAnimation* get_random_ability_animation()
{
	unsigned int animationIdx = (rand() >> 4) % NUM_ABILITY_ANIMATIONS[abilityIdx];

	return &(ABILITY_ANIMATION_SETS[abilityIdx][animationIdx]);
}

static void load_and_play_manual_animation(const ManualAnimation* pAnimation)
{
	// Cancel any existing animation
	if (s_pKirbyAnimationTimer)
	{
		app_timer_cancel(s_pKirbyAnimationTimer);
		s_pKirbyAnimationTimer = NULL;
	}

	if (s_pBitmapSequenceKirby)
	{
		gbitmap_sequence_destroy(s_pBitmapSequenceKirby);
		s_pBitmapSequenceKirby = NULL;
	}

	if (s_pBitmapKirby)
	{
		gbitmap_destroy(s_pBitmapKirby);
		s_pBitmapKirby = NULL;
	}
	
	currentAnimation = pAnimation;
	nextFrame = 0;
	loopsRemaining = pAnimation->loops -1;

	manual_animation_timer_handler(NULL);
}

static void load_and_play_apng_animation(const APNGAnimation* pAnimation)
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


	// Create sequence
	s_pBitmapSequenceKirby = gbitmap_sequence_create_with_resource(pAnimation->resourceID);

	// Create blank GBitmap using APNG frame size
	GSize frame_size = gbitmap_sequence_get_bitmap_size(s_pBitmapSequenceKirby);
	s_pBitmapKirby = gbitmap_create_blank(frame_size, GBitmapFormat8Bit);

	// Set Frame Location
	GRect frame = {
		.origin = pAnimation->origin,
		.size = frame_size };

	layer_set_frame(bitmap_layer_get_layer(s_pLayerKirby), frame);

	apng_animation_timer_handler(NULL);
}

static void load_and_play_ability_animation(const AbilityAnimation* pAnimation)
{
	switch (pAnimation->type)
	{
		case AT_APNG:
		{
			load_and_play_apng_animation(&(pAnimation->animation.APNGAnimation));
			break;
		}
		case AT_Manual:
		{
			load_and_play_manual_animation(&(pAnimation->animation.manualAnimation));
			break;
		}
	}
}

static void glancing_handler(GlancingData *data)
{
	if (data->state == GLANCING_ACTIVE && !s_pKirbyAnimationTimer)
	{
		load_and_play_ability_animation(get_random_ability_animation());
	}
}

static void update_date_time_layers(const struct tm* tick_time)
{
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

void update_bg_color()
{
	window_set_background_color(s_WindowMain, bt_connected ? s_bgColorTime : GColorRed);
}

void update_bg_color_time(struct tm* current_time)
{
	APP_LOG(APP_LOG_LEVEL_INFO, "Sunrise hour: %d", s_sunriseHour);
	APP_LOG(APP_LOG_LEVEL_INFO, "Sunset hour: %d", s_sunsetHour);
	APP_LOG(APP_LOG_LEVEL_INFO, "Current hour: %d", current_time->tm_hour);

	if (s_sunriseHour-1 <= current_time->tm_hour && current_time->tm_hour < s_sunriseHour+2)
	{
		// Sunrise
		s_bgColorTime = GColorFromRGB(255, 0, 128);
		daytime = s_sunriseHour <= current_time->tm_hour;
	}
	else if (s_sunriseHour+2 <= current_time->tm_hour && current_time->tm_hour < s_sunsetHour-1)
	{
		// Daytime
		s_bgColorTime = GColorFromRGB(0, 170, 255);
		daytime = true;
	}
	else if (s_sunsetHour-1 <= current_time->tm_hour && current_time->tm_hour < s_sunsetHour+2)
	{
		// Sunset
		s_bgColorTime = GColorFromRGB(255, 170, 0);
		daytime = current_time->tm_hour < s_sunsetHour;
	}
	else //if (s_sunsetHour+2 <= current_time->tm_hour || current_time->tm_hour < s_sunriseHour-1)
	{
		// Night
		s_bgColorTime = GColorFromRGB(0, 0, 85);
		daytime = false;
	}

	update_bg_color();
}

static void weather_ended()
{
	// If the weather can't be updated show the error icon
	// APP_LOG(APP_LOG_LEVEL_INFO, "Weather timer ended");

	if (s_pWeatherTimeoutTimer != NULL)
	{
		update_boss();
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

	dict_write_cstring(iter, MESSAGE_KEY_OpenWeatherAPIKey, g_settings.openWeatherMapAPIKey);
	dict_write_cstring(iter, MESSAGE_KEY_City, g_settings.city);
	result = app_message_outbox_send();

	if (result != APP_MSG_OK)
	{
		APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the outbox: %d", (int)result);
	}

	s_pWeatherTimeoutTimer = app_timer_register(60000, weather_ended, NULL);
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
		strncpy(g_settings.openWeatherMapAPIKey, openWeatherAPIKey_t->value->cstring, sizeof(g_settings.openWeatherMapAPIKey));
		updated_settings = true;
	}

	// City
	Tuple* city_t = dict_find(iter, MESSAGE_KEY_City);
	if (city_t)
	{
		strncpy(g_settings.city, city_t->value->cstring, sizeof(g_settings.city));
		APP_LOG(APP_LOG_LEVEL_DEBUG, "GOT CITY %s", g_settings.city);
		updated_settings = true;
	}

	// PebbleKitReady
	Tuple* pebblekit_ready_t = dict_find(iter, MESSAGE_KEY_PebbleKitReady);
	if (pebblekit_ready_t)
	{
		pebbleKitReady = true;
		// APP_LOG(APP_LOG_LEVEL_DEBUG, "It is now %d", time(NULL));
		// APP_LOG(APP_LOG_LEVEL_DEBUG, "last got weather at %d", s_lastWeatherTime);
		if (((unsigned int)time(NULL) - (unsigned int)s_lastWeatherTime) > TIME_REFRESH_WEATHER)
		{
			// Weather last checked over 20 minutes ago.
			request_weather_update();
		}
	}

	// ScalePreference
	Tuple* scale_preference_t = dict_find(iter, MESSAGE_KEY_ScalePreference);
	if (scale_preference_t)
	{
		if (strcmp(scale_preference_t->value->cstring, "F") == 0)
		{
			g_settings.scalePreference = FAHRENHEIT;
		}
		else if (strcmp(scale_preference_t->value->cstring, "C") == 0)
		{
			g_settings.scalePreference = CELSIUS;
		}

		updated_settings = true;
		recalc_weather_text = true;
	}

	// StepsGoal
	Tuple* stepsgoal_t = dict_find(iter, MESSAGE_KEY_StepsGoal);
	if (stepsgoal_t)
	{
		g_settings.stepsGoal = stepsgoal_t->value->uint16;
		layer_mark_dirty(g_pLayerSteps);

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

	// Sunrise
	Tuple* sunrise_t = dict_find(iter, MESSAGE_KEY_Sunrise);
	if (sunrise_t)
	{
		if (sunrise_t->value->int32 != 0)
		{
			struct tm* sunrise_time = localtime(&sunrise_t->value->int32);
			s_sunriseHour = sunrise_time->tm_hour;
			persist_write_int(STORAGE_KEY_LastSeenSunriseHour, s_sunriseHour);
		}
	}

	// Sunset
	Tuple* sunset_t = dict_find(iter, MESSAGE_KEY_Sunset);
	if (sunset_t)
	{
		if (sunset_t->value->int32 != 0)
		{
			struct tm* sunset_time = localtime(&sunset_t->value->int32);
			s_sunsetHour = sunset_time->tm_hour;
			persist_write_int(STORAGE_KEY_LastSeenSunsetHour, s_sunsetHour);
		}
	}

	if (got_weather)
	{
		cancel_weather_timeout();
		s_lastWeatherTime = time(NULL);
		persist_write_int(STORAGE_KEY_LastTimeRecievedWeather, s_lastWeatherTime);
		update_boss();
	}

	if (recalc_weather_text)
	{
		update_weather_layer_text();
	}

	if (updated_settings)
	{
		persist_write_int(STORAGE_KEY_ClaySettingsVersion, 1);
		persist_write_data(STORAGE_KEY_ClaySettings, &g_settings, sizeof(g_settings));

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
		update_date_time_layers(tick_time);

		if (tick_time->tm_min % WEATHER_REFRESH_RATE_MIN == 0)
		{
			request_weather_update();
		}
	}

	if ((units_changed & HOUR_UNIT) != 0)
	{
		// Change abilities
		abilityIdx = (rand() >> 4) % NUM_ABILITIES;
		set_container_image(&s_pBitmapAbilityName, s_pLayerAbilityName, ABILITIES_NAME_RESOURCE_IDS[abilityIdx], ABILITY_NAME_LAYER_ORIGIN);
		load_and_play_ability_animation(INITIAL_ANIMATION_GETTER);
		
		update_bg_color_time(tick_time);
	}

	if ((units_changed & DAY_UNIT) != 0)
	{
		// Steps will have reset. Make sure we refresh.
		update_steps();
	}
}

void update_steps()
{
	layer_mark_dirty(g_pLayerSteps);

	if (!g_fStepGoalMet && g_steps >= g_settings.stepsGoal)
	{
		g_fStepGoalMet = true;
		update_boss();
	}
	else if (g_fStepGoalMet && g_steps < g_settings.stepsGoal)
	{
		g_fStepGoalMet = false;
		update_boss();
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
		g_steps = health_service_sum_today(HealthMetricStepCount);
		// APP_LOG(APP_LOG_LEVEL_INFO, "Steps: %d", steps);
		update_steps();
	}
	else
	{
		// APP_LOG(APP_LOG_LEVEL_INFO, "Step data unavailable");
	}
}

void handle_battery(BatteryChargeState charge)
{
	update_battery_data(&charge);
	layer_mark_dirty(g_pLayerBattery);
}

static void handle_bluetooth(bool connected)
{
	bt_connected = connected;
	update_boss();
	update_bg_color();

	if (!initiate_watchface)
	{
		if (connected)
		{
			vibes_double_pulse();
		}
		else
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
		load_and_play_ability_animation(get_random_ability_animation());
	}

	layer_set_hidden(text_layer_get_layer(s_pTextLayerDate), true);
	layer_set_hidden(text_layer_get_layer(s_pTextLayerWeather), false);
	s_pShowDateTimer = app_timer_register(2000, show_date_timer_handler, NULL);
}

static void main_window_load(Window* window)
{
	Layer* window_layer = window_get_root_layer(window);

	load_background_layer(window_layer);
	load_boss_layer(window_layer);
	load_kirby_layer(window_layer);
	load_HUD_layers(window_layer);
	load_battery_layer(bitmap_layer_get_layer(s_pLayerHUDKirby));
	load_steps_layer(bitmap_layer_get_layer(s_pLayerHUDBoss));
	load_ability_name_layer(window_layer);
	load_boss_name_layer(window_layer);
	
	load_custom_fonts();
	load_time_layer(window_layer);
	load_date_layer(window_layer);
	load_weather_layer(window_layer);

	glancing_service_subscribe(0, false, false, glancing_handler);
}

static void main_window_unload(Window* window)
{
	glancing_service_unsubscribe();

	unload_weather_layer();
	unload_date_layer();
	unload_time_layer();

	unload_custom_fonts();

	unload_boss_name_layer();
	unload_ability_name_layer();
	unload_steps_layer();
	unload_battery_layer();
	unload_HUD_layers();
	unload_kirby_layer();
	unload_boss_layer();
	unload_background_layer();
}

static void load_settings()
{
	g_settings.openWeatherMapAPIKey[0] = '\0';
	g_settings.city[0] = '\0';
	g_settings.scalePreference = CELSIUS;
	g_settings.stepsGoal = 5000;

	if (persist_exists(STORAGE_KEY_ClaySettings) && persist_exists(STORAGE_KEY_ClaySettingsVersion))
	{
		if (persist_read_int(STORAGE_KEY_ClaySettingsVersion) == 1)
		{
			persist_read_data(STORAGE_KEY_ClaySettings, &g_settings, sizeof(g_settings));
		}
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

	if (persist_exists(STORAGE_KEY_LastSeenSunriseHour))
	{
		s_sunriseHour = persist_read_int(STORAGE_KEY_LastSeenSunriseHour);
	}

	if (persist_exists(STORAGE_KEY_LastSeenSunsetHour))
	{
		s_sunsetHour = persist_read_int(STORAGE_KEY_LastSeenSunsetHour);
	}
}

void handle_init(void)
{
	load_settings();

	s_WindowMain = window_create();

	window_set_window_handlers(s_WindowMain, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload,
	});

	time_t now = time(NULL);
	struct tm* tick_time = localtime(&now);

	window_stack_push(s_WindowMain, true);

	handle_tick(tick_time, MINUTE_UNIT | HOUR_UNIT | DAY_UNIT);

	tick_timer_service_subscribe(MINUTE_UNIT | HOUR_UNIT | DAY_UNIT, handle_tick);

	health_service_events_subscribe(handle_health, NULL);
	battery_state_service_subscribe(&handle_battery);

	handle_bluetooth(connection_service_peek_pebble_app_connection());
	connection_service_subscribe((ConnectionHandlers){&handle_bluetooth, NULL});

	accel_tap_service_subscribe(&handle_tap);

	// APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");

	initiate_watchface = false;

	update_bg_color_time(tick_time);
	update_date_time_layers(tick_time);
	update_boss();

	app_message_register_inbox_received(inbox_received_callback);
	app_message_register_inbox_dropped(inbox_dropped_callback);
	app_message_register_outbox_failed(outbox_failed_callback);
	app_message_register_outbox_sent(outbox_sent_callback);

	app_message_open(150, 150);
}

void handle_deinit(void)
{
	
	window_destroy(s_WindowMain);
	battery_state_service_unsubscribe();
	accel_tap_service_unsubscribe();
	connection_service_unsubscribe();
	tick_timer_service_unsubscribe();
	health_service_events_unsubscribe();
}

int main(void)
{
	handle_init();
	app_event_loop();
	handle_deinit();
}