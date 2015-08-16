#include <pebble.h>

static Window *s_pop_window;

static void pop_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);
}

static void pop_window_unload(Window *window) {

}
