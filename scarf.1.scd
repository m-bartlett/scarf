scarf(1)

# NAME

scarf - Wayland screen area fetcher.

# SYNOPSIS

*scarf* [options...]

# DESCRIPTION

scarf is a command-line utility to select a region from Wayland compositors
which support the layer-shell protocol. Drag with the pointer to create a
selection. The selection is not confirmed on mouse-up; instead it remains on
screen and can be adjusted with the keyboard (see *KEYBOARD CONTROLS*). Press
_Enter_ to confirm the selection and print its geometry to standard output, or
press _q_/_Esc_ to cancel (scarf then exits with status 1 and prints nothing).

If the standard input is not a TTY or the -r option is used, scarf will read a
list of predefined rectangles for quick selection. Each line must be in the form
"<x>,<y> <width>x<height> [label]". The label is optional and can be any string
that doesn't contain newlines. It can be accessed using the "%l" sequence in a
format string.

While making a selection, the "X,Y WxH" geometry of the current selection is
drawn above the selection rectangle, or below it if there is not enough room at
the top of the screen.

# OPTIONS

*-h*, *--help*
	Show help message and quit.

*-b*, *--background* _color_
	Set background color. Default is #FFFFFF40. See *COLORS* for more detail.

*-c*, *--border-color* _color_
	Set border color. Default is #000000FF. See *COLORS* for more detail.

*-s*, *--selection-color* _color_
	Set selection color. Default is #00000000. See *COLORS* for more detail.

*-B*, *--choice-color* _color_
	Set color for highlighting predefined rectangles from standard input when not
	selected. Default is #FFFFFF40.

*-F*, *--font-family* _font family_
	Set the font family name used when drawing the geometry label. The available
	font family names guaranteed to work are the standard generic CSS2 options:
	serif, sans-serif, monospace, cursive and fantasy. It defaults to the
	sans-serif family name.

*-w*, *--border-weight* _weight_
	Set border weight. Default is 2.

*-f*, *--format* _format_
	Set format. See *FORMAT* for more detail.

*-p*, *--point*
	Select a single point instead of a rectangle. The point is drawn as a circle
	using the current border color and weight, and its coordinates are printed
	underneath. This mode ignores any predefined rectangles read from the
	standard input.

*-o*, *--outputs*
	Add predefined rectangles for all outputs, as if provided on standard input.
	The label will be the name of the output.

*-r*, *--restrict*
	Require the user to select one of the predefined rectangles. These can come
	from standard input, if *-o* is used, the rectangles of all display outputs.
	This option conflicts with *-p*.

*-a*, *--aspect-ratio* _width_:_height_
	Force selections to have the given aspect ratio. This constraint is not
	applied to the predefined rectangles specified using *-o*.

*-x*, *--crosshairs*
	Draw fullscreen crosshairs on the display output containing the cursor until
	a selection is started. This help aligning the origin of the selection.

# COLORS

Colors may be specified in #RRGGBB or #RRGGBBAA format. The # is optional.

# FORMAT

Interpreted sequences are:

%x	The x-coordinate of the selection

%y	The y-coordinate of the selection

%w	The width of the selection

%h	The height of the selection

%X	The x-coordinate of the selection with coordinates relative to the output
	containing the top left corner.

%Y	The y-coordinate of the selection with coordinates relative to the output
	containing the top left corner.

%W	The width of the selection cropped to the output containing the top left
	corner.

%H	The height of the selection cropped to the output containing the top left
	corner.

%l	Label included with region from stdin

%o	The name of the output containing the top left corner, or "<unknown>" if
	not known

The default format is "%x,%y %wx%h" followed by a newline.

# KEYBOARD CONTROLS

The following keyboard actions can be used during selection. They apply to the
current selection rectangle (or point in *-p* mode).

*Enter*/*Return*	Confirm the current selection, print its geometry to standard
output, and exit.

*q*/*Escape*	Cancel the selection, print nothing, and exit with status 1.

*Arrow keys* / *h j k l*	Move the entire selection rectangle in the respective
direction by 1 pixel, without changing its dimensions.

*Shift + Arrow/hjkl*	Move the selection rectangle as above, but by 10 pixel
increments.

*Ctrl + Arrow/hjkl*	Change the selection's dimensions relative to its top-left
corner by 1 pixel. Ctrl+Left/Right decrease/increase the width, Ctrl+Up/Down
decrease/increase the height.

*Ctrl + Shift + Arrow/hjkl*	Change the selection's dimensions as above, but by
10 pixel increments.

*Alt* (held) + *mouse*	Pan the selection's origin with the mouse without
changing its dimensions.

*Alt + Ctrl* (held) + *mouse*	Modify the selection's dimensions with the mouse,
relative to the corner nearest the pointer when the modifiers were pressed.

*Space*	 If currently making a selection, while space is held down, move the
entire selection rather than change the selection's size as you move the
pointer.

*Shift*	Experimental. While actively resizing with the mouse, if the *-a* option
wasn't specified, set the aspect ratio to 1:1 while shift is held down;
releasing it restores the un-constrained aspect ratio.


# AUTHORS

Maintained by m-bartlett <michael@mbartlett.dev>, who is assisted by other
open-source contributors. For more information about scarf development, see
https://github.com/m-bartlett/scarf.
