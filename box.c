#include "box.h"

bool box_intersect(const struct scarf_box *a, const struct scarf_box *b) {
	return a->x < b->x + b->width &&
		a->x + a->width > b->x &&
		a->y < b->y + b->height &&
		a->height + a->y > b->y;
}

bool in_box(const struct scarf_box *box, int32_t x, int32_t y) {
	return box->x <= x
		&& box->x + box->width > x
		&& box->y <= y
		&& box->y + box->height > y;
}

int32_t box_size(const struct scarf_box *box) {
	return box->width * box->height;
}
