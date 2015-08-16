/*
 * Constructs a Window housing an output TextLayer to show data for
 * one's health state based on the motion of the of the accelerometer.
 */
#include <pebble.h>
#include "popup.h"

static int SEIZURE_Z_THRESHOLD = 1500;
static Window    *s_main_window;
static TextLayer *s_output_layer;

/* 
 * Event handler for incoming accelerometer data events.
 */
static void data_handler(AccelData *data, uint32_t num_samples) {
  // Long lived buffer
  static char s_buffer[128];

  // Detected seizure-like hand motion
  if (data[0].z > SEIZURE_Z_THRESHOLD) {
    pop_window_push();  
  }

  else {
    snprintf(s_buffer, sizeof(s_buffer), "Health State: Neutral");
  }

  //Show the data
  text_layer_set_text(s_output_layer, s_buffer);
}

/*
 * Loads the primary app window resources and text frames.
 */
static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Create output TextLayer
  s_output_layer = text_layer_create(GRect(5, 0, window_bounds.size.w - 10, window_bounds.size.h));
  text_layer_set_font(s_output_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text_alignment(s_output_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_output_layer));
}

/*
 * Cleans up the primary app window resources and text frames.
 */
static void main_window_unload(Window *window) {
  text_layer_destroy(s_output_layer);
}

/*
 * Initializes the main resources for the application.
 */
static void init() {
  // Create main Window
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_stack_push(s_main_window, true);

  // Subscribe to the accelerometer data service
  int num_samples = 3;
  accel_data_service_subscribe(num_samples, data_handler);

  // Choose update rate
  accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
}

/*
 * Cleans up the main resources for the application.
 */
static void deinit() {
  window_destroy(s_main_window);
  accel_data_service_unsubscribe(); 
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
