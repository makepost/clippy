/* main.js
 *
 * Copyright (C) 2017 makepost
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name(s) of the above copyright
 * holders shall not be used in advertising or otherwise to promote the sale,
 * use or other dealings in this Software without prior written
 * authorization.
 */

const GLib = imports.gi.GLib;
const GObject = imports.gi.GObject;
const Gtk = imports.gi.Gtk;

const DemoWindow = GObject.registerClass(
class DemoWindow extends Gtk.ApplicationWindow {
    _init(params) {
        super._init(params);
        const board = new imports.gi.Clippy.Board({ clipboard: 'CLIPBOARD' });

        const box = new Gtk.VBox();
        this.add(box);

        const fileChooserWidget = new Gtk.FileChooserWidget({
          action: Gtk.FileChooserAction.OPEN,
          expand: true,
          select_multiple: true
        });
        box.add(fileChooserWidget);

        const button = new Gtk.Button({ label: 'Copy to clipboard' });
        box.add(button);
        button.connect('clicked', () => {
            const uris = fileChooserWidget.get_uris().join('\n');

            board.set_targets(
              ['x-special/gnome-copied-files', 'text/uri-list', 'text/plain;charset=utf-8'],
              ['copy\n' + uris, uris, uris]
            );
        });

        this.show_all();
    }
});

function main() {
    GLib.set_application_name("Clippy");

    const app = new Gtk.Application();

    app.connect('activate', () => {
        const win = new DemoWindow({
          application: app,
          default_width: 400,
          default_height: 300
        });

        win.present();
    });

    return app.run(null);
}
