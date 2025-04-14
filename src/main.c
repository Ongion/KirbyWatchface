#include <pebble.h>
#include "main.h"

/*
Fire (Size 11512)
Beam (Size 4755)
Cutter (Size 4339)
Sword (Size 3515)
Hammer (Size 3345)
Mike (Size 2483)
Sleep (Size 1150)
*/

//Fix Auto Hide Issue
//Add animation for steps goal being reached
//Fix memory fragmentation

Window *window;

time_t auto_hide;

static ClaySettings settings;
static bool initiate_watchface = true;
static bool daytime;

static bool pebbleKitReady = false;

AppTimer *weather_timeout;
static int timeout = 60000;

TextLayer *text_weather_layer;
TextLayer *text_time_layer;
TextLayer *text_date_layer;

static char weather_layer_buffer[32];

static Layer *steps_layer;
int steps;

static uint8_t battery_level;
static bool battery_plugged;
static Layer *battery_layer;

static GBitmap *foreground_image;
static BitmapLayer *foreground_layer;

static int s_temperature = -1;
static int s_weatherCondition = -1;
static time_t s_lastWeatherTime = 0;

int replay = 2;
int current_frame, starting_frame;
int ending_frame;
int delay;//delay between each frame is in milliseconds

static int ANIMATIONS = 7;
int seed_images2, start_number_image, random_image;

static GBitmap *powers_image;
static BitmapLayer *powers_layer;

static GBitmap *boss_image;
static BitmapLayer *boss_layer;

static GBitmap *kirby_image;
static BitmapLayer *kirby_layer;

