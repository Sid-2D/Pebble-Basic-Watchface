#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static GFont s_time_font;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

static void main_window_load(Window *window) {
	// Get info about window
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);

	// Create the TextLayer with specific bounds
	s_time_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));

	// Create GFont
	s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_48));

	// Create GBitmap
	s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);

	// Create BitmapLayer to display the GBitmap
	s_background_layer = bitmap_layer_create(bounds);

	// Set the bitmap onto the layer and add to the window
	bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
	layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));

	// Improve layout for watchface
	text_layer_set_background_color(s_time_layer, GColorClear);
	text_layer_set_text_color(s_time_layer, GColorBlack);
	text_layer_set_text(s_time_layer, "20:07");
	text_layer_set_font(s_time_layer, s_time_font);
	text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

	// Add it as a child layer to Window's root layer
	layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) {
	// Destroy TextLayer
	text_layer_destroy(s_time_layer);
	fonts_unload_custom_font(s_time_font);
	gbitmap_destroy(s_background_bitmap);
	bitmap_layer_destroy(s_background_layer);
}

static void update_time() {
	// Get a tm structure
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);

	// Write current hours and minutes into buffer
	static char s_buffer[8];
	strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);

	// Display this time on TextLayer
	text_layer_set_text(s_time_layer, s_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	update_time();
}

static void init() {
	// Create main window element
	s_main_window = window_create();

	// Set handlers to manage elements inside window
	window_set_window_handlers(s_main_window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload
	});

	window_set_background_color(s_main_window, GColorRed);

	// Show window on watch, animated = true
	window_stack_push(s_main_window, true);

	// Register with TickTimerService
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
	update_time();
}

static void deinit() {
	// Destroy Window
	window_destroy(s_main_window);
}


int main(void) {
  init();
  // APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", s_window);
  app_event_loop();
  deinit();
}
