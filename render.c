#include <cairo/cairo.h>
#include <stdio.h>
#include <stdlib.h>

#include "pool-buffer.h"
#include "render.h"
#include "scarf.h"

static void set_source_u32(cairo_t *cairo, uint32_t color) {
	cairo_set_source_rgba(cairo, (color >> (3 * 8) & 0xFF) / 255.0,
		(color >> (2 * 8) & 0xFF) / 255.0,
		(color >> (1 * 8) & 0xFF) / 255.0,
		(color >> (0 * 8) & 0xFF) / 255.0);
}

static void draw_rect(cairo_t *cairo, struct scarf_box *box, uint32_t color) {
	set_source_u32(cairo, color);
	cairo_rectangle(cairo, box->x, box->y,
			box->width, box->height);
}

void render(struct scarf_output *output) {
	struct scarf_state *state = output->state;
	struct pool_buffer *buffer = output->current_buffer;
	cairo_t *cairo = buffer->cairo;

	// Clear
	cairo_set_operator(cairo, CAIRO_OPERATOR_SOURCE);
	set_source_u32(cairo, state->colors.background);
	cairo_paint(cairo);

	// Draw option boxes from input
	struct scarf_box *choice_box;
	wl_list_for_each(choice_box, &state->boxes, link) {
		if (box_intersect(&output->logical_geometry,
					choice_box)) {
			draw_rect(cairo, choice_box, state->colors.choice);
			cairo_fill(cairo);
		}
	}

	struct scarf_seat *seat;
	wl_list_for_each(seat, &state->seats, link) {
		struct scarf_selection *current_selection =
			scarf_seat_current_selection(seat);

		if (!current_selection->has_selection && state->crosshairs) {
			struct scarf_box *output_box = &output->logical_geometry;
			if (in_box(output_box, current_selection->x, current_selection->y)) {

				set_source_u32(cairo, state->colors.border);
				cairo_rectangle(cairo, output_box->x, current_selection->y, output->logical_geometry.width, 1);
				cairo_fill(cairo);
				cairo_rectangle(cairo, current_selection->x, output->logical_geometry.y, 1, output->logical_geometry.height);
				cairo_fill(cairo);
			}
		}

		if (!current_selection->has_selection) {
			continue;
		}

		if (!box_intersect(&output->logical_geometry,
			&current_selection->selection)) {
			continue;
		}
		struct scarf_box *sel_box = &current_selection->selection;

		draw_rect(cairo, sel_box, state->colors.selection);
		cairo_fill(cairo);

		// Draw border
		cairo_set_line_width(cairo, state->border_weight);
		draw_rect(cairo, sel_box, state->colors.border);
		cairo_stroke(cairo);

		// Draw the "X,Y WxH" geometry label above the rectangle, or below
		// it if there isn't enough room at the top.
		{
			cairo_select_font_face(cairo, state->font_family,
					       CAIRO_FONT_SLANT_NORMAL,
					       CAIRO_FONT_WEIGHT_NORMAL);
			cairo_set_font_size(cairo, 14);
			set_source_u32(cairo, state->colors.border);
			char geom[48];
			snprintf(geom, sizeof(geom), "%d,%d %dx%d",
				 sel_box->x, sel_box->y,
				 sel_box->width, sel_box->height);

			cairo_font_extents_t fe;
			cairo_font_extents(cairo, &fe);
			double text_height = fe.ascent + fe.descent;

			double top_of_output = output->logical_geometry.y;
			double label_y;
			if (sel_box->y - text_height - 4 >= top_of_output) {
				// enough room above
				label_y = sel_box->y - 6;
			} else {
				// not enough room, place below the rectangle
				label_y = sel_box->y + sel_box->height + fe.ascent + 4;
			}
			cairo_move_to(cairo, sel_box->x, label_y);
			cairo_show_text(cairo, geom);
		}
	}
}