const int animation_frames[] = {  

RESOURCE_ID_KIRBY_BEAM_1, //0
RESOURCE_ID_KIRBY_BEAM_2,
RESOURCE_ID_KIRBY_BEAM_3,
RESOURCE_ID_KIRBY_BEAM_4,
RESOURCE_ID_KIRBY_BEAM_5,
RESOURCE_ID_KIRBY_BEAM_6,
RESOURCE_ID_KIRBY_BEAM_7,
RESOURCE_ID_KIRBY_BEAM_8,
RESOURCE_ID_KIRBY_BEAM_9,
RESOURCE_ID_KIRBY_BEAM_10,
RESOURCE_ID_KIRBY_BEAM_11,
RESOURCE_ID_KIRBY_BEAM_12,
RESOURCE_ID_KIRBY_BEAM_13, //12
  
RESOURCE_ID_KIRBY_CUTTER_1, //13
RESOURCE_ID_KIRBY_CUTTER_2,
RESOURCE_ID_KIRBY_CUTTER_3,
RESOURCE_ID_KIRBY_CUTTER_4,
RESOURCE_ID_KIRBY_CUTTER_5,
RESOURCE_ID_KIRBY_CUTTER_6,
RESOURCE_ID_KIRBY_CUTTER_7,
RESOURCE_ID_KIRBY_CUTTER_8,
RESOURCE_ID_KIRBY_CUTTER_9,
RESOURCE_ID_KIRBY_CUTTER_10,
RESOURCE_ID_KIRBY_CUTTER_11,
RESOURCE_ID_KIRBY_CUTTER_12,
RESOURCE_ID_KIRBY_CUTTER_13,
RESOURCE_ID_KIRBY_CUTTER_14,
RESOURCE_ID_KIRBY_CUTTER_15,
RESOURCE_ID_KIRBY_CUTTER_16,
RESOURCE_ID_KIRBY_CUTTER_17, //29
  
RESOURCE_ID_KIRBY_FIRE_1, //30
RESOURCE_ID_KIRBY_FIRE_2,
RESOURCE_ID_KIRBY_FIRE_3,
RESOURCE_ID_KIRBY_FIRE_4,
RESOURCE_ID_KIRBY_FIRE_5,
RESOURCE_ID_KIRBY_FIRE_6,
RESOURCE_ID_KIRBY_FIRE_7,
RESOURCE_ID_KIRBY_FIRE_8,
RESOURCE_ID_KIRBY_FIRE_9,
RESOURCE_ID_KIRBY_FIRE_10,
RESOURCE_ID_KIRBY_FIRE_11,
RESOURCE_ID_KIRBY_FIRE_12,
RESOURCE_ID_KIRBY_FIRE_13,
RESOURCE_ID_KIRBY_FIRE_14,
RESOURCE_ID_KIRBY_FIRE_15,
RESOURCE_ID_KIRBY_FIRE_16,
RESOURCE_ID_KIRBY_FIRE_17,
RESOURCE_ID_KIRBY_FIRE_18,
RESOURCE_ID_KIRBY_FIRE_19,
RESOURCE_ID_KIRBY_FIRE_20,
RESOURCE_ID_KIRBY_FIRE_21,
RESOURCE_ID_KIRBY_FIRE_22,
RESOURCE_ID_KIRBY_FIRE_23, //52
  
RESOURCE_ID_KIRBY_HAMMER_1, //53
RESOURCE_ID_KIRBY_HAMMER_2,
RESOURCE_ID_KIRBY_HAMMER_3,
RESOURCE_ID_KIRBY_HAMMER_4,
RESOURCE_ID_KIRBY_HAMMER_5,
RESOURCE_ID_KIRBY_HAMMER_6,
RESOURCE_ID_KIRBY_HAMMER_7,
RESOURCE_ID_KIRBY_HAMMER_8,
RESOURCE_ID_KIRBY_HAMMER_9,
RESOURCE_ID_KIRBY_HAMMER_10,
RESOURCE_ID_KIRBY_HAMMER_11,
RESOURCE_ID_KIRBY_HAMMER_12,
RESOURCE_ID_KIRBY_HAMMER_13, //65
  
RESOURCE_ID_KIRBY_MIKE_1, //66
RESOURCE_ID_KIRBY_MIKE_2,
RESOURCE_ID_KIRBY_MIKE_3,
RESOURCE_ID_KIRBY_MIKE_4,
RESOURCE_ID_KIRBY_MIKE_5,
RESOURCE_ID_KIRBY_MIKE_6,
RESOURCE_ID_KIRBY_MIKE_7,
RESOURCE_ID_KIRBY_MIKE_8,
RESOURCE_ID_KIRBY_MIKE_9,
RESOURCE_ID_KIRBY_MIKE_10,
RESOURCE_ID_KIRBY_MIKE_11,
RESOURCE_ID_KIRBY_MIKE_12,
RESOURCE_ID_KIRBY_MIKE_13,
RESOURCE_ID_KIRBY_MIKE_14, //79
  
RESOURCE_ID_KIRBY_SLEEP_1, //80
RESOURCE_ID_KIRBY_SLEEP_2,
RESOURCE_ID_KIRBY_SLEEP_3,
RESOURCE_ID_KIRBY_SLEEP_4,
RESOURCE_ID_KIRBY_SLEEP_5,
RESOURCE_ID_KIRBY_SLEEP_6, //85
  
RESOURCE_ID_KIRBY_SWORD_1, //86
RESOURCE_ID_KIRBY_SWORD_2,
RESOURCE_ID_KIRBY_SWORD_3,
RESOURCE_ID_KIRBY_SWORD_4,
RESOURCE_ID_KIRBY_SWORD_5,
RESOURCE_ID_KIRBY_SWORD_6,
RESOURCE_ID_KIRBY_SWORD_7,
RESOURCE_ID_KIRBY_SWORD_8,
RESOURCE_ID_KIRBY_SWORD_9, //94
};

const int POWERS_IMAGE_RESOURCE_IDS[] = 
{
  RESOURCE_ID_BEAM,
  RESOURCE_ID_CUTTER,
  RESOURCE_ID_FIRE,
  RESOURCE_ID_HAMMER,
  RESOURCE_ID_MIKE,
  RESOURCE_ID_SLEEP,
  RESOURCE_ID_SWORD,
};

static void load_foreground_layer(Layer *window_layer)
{
  foreground_image = gbitmap_create_with_resource(RESOURCE_ID_FOREGROUND);
  foreground_layer = bitmap_layer_create(layer_get_frame(window_layer));
  bitmap_layer_set_compositing_mode(foreground_layer, GCompOpSet);
  bitmap_layer_set_bitmap(foreground_layer, foreground_image);
 	layer_add_child(window_layer, bitmap_layer_get_layer(foreground_layer));
}

void step_layer_update_callback(Layer *layer, GContext *ctx) 
{
  uint16_t steps_per_px = settings.stepsGoal / 50;
  graphics_context_set_compositing_mode(ctx, GCompOpAssign);
  GColor8 stepColor = GColorWhite;
  graphics_context_set_stroke_color(ctx, stepColor);
  graphics_context_set_fill_color(ctx,  stepColor);
  graphics_fill_rect(ctx, GRect(50 - (steps / steps_per_px), 0,(steps / steps_per_px), 10), 0, GCornerNone);
}

