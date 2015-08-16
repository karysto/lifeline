#include <pebble.h>

static Window *s_pop_window;
static TextLayer *s_output_pop_layer;

static void pop_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Create output TextLayer
  s_output_pop_layer = text_layer_create(GRect(5, 0, window_bounds.size.w - 10, window_bounds.size.h));
  text_layer_set_font(s_output_pop_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text(s_output_pop_layer, "seizure alert");
  text_layer_set_overflow_mode(s_output_pop_layer, GTextOverflowModeWordWrap);
  layer_add_child(window_layer, text_layer_get_layer(s_output_pop_layer));
}

static void pop_window_unload(Window *window) {
  // Destroy output TextLayer
  text_layer_destroy(s_output_pop_layer);
}
