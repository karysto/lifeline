#include <pebble.h>

static Window *s_pop_window;
static TextLayer *s_output_pop_layer;
static AppTimer *s_app_timer;

static void app_timer_callback(void *context){
    window_stack_pop(true);
}

static void pop_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Create output TextLayer
  s_output_pop_layer = text_layer_create(GRect(5, 0, window_bounds.size.w - 10, window_bounds.size.h));
  text_layer_set_font(s_output_pop_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text(s_output_pop_layer, "seizure alert");
  text_layer_set_overflow_mode(s_output_pop_layer, GTextOverflowModeWordWrap);
  layer_add_child(window_layer, text_layer_get_layer(s_output_pop_layer));

  // emit haptic feedback
  // Vibe pattern: ON for 1600ms, OFF for 800ms, ON for 800ms:
  static const uint32_t const segments[] = { 1600, 800, 800 };
  VibePattern pat = {
    .durations = segments,
    .num_segments = ARRAY_LENGTH(segments),
  };
  vibes_enqueue_custom_pattern(pat);

  // display the timer
  s_app_timer = app_timer_register(7000, app_timer_callback, NULL);
}

static void pop_window_unload(Window *window) {
  // Destroy output TextLayer
  text_layer_destroy(s_output_pop_layer);
}
