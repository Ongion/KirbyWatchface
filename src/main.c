#include <pebble.h>

#define KEY_PEBBLEKIT_READY 0
#define KEY_TEMPERATURE 1
#define KEY_ICON 2
#define KEY_SCALE 3
#define KEY_REQUEST_WEATHER 4
#define KEY_STEPSGOAL 6

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

static bool initiate_watchface = true;
static bool daytime;

AppTimer *weather_timeout;
static int timeout = 60000;

TextLayer *text_weather_layer;
TextLayer *text_time_layer;
TextLayer *text_date_layer;

static char weather_layer_buffer[32];

static Layer *steps_layer;
int steps, steps_per_px, stepgoal;

static uint8_t battery_level;
static bool battery_plugged;
static Layer *battery_layer;

static GBitmap *foreground_image;
static BitmapLayer *foreground_layer;

static int s_temperature = -1;
static int s_temperatureScale = -1;
static int s_icon = -1;
static time_t s_lastWeatherTime = 0;

int replay = 2;
int current_frame, starting_frame;
int ending_frame;
int delay;//delay between each frame is in milliseconds

static int ANIMATIONS = 7;
int seed_images2, start_number_image;

AppTimer* s_pKirbyAnimationTimer;

static GBitmap *powers_image;
static BitmapLayer *powers_layer;

static GBitmap *boss_image;
static BitmapLayer *boss_layer;

static GBitmap *s_kirbyBitmap;
static GBitmapSequence *s_kirbySequence;
static BitmapLayer *s_kirbyLayer;

typedef enum temperature_scales
{
  FAHRENHEIT,
  CELSIUS
} TemperatureScale;

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

const int POWERS_ANIMATIONS_RESOURCE_IDS[] = 
{
  // RESOURCE_ID_BEAM,
  // RESOURCE_ID_CUTTER,
  // RESOURCE_ID_FIRE,
  // RESOURCE_ID_HAMMER,
  // RESOURCE_ID_MIKE,
  // RESOURCE_ID_SLEEP,
  RESOURCE_ID_KIRBY_BEAM_ANIM,
  RESOURCE_ID_KIRBY_SWORD_ANIM,
  RESOURCE_ID_KIRBY_SWORD_ANIM,
  RESOURCE_ID_KIRBY_HAMMER_ANIM,
  RESOURCE_ID_KIRBY_SWORD_ANIM,
  RESOURCE_ID_KIRBY_SWORD_ANIM,
  RESOURCE_ID_KIRBY_SWORD_ANIM,
};

const GPoint POWERS_ORIGINS[] =
{
  // GPoint(0, 54),  // Beam
  // GPoint(0, 81),  // Cutter
  // GPoint(0, 66),  // Fire
  // GPoint(5, 69),  // Hammer
  // GPoint(0, 80),  // Mike
  // GPoint(13, 75), // Sleep
  {0, 53},  // Beam
  {0, 69},  // Sword
  {0, 69},  // Sword
  {5, 69},  // Hammer
  {0, 69},  // Sword
  {0, 69},  // Sword
  {0, 69},  // Sword
};

const int BOSSES_IMAGE_RESOURCE_IDS[] = 
{
  RESOURCE_ID_MR_BRIGHT,
  RESOURCE_ID_KRACKO,
  RESOURCE_ID_MR_SHINE,
  RESOURCE_ID_KING,
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
  stepgoal = persist_read_int(KEY_STEPSGOAL);
  steps_per_px = stepgoal / 50;
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

  if(s_temperatureScale == FAHRENHEIT)
  {
    finalTemp = (s_temperature - 273.15) * 1.8 + 32;
  }
  else // (s_temperatureScale == CELSIUS)
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
  if(s_icon == 0 && daytime==true){
    set_container_image(&boss_image, boss_layer, BOSSES_IMAGE_RESOURCE_IDS[0], GPoint(94, 56));
  }
  else if(s_icon == 1){
    set_container_image(&boss_image, boss_layer, BOSSES_IMAGE_RESOURCE_IDS[1], GPoint(64, 36));
  }
  else if(s_icon == 0 && daytime==false){
    set_container_image(&boss_image, boss_layer, BOSSES_IMAGE_RESOURCE_IDS[2], GPoint(97, 69));
  }
}

static void kirby_animation_timer_handler(void* context)
{
  uint32_t next_delay;

  // Advance to the next APNG frame, and get the delay for this frame
  if (gbitmap_sequence_update_bitmap_next_frame(s_kirbySequence, s_kirbyBitmap, &next_delay))
  {
    // Set the new frame into the BitmapLayer
    bitmap_layer_set_bitmap(s_kirbyLayer, s_kirbyBitmap);
    layer_mark_dirty(bitmap_layer_get_layer(s_kirbyLayer));

    // Timer for next frame
    s_pKirbyAnimationTimer = app_timer_register(next_delay, kirby_animation_timer_handler, NULL);
  }
  else
  {
    // There were no more frames
    s_pKirbyAnimationTimer = NULL;
  }

}

