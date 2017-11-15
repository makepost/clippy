/* clippy.c
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

#include "clippy.h"

typedef struct _SetTargetsContents SetTargetsContents;

struct _SetTargetsContents {
  gint n;

  GtkTargetEntry *targets;

  gchar **texts;
};

static void
targets_get_func (GtkClipboard     *clipboard,
                  GtkSelectionData *selection_data,
                  guint             info,
                  gpointer          data)
{
  gint i;
  SetTargetsContents *contents = data;
  GdkAtom target = gtk_selection_data_get_target (selection_data);

  for (i = 0; i < contents->n; i++) {
    if (target == gdk_atom_intern_static_string (contents->targets[i].target)) {
      gtk_selection_data_set_text (selection_data, contents->texts[i], strlen (contents->texts[i]));
      break;
    }
  }
}

static void
targets_clear_func (GtkClipboard *clipboard,
                    gpointer      data)
{
  SetTargetsContents *contents = data;

  gtk_target_table_free (contents->targets, contents->n);
  g_strfreev (contents->texts);
  g_free (contents);
}

/**
 * clippy_set_targets:
 * @clipboard: a #GtkClipboard object
 * @targets:   (array length=n_targets): array containing information
 *     about the available forms for the clipboard data
 * @n_targets: number of elements in @targets
 * @texts:     (array length=n_texts): UTF-8 strings
 * @n_texts:   number of elements in @targets, must be equal to n_targets
 *
 * Sets the contents of the clipboard to the given UTF-8 strings, each
 * corresponding to a given data type.
 **/
void
clippy_set_targets (GtkClipboard  *clipboard,
                    gchar        **targets,
                    gint           n_targets,
                    gchar        **texts,
                    gint           n_texts)
{
  gint i;
  GtkTargetList *target_list;

  g_return_if_fail (GTK_IS_CLIPBOARD (clipboard));
  g_return_if_fail (n_targets == n_texts);

  SetTargetsContents *contents = g_new (SetTargetsContents, 1);
  contents->n = 0;

  contents->texts = g_strdupv (texts);
  target_list = gtk_target_list_new (NULL, 0);

  for (i = 0; i < n_targets; i++) {
    gtk_target_list_add (target_list,
                         gdk_atom_intern_static_string (targets[i]), 0, 0);
  }

  contents->targets = gtk_target_table_new_from_list (target_list,
                                                      &contents->n);

  gtk_clipboard_set_with_data (clipboard,
                               contents->targets, n_targets,
                               targets_get_func, targets_clear_func,
                               contents);
  gtk_clipboard_set_can_store (clipboard, NULL, 0);

  gtk_target_list_unref (target_list);
}

/**
 * clippy_request_target:
 * @clipboard: a #GtkClipboard
 * @target:    information about the form into which the clipboard
 *     owner should convert the selection
 * @callback:  (scope async): A function to call when the results are received
 *     (or the retrieval fails). If the retrieval fails the length field of
 *     @selection_data will be negative.
 * @user_data: user data to pass to @callback
 *
 * Requests the contents of clipboard as the given target.
 * When the results of the result are later received the supplied callback
 * will be called.
 **/
void
clippy_request_target (GtkClipboard             *clipboard,
                       const gchar              *target,
                       GtkClipboardReceivedFunc  callback,
                       gpointer                  user_data)
{
  g_return_if_fail (GTK_IS_CLIPBOARD (clipboard));
  g_return_if_fail (callback != NULL);

  gtk_clipboard_request_contents (clipboard,
                                  gdk_atom_intern_static_string (target),
                                  callback, user_data);
}

/**
 * clippy_get:
 * @selection: a string which identifies the clipboard to use
 *
 * Returns the clipboard object for the given selection.
 * See gtk_clipboard_get_for_display() for complete details.
 *
 * Returns: (transfer none): the appropriate clipboard object. If no clipboard
 *     already exists, a new one will be created. Once a clipboard
 *     object has been created, it is persistent and, since it is
 *     owned by GTK+, must not be freed or unreffed.
 */
GtkClipboard *
clippy_get (const gchar *selection)
{
  return gtk_clipboard_get (gdk_atom_intern_static_string (selection));
}