static void load_step_layer(Layer *window_layer)
{
 	steps_layer = layer_create(GRect(85,13,50,10));
 	layer_set_update_proc(steps_layer, &step_layer_update_callback);  
  layer_add_child(window_layer, steps_layer);
}

static void update_weather_layer_text()
{
  int finalTemp;

  if(settings.scalePreference == FAHRENHEIT)
  {
    finalTemp = (s_temperature - 273.15) * 1.8 + 32;
  }
  else // (settings.scalePreference == CELSIUS)
  {
    finalTemp = s_temperature - 273.15;
  }

  snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%d°", finalTemp);
  text_layer_set_text(text_weather_layer, weather_layer_buffer);
}

static void load_weather_layer(Layer *window_layer)
{
  text_weather_layer = text_layer_create(GRect(72, 130, 72, 26));
  text_layer_set_background_color(text_weather_layer, GColorClear);
  text_layer_set_text_color(text_weather_layer, GColorBlack);
  update_weather_layer_text();
  text_layer_set_font(text_weather_layer,fonts_get_system_font(FONT_KEY_LECO_20_BOLD_NUMBERS));
  text_layer_set_text_alignment(text_weather_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_weather_layer));
  layer_set_hidden(text_layer_get_layer(text_weather_layer), true);
}

void battery_layer_update_callback(Layer *layer, GContext *ctx) 
{
  graphics_context_set_compositing_mode(ctx, GCompOpAssign);
  GColor8 batteryColor = GColorRed;
  graphics_context_set_stroke_color(ctx, batteryColor);
  graphics_context_set_fill_color(ctx,  batteryColor);
  graphics_fill_rect(ctx, GRect(0, 0, (uint8_t)(battery_level)/2, 10), 0, GCornerNone);
}

static void load_battery_layer(Layer *window_layer)
{  
 	BatteryChargeState initial = battery_state_service_peek();
 	battery_level = initial.charge_percent;
 	battery_plugged = initial.is_plugged;
 	battery_layer = layer_create(GRect(9,13,50,10));
 	layer_set_update_proc(battery_layer, &battery_layer_update_callback);  
  layer_add_child(window_layer, battery_layer);
}

static void load_time_text_layer(Layer *window_layer)
{
  text_time_layer = text_layer_create(GRect(1, 130, 72, 26));
 	text_layer_set_text_alignment(text_time_layer, GTextAlignmentCenter);
 	text_layer_set_text_color(text_time_layer, GColorBlack);
 	text_layer_set_background_color(text_time_layer, GColorClear);
  text_layer_set_font(text_time_layer,fonts_get_system_font(FONT_KEY_LECO_20_BOLD_NUMBERS));
 	layer_add_child(window_layer, text_layer_get_layer(text_time_layer));  
}

static void load_date_text_layer(Layer *window_layer)
{
  text_date_layer = text_layer_create(GRect(72, 130, 72, 26));	
	text_layer_set_text_alignment(text_date_layer, GTextAlignmentCenter);
 	text_layer_set_text_color(text_date_layer, GColorBlack);
 	text_layer_set_background_color(text_date_layer, GColorClear);
  text_layer_set_font(text_date_layer,fonts_get_system_font(FONT_KEY_LECO_20_BOLD_NUMBERS));
 	layer_add_child(window_layer, text_layer_get_layer(text_date_layer)); 
}

static void set_container_image(GBitmap **bmp_image, BitmapLayer *bmp_layer, const int resource_id, GPoint origin) 
{
  GBitmap *old_image = *bmp_image;

 	*bmp_image = gbitmap_create_with_resource(resource_id);
 	GRect frame = (GRect)
  {
   	.origin = origin,
    .size = gbitmap_get_bounds(*bmp_image).size 
  };
 	bitmap_layer_set_bitmap(bmp_layer, *bmp_image);
 	layer_set_frame(bitmap_layer_get_layer(bmp_layer), frame);

 	if (old_image != NULL) {
 	  gbitmap_destroy(old_image);
  }
}