static void load_power_sequence(int powerIdx, GRect* pFrame)
{
  if (!pFrame)
  {
    // pFrame needs to point to a valid frame
    return;
  }

  // Free old data
  if (s_pKirbyAnimationTimer)
  {
    // Cancel any existing animation
    app_timer_cancel(s_pKirbyAnimationTimer);
  }  
  if (s_kirbySequence)
  {
    gbitmap_sequence_destroy(s_kirbySequence);
    s_kirbySequence = NULL;
  }
  if (s_kirbyBitmap)
  {
    gbitmap_destroy(s_kirbyBitmap);
    s_kirbyBitmap = NULL;
  }

  // Create sequence
  s_kirbySequence = gbitmap_sequence_create_with_resource(POWERS_ANIMATIONS_RESOURCE_IDS[powerIdx]);

  // Create blank GBitmap using APNG frame size
  GSize frame_size = gbitmap_sequence_get_bitmap_size(s_kirbySequence);
  s_kirbyBitmap = gbitmap_create_blank(frame_size, GBitmapFormat8Bit);

  // Set Frame Location
  *pFrame = (GRect) {
    .origin = POWERS_ORIGINS[powerIdx],
    .size = frame_size
  };
}

static void load_kirby_layer(int powerIdx)
{
  // Load the animation sequence for this power
  GRect frame;
  load_power_sequence(powerIdx, &frame);
  layer_set_frame(bitmap_layer_get_layer(s_kirbyLayer), frame);

  // Schedule timer to advance the first frame
  s_pKirbyAnimationTimer = app_timer_register(0, kirby_animation_timer_handler, NULL);
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
    set_container_image(&boss_image, boss_layer, BOSSES_IMAGE_RESOURCE_IDS[3], GPoint(82, 53));
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

  int value = 0; // Just a dummy value, we don't use this on the other end
  dict_write_int(iter, KEY_REQUEST_WEATHER, &result, sizeof(int), true /*isSigned*/);
  result = app_message_outbox_send();

  if (result != APP_MSG_OK)
  {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the outbox: %d", (int)result);
  }

  weather_timeout = app_timer_register(timeout, weather_ended, NULL);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) 
{
  Tuple *t = dict_read_first(iterator);

  bool recalc_weather_text = false;
  bool got_weather = false;
  
  while(t != NULL) {
    switch(t->key) {
      case KEY_PEBBLEKIT_READY:
        if ((time(NULL) - s_lastWeatherTime) > 1800)
        {
          // Weather last checked over 30 minutes ago.
          request_weather_update();
        }
        break;
      case KEY_SCALE:
        if(strcmp(t->value->cstring, "F") == 0){
          s_temperatureScale = FAHRENHEIT;
        }
        else if(strcmp(t->value->cstring, "C") == 0){
          s_temperatureScale = CELSIUS;
        }

        persist_write_int(KEY_SCALE, s_temperatureScale);
        recalc_weather_text = true;
        break;

      case KEY_TEMPERATURE:
        s_temperature = t->value->int32;
        persist_write_int(KEY_TEMPERATURE, s_temperature);
        recalc_weather_text = true;
        got_weather = true;
        break;
        
      case KEY_ICON:
        s_icon = t->value->int32;
        persist_write_int(KEY_ICON, s_icon);
        got_weather = true;
        
        update_boss_layer();

        break;
      
      case KEY_STEPSGOAL:
        stepgoal = t->value->int16;
        //APP_LOG(APP_LOG_LEVEL_INFO, "stepgoal is %d", stepgoal);
        persist_write_int(KEY_STEPSGOAL, stepgoal);
        break;
    }

    t = dict_read_next(iterator);
  }

  if (recalc_weather_text)
  {
    update_weather_layer_text();
  }
  
  if (got_weather)
  {
    cancel_weather_timeout();
    persist_write_int(KEY_REQUEST_WEATHER, time(NULL));
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
  int powerIdx = (seed_images2 % ANIMATIONS);
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "random character generated [#%d].", powerIdx);

  set_container_image(&powers_image, powers_layer, POWERS_IMAGE_RESOURCE_IDS[powerIdx], GPoint(14, 26));
  load_kirby_layer(powerIdx);
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
  // Cancel any existing animation
  if (s_pKirbyAnimationTimer)
  {
    app_timer_cancel(s_pKirbyAnimationTimer);
  }

  gbitmap_sequence_restart(s_kirbySequence);
  app_timer_register(1, kirby_animation_timer_handler, NULL);
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
  
  layer_remove_from_parent(bitmap_layer_get_layer(s_kirbyLayer));
  gbitmap_sequence_destroy(s_kirbySequence);
  gbitmap_destroy(s_kirbyBitmap);
  bitmap_layer_destroy(s_kirbyLayer);
  
  layer_remove_from_parent(bitmap_layer_get_layer(boss_layer));
  gbitmap_destroy(boss_image);
  bitmap_layer_destroy(boss_layer);
}

void handle_init(void) 
{
  if (persist_exists(KEY_TEMPERATURE))
  {
    s_temperature = persist_read_int(KEY_TEMPERATURE);
  }

  if (persist_exists(KEY_SCALE))
  {
    s_temperatureScale = (TemperatureScale) persist_read_int(KEY_SCALE);
  }

  if (persist_exists(KEY_ICON))
  {
    s_icon = persist_read_int(KEY_ICON);
  }

  if (persist_exists(KEY_REQUEST_WEATHER))
  {
    s_lastWeatherTime = persist_read_int(KEY_REQUEST_WEATHER);
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
  
  s_kirbyLayer = bitmap_layer_create(dummy_frame);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_kirbyLayer));
  bitmap_layer_set_compositing_mode(s_kirbyLayer, GCompOpSet);

  update_boss_layer();

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
  
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  app_message_open(64,64); 
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