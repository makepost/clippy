# clippy

Store multiple UTF-8 strings with different targets in GtkClipboard from Gjs.

## Use cases

- Copy or cut x-special/gnome-copied-files, a list of URIs to paste in Nautilus

- Copy text/html with text/plain fallback

## Todo

- [ ] Paste text given a target

- [ ] Gjs demo

- [ ] Review, find possible memory leaks

  - You know C? Please make a pull request.

- [ ] Merge to upstream GtkClipboard

  - You contribute to Gtk? Suggest what to change to make this possible.

## References

- https://bugzilla.gnome.org/show_bug.cgi?id=656312

- https://gitlab.gnome.org/GNOME/gjs/issues/14

- https://github.com/makepost/acme-commander/blob/master/bin/clipboard.py

## License

MIT