static void update_boss_layer()
{
  // // Thunderstorm
  // if (200 <= s_weatherCondition && s_weatherCondition < 300)
  // {
  //   set_container_image(&boss_image, boss_layer, RESOURCE_ID_KRACKO, GPoint(64, 36));
  // }

  // // Drizzle
  // else if (300 <= s_weatherCondition && s_weatherCondition < 400)
  // {
  //   set_container_image(&boss_image, boss_layer, RESOURCE_ID_KRACKO, GPoint(64, 36));
  // }

  // // Rain
  // else if (500 <= s_weatherCondition && s_weatherCondition < 600)
  // {
  //   set_container_image(&boss_image, boss_layer, RESOURCE_ID_KRACKO, GPoint(64, 36));
  // }
  if ((200 <= s_weatherCondition && s_weatherCondition < 700) || s_weatherCondition > 800)
  {
    set_container_image(&boss_image, boss_layer, RESOURCE_ID_KRACKO, GPoint(64, 36));
  }
  else if (daytime)
  {
    set_container_image(&boss_image, boss_layer, RESOURCE_ID_MR_BRIGHT, GPoint(94, 56));
  }
  else
  {
    set_container_image(&boss_image, boss_layer, RESOURCE_ID_MR_SHINE, GPoint(97, 69));
  }
}

static void timer_handler(void *context) 
{
  
  if(current_frame < ending_frame || replay > 0){
    if(current_frame == ending_frame){
     current_frame = starting_frame;
     replay--;
    }
    if (kirby_image != NULL) {
      gbitmap_destroy(kirby_image);
      kirby_image = NULL;
    }
    
    kirby_image = gbitmap_create_with_resource(animation_frames[current_frame]);
    
    bitmap_layer_set_bitmap(kirby_layer, kirby_image);
    layer_mark_dirty(bitmap_layer_get_layer(kirby_layer));

    current_frame++;
    app_timer_register(delay, timer_handler, NULL);
  }  
  
}


static void load_sequence() 
{
  //Beam
  if(random_image == 0){
  current_frame = 0;
  ending_frame = 13;
  delay = 77;
  }
  
  //Cutter
  if(random_image == 1){
  current_frame = 13;
  ending_frame = 30;
  delay = 59;
  }
  
  //Fire
  if(random_image == 2){
  current_frame = 31;
  ending_frame = 52;
  delay = 88;
  }
  
  //Hammer
  if(random_image == 3){
  current_frame = 53;
  ending_frame = 66;
  delay = 77;
  }
  
  //Mike
  if(random_image == 4){
  current_frame = 67;
  ending_frame = 80;
  delay = 72;
  }
  
  //Sleep
  if(random_image == 5){
  current_frame = 81;
  ending_frame = 86;
  delay = 167;
  }
  
  //Sword
  if(random_image == 6){
  current_frame = 87;
  ending_frame = 95;
  delay = 111;
  }

  starting_frame = current_frame;
  app_timer_register(1, timer_handler, NULL);
}

static void load_kirby_layer()
{ 
  if(random_image == 0){//Beam
   set_container_image(&kirby_image, kirby_layer, animation_frames[0], GPoint(0, 54));
  }
  else if(random_image == 1){//Cutter
   set_container_image(&kirby_image, kirby_layer, animation_frames[13], GPoint(0, 81));
  }
  else if(random_image == 2){//Fire
    set_container_image(&kirby_image, kirby_layer, animation_frames[30], GPoint(0, 66));
  }
  else if(random_image == 3){//Hammer
    set_container_image(&kirby_image, kirby_layer, animation_frames[53], GPoint(5, 69));
  }
  else if(random_image == 4){//Mike
    set_container_image(&kirby_image, kirby_layer, animation_frames[66], GPoint(0, 80));
  }
  else if(random_image == 5){//Sleep
    set_container_image(&kirby_image, kirby_layer, animation_frames[80], GPoint(13, 75));
  }
  else if(random_image == 6){//Sword
    set_container_image(&kirby_image, kirby_layer, animation_frames[86], GPoint(0, 69));
  }

}

