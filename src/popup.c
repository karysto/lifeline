#include "popup.h"
#include <stdio.h>
#include <stdlib.h>

static Window    *s_pop_window;
static TextLayer *s_output_pop_layer;
static TextLayer *s_countdown_layer;
static AppTimer  *s_app_timer;
const  int  COUNTDOWN_SECONDS = 7;
static int  counter           = 7;
static char buf[]             = "1234567890";

/*
 * Performs one second of the countdown.
 * Writes the current countdown to the screen.
 */
static void app_timer_callback(void *context) {
   APP_LOG(APP_LOG_LEVEL_DEBUG, "timer callback"); 
   if (counter > 0) {
     counter--;

     // convert the counter into a string
     snprintf(buf, sizeof(buf), "%d", counter);
     text_layer_set_text(s_countdown_layer, buf);
     s_app_timer = app_timer_register(1000, app_timer_callback, NULL);
   } 

   else {
     app_timer_cancel(s_app_timer);
     window_stack_pop(true);
   }
}

/*
 * Emits two spaced-vibrations.
 */
static void emit_vibration_alerts(void) {
  static const uint32_t const segments[] = { 1600, 800, 800 };
  VibePattern pat = {
    .durations = segments,
    .num_segments = ARRAY_LENGTH(segments),
  };
  vibes_enqueue_custom_pattern(pat);
}

/*
 * Handle alarm termination key presses.
 */
void terminate_alarm_handler(ClickRecognizerRef recognizer, void *context) {
    // handler logic
    pop_window_pop();
}

/*
 * Subscribes handlers to click events.
 */
void terminate_click_config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_UP,     terminate_alarm_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, terminate_alarm_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN,   terminate_alarm_handler);
}

/*
 * Loads the popup window when seizure motion detected. 
 */
static void pop_window_load(Window *window) {
  counter = COUNTDOWN_SECONDS; 
  window_set_click_config_provider(s_pop_window, terminate_click_config_provider);

  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Create output TextLayer
  s_output_pop_layer = text_layer_create(GRect(5, 0, window_bounds.size.w - 10, window_bounds.size.h));
  text_layer_set_font(s_output_pop_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text(s_output_pop_layer, "Health State: Seizure");
  text_layer_set_text_alignment(s_output_pop_layer, GTextAlignmentCenter);
  text_layer_set_overflow_mode(s_output_pop_layer, GTextOverflowModeWordWrap);
  layer_add_child(window_layer, text_layer_get_layer(s_output_pop_layer));

  // Create output TextLayer for countdown
  s_countdown_layer = text_layer_create(GRect(30, 50, 100, 50));
  text_layer_set_font(s_countdown_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text(s_countdown_layer, "7");
  text_layer_set_text_alignment(s_countdown_layer, GTextAlignmentCenter);
  text_layer_set_overflow_mode(s_countdown_layer, GTextOverflowModeWordWrap);
  layer_add_child(window_layer, text_layer_get_layer(s_countdown_layer));

  // emit haptic feedback
  emit_vibration_alerts();

  // display the timer
  s_app_timer = app_timer_register(1000, app_timer_callback, NULL);
}

/*
 * Cleans up elements when the popup window closes.
 */
static void pop_window_unload(Window *window) {
  text_layer_destroy(s_output_pop_layer);
}

/*
 * Pushes the pop up window to the window stack frame.
 */
void pop_window_push() {
    s_pop_window = window_create();
    window_set_window_handlers(s_pop_window, (WindowHandlers) {
        .load = pop_window_load,
        .unload = pop_window_unload
    });
    window_stack_push(s_pop_window, true);
}

/*
 * Pops the pop up window off of the window stack frame.
 */
void pop_window_pop() {
    window_stack_pop(true);
}
