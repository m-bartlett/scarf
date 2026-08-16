#ifndef _SCARF_H
#define _SCARF_H

#include <stdbool.h>
#include <stdint.h>
#include <wayland-client.h>
#include <xkbcommon/xkbcommon.h>

#include "box.h"
#include "cursor-shape-v1-client-protocol.h"
#include "pool-buffer.h"
#include "wlr-layer-shell-unstable-v1-client-protocol.h"
#include "xdg-output-unstable-v1-client-protocol.h"

#define TOUCH_ID_EMPTY -1

struct scarf_selection {
  struct scarf_output *current_output;
  int32_t x, y;
  int32_t anchor_x, anchor_y;
  struct scarf_box selection;
  bool has_selection;
};

struct scarf_state {
  bool running;
  bool edit_anchor;

  // Modifier key state (tracked from keyboard events)
  bool alt_pressed;
  bool ctrl_pressed;
  bool shift_pressed;

  // Whether the pointer button is being used to resize from a corner
  // (alt+ctrl mouse mode). Records which corner is anchored.
  bool mouse_resize;
  bool mouse_pan;

  struct wl_display *display;
  struct wl_registry *registry;
  struct wl_shm *shm;
  struct wl_compositor *compositor;
  struct zwlr_layer_shell_v1 *layer_shell;
  struct zxdg_output_manager_v1 *xdg_output_manager;
  struct wp_cursor_shape_manager_v1 *cursor_shape_manager;
  struct wl_list outputs; // scarf_output::link
  struct wl_list seats;   // scarf_seat::link

  struct xkb_context *xkb_context;

  struct {
    uint32_t background;
    uint32_t border;
    uint32_t selection;
    uint32_t choice;
  } colors;

  const char *font_family;

  uint32_t border_weight;
  bool single_point;
  bool restrict_selection;
  bool crosshairs;
  bool resizing_selection;
  struct wl_list boxes; // scarf_box::link
  bool fixed_aspect_ratio;
  double aspect_ratio; // h / w

  struct scarf_box result;
};

struct scarf_output {
  struct wl_output *wl_output;
  struct scarf_state *state;
  struct wl_list link; // scarf_state::outputs

  struct scarf_box geometry;
  struct scarf_box logical_geometry;
  int32_t scale;

  struct wl_surface *surface;
  struct zwlr_layer_surface_v1 *layer_surface;

  struct zxdg_output_v1 *xdg_output;

  struct wl_callback *frame_callback;
  bool configured;
  bool dirty;
  int32_t width, height;
  struct pool_buffer buffers[2];
  struct pool_buffer *current_buffer;

  struct wl_cursor_theme *cursor_theme;
  struct wl_cursor_image *cursor_image;
};

struct scarf_seat {
  struct wl_surface *cursor_surface;
  struct scarf_state *state;
  struct wl_seat *wl_seat;
  struct wl_list link; // scarf_state::seats

  // keyboard:
  struct wl_keyboard *wl_keyboard;

  // selection (pointer/touch):

  struct scarf_selection pointer_selection;
  struct scarf_selection touch_selection;

  // pointer:
  struct wl_pointer *wl_pointer;
  enum wl_pointer_button_state button_state;

  // keymap:
  struct xkb_keymap *xkb_keymap;
  struct xkb_state *xkb_state;

  // key repeat:
  int repeat_timer_fd;      // timerfd, -1 if none
  int32_t repeat_rate;      // keys per second (0 = repeat disabled)
  int32_t repeat_delay;     // milliseconds before first repeat
  xkb_keysym_t repeat_sym;  // keysym currently being repeated

  // touch:
  struct wl_touch *wl_touch;
  int32_t touch_id;
};

bool box_intersect(const struct scarf_box *a, const struct scarf_box *b);

static inline struct scarf_selection *
scarf_seat_current_selection(struct scarf_seat *seat) {
  return seat->touch_selection.has_selection ? &seat->touch_selection
                                             : &seat->pointer_selection;
}
#endif