static void update_date_time_layers()
{
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  
  static char time_text[] = "00:00";
 	static char date_text[] = "00/00";

 	char *date_format;
  
	date_format = "%m/%d";
  
  if(clock_is_24h_style() == true) {
    strftime(time_text, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    strftime(time_text, sizeof("00:00"), "%I:%M", tick_time);
  }
  
 	strftime(date_text, sizeof(date_text), date_format, tick_time);  
  if (time_text[0] == '0') {
   		memmove(time_text, &time_text[1], sizeof(time_text) - 1);
	}   
  text_layer_set_text(text_time_layer, time_text);
 	text_layer_set_text(text_date_layer, date_text);
}

static void update_bg_color(struct tm *current_time) 
{
  if (current_time->tm_hour >= 12 && current_time->tm_hour < 17){
    window_set_background_color(window, GColorFromRGB(0,170,255));
    daytime=true;
  }
  else if (current_time->tm_hour >= 5 && current_time->tm_hour < 12){
    window_set_background_color(window, GColorFromRGB(255,0,128));
    daytime=true;
  } 
  else if (current_time->tm_hour >= 17 && current_time->tm_hour < 21){
    window_set_background_color(window, GColorFromRGB(255,170,0));
    daytime=false;
  } 
  else if (current_time->tm_hour >= 21 || current_time->tm_hour < 5){
    window_set_background_color(window, GColorFromRGB(0,0,85));
    daytime=false;
  }
}

static void weather_ended() 
{
	// If the weather can't be updated show the error icon
	//APP_LOG(APP_LOG_LEVEL_INFO, "Weather timer ended");
	
	if (weather_timeout != NULL) {
		//APP_LOG(APP_LOG_LEVEL_INFO, "Weather timer is not NULL");
    set_container_image(&boss_image, boss_layer, RESOURCE_ID_KING, GPoint(82, 53));
    weather_timeout = NULL;
	}
}

static void cancel_weather_timeout() 
{
	// Cancel the timeout once weather is received
	if (weather_timeout != NULL) {
			//APP_LOG(APP_LOG_LEVEL_INFO, "Cancelling weather timer");
			app_timer_cancel(weather_timeout);
			weather_timeout = NULL;
	}
}

static void request_weather_update()
{
  DictionaryIterator *iter;
  
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

  weather_timeout = app_timer_register(timeout, weather_ended, NULL);
}

static void inbox_received_callback(DictionaryIterator *iter, void *context) 
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
    if(strcmp(scale_preference_t->value->cstring, "F") == 0){
      settings.scalePreference = FAHRENHEIT;
    }
    else if(strcmp(scale_preference_t->value->cstring, "C") == 0){
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
    
    update_boss_layer();
  }

  if (got_weather)
  {
    cancel_weather_timeout();
    s_lastWeatherTime = time(NULL);
    persist_write_int(STORAGE_KEY_LastTimeRecievedWeather, s_lastWeatherTime);
  }

  if (recalc_weather_text)
  {
    update_weather_layer_text();
  }

  if (updated_settings)
  {
    persist_write_int(STORAGE_KEY_ClaySettingsVersion, 1);
    persist_write_data(STORAGE_KEY_ClaySettings, &settings, sizeof(settings));
    
    if (pebbleKitReady)
    {
      request_weather_update();
    }
  }
  
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) 
{
  //APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) 
{
  //APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) 
{
  //APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void update_display(struct tm *current_time) 
{
  start_number_image = (current_time->tm_sec) + (current_time->tm_min) + ANIMATIONS;

  static long seed_images = 100;
  seed_images  = (((seed_images * 214013L + 2531011L) >> 16) & 32767);
  seed_images2 = seed_images + start_number_image;
  random_image = (seed_images2 % ANIMATIONS);
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "random character generated [#%d].", random_image);

  load_sequence();
  set_container_image(&powers_image, powers_layer, POWERS_IMAGE_RESOURCE_IDS[random_image], GPoint(14, 26));
  load_kirby_layer();
  update_bg_color(current_time);
}

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed)
{    
  if( (units_changed & SECOND_UNIT) != 0 ) {
    if(time(NULL) == auto_hide){
      layer_set_hidden(text_layer_get_layer(text_date_layer), false);
      layer_set_hidden(text_layer_get_layer(text_weather_layer), true);
    } 
  }
  
  if( (units_changed & MINUTE_UNIT) != 0 ) {

    update_date_time_layers();
    
    if(tick_time->tm_min % 30 == 0) {
      request_weather_update();
    }
 }
  
    if( ((tick_time->tm_min == 0) && (tick_time->tm_sec == 0)) || (initiate_watchface == true) ){
      update_display(tick_time);
    }
}

static void handle_health(HealthEventType event, void *context) 
{
	time_t start = time_start_of_today();
	time_t end = time(NULL);
	HealthServiceAccessibilityMask mask = health_service_metric_accessible(HealthMetricStepCount, start, end);
		
		if (mask & HealthServiceAccessibilityMaskAvailable) {
				//APP_LOG(APP_LOG_LEVEL_INFO, "Step data available!");
				steps = health_service_sum_today(HealthMetricStepCount);
				//APP_LOG(APP_LOG_LEVEL_INFO, "Steps: %d", steps);
		} else {
				//APP_LOG(APP_LOG_LEVEL_INFO, "Step data unavailable");
		}
}

void handle_battery(BatteryChargeState charge) 
{
	battery_level = charge.charge_percent;
  battery_plugged = charge.is_plugged;
  layer_mark_dirty(battery_layer);
}

static void handle_bluetooth(bool connected) 
{	
	if (connected) {

		if (!initiate_watchface) {
			vibes_double_pulse();
		}
	}
	else {

		if (!initiate_watchface) {      
			vibes_enqueue_custom_pattern( (VibePattern) {
   				.durations = (uint32_t []) {100, 100, 100, 100, 100},
   				.num_segments = 5
			} );
		}	
	}
}

static void handle_tap(AccelAxisType axis, int32_t direction)
{ 
  replay = 2;
  load_sequence();
  auto_hide = time(NULL) + 4;
  layer_set_hidden(text_layer_get_layer(text_date_layer), true);
  layer_set_hidden(text_layer_get_layer(text_weather_layer), false);
}

static void main_window_load(Window *window) 
{
  Layer *window_layer = window_get_root_layer(window);
    
  load_foreground_layer(window_layer);
  load_step_layer(window_layer);
  load_weather_layer(window_layer);
  load_battery_layer(window_layer);
  load_time_text_layer(window_layer);
  load_date_text_layer(window_layer);
}

static void main_window_unload(Window *window) 
{  
  gbitmap_destroy(foreground_image);
  bitmap_layer_destroy(foreground_layer);

  layer_destroy(steps_layer);

  text_layer_destroy(text_weather_layer);

  layer_destroy(battery_layer);

  text_layer_destroy(text_time_layer);

  text_layer_destroy(text_date_layer);
  
  layer_remove_from_parent(bitmap_layer_get_layer(powers_layer));
  gbitmap_destroy(powers_image);
  bitmap_layer_destroy(powers_layer);
 	 
  
  layer_remove_from_parent(bitmap_layer_get_layer(kirby_layer));
  gbitmap_destroy(kirby_image);
  bitmap_layer_destroy(kirby_layer);
  
  layer_remove_from_parent(bitmap_layer_get_layer(boss_layer));
  gbitmap_destroy(boss_image);
  bitmap_layer_destroy(boss_layer);
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

  window = window_create();
  
  window_set_window_handlers(window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });   
    
  time_t now = time(NULL);
 	struct tm *tick_time = localtime(&now);

	window_stack_push(window, true); 
  
  GRect dummy_frame = { {0, 0}, {0, 0} };
  
  powers_layer = bitmap_layer_create(dummy_frame);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(powers_layer));
  bitmap_layer_set_compositing_mode(powers_layer, GCompOpSet);
  
  boss_layer = bitmap_layer_create(dummy_frame);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(boss_layer));
  bitmap_layer_set_compositing_mode(boss_layer, GCompOpSet);
  
  kirby_layer = bitmap_layer_create(dummy_frame);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(kirby_layer));
  bitmap_layer_set_compositing_mode(kirby_layer, GCompOpSet);

	handle_minute_tick(tick_time, MINUTE_UNIT);
  
 	tick_timer_service_subscribe(MINUTE_UNIT | SECOND_UNIT, handle_minute_tick);
  
  health_service_events_subscribe(handle_health, NULL);
  battery_state_service_subscribe (&handle_battery);
  
  handle_bluetooth(bluetooth_connection_service_peek());
  bluetooth_connection_service_subscribe(&handle_bluetooth);
  
  accel_tap_service_subscribe(&handle_tap);
  
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");
  
  initiate_watchface = false;
  
  update_bg_color(tick_time); 
  update_date_time_layers();
  update_boss_layer();
  
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  app_message_open(150,150); 
}

void handle_deinit(void) 
{  
	window_destroy(window);
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