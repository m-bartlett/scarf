<p align="center">
	<img width="500" alt="scarf" src="https://github.com/user-attachments/assets/1c37b104-19b4-40ed-8b6b-02d836f219ba" />
	<br>
	<i><b>sc</b>reen <b>ar</b>ea <b>f</b>etcher</i>
</p>

## Building

Install dependencies:

* meson
* wayland
* cairo
* libxkbcommon
* scdoc (optional: man pages)

Then run:

```sh
git clone https://github.com/m-bartlett/scarf
cd scarf
meson setup build
ninja -C build
build/scarf
```

## Example usage

Select a region and print it to stdout:

```sh
scarf
```

Select a single point instead of a region:

```sh
scarf -p
```

Select an output and print its name:

```sh
scarf -o -f "%o"
```

Select a window under Sway, using `swaymsg` and `jq`:

```sh
swaymsg -t get_tree | jq -r '.. | select(.pid? and .visible?) | .rect | "\(.x),\(.y) \(.width)x\(.height)"' | scarf
```

Select a window without border under Sway, using `swaymsg` and `jq`:

```sh
swaymsg -t get_tree | jq -r '.. | select(.pid? and .visible?) | "\(.rect.x+.window_rect.x),\(.rect.y+.window_rect.y) \(.window_rect.width)x\(.window_rect.height)"' | scarf
```
## Contributing

[Send GitHub pull requests][GitHub].

## License

0BSD

[grim]: https://gitlab.freedesktop.org/emersion/grim
[GitHub]: https://github.com/m-bartlett/scarf
