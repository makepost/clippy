/* clippy-board.c
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

#include "clippy-board.h"

typedef struct _ClippyBoardPrivate ClippyBoardPrivate;

struct _ClippyBoard {
    GObject parent_instance;

    ClippyBoardPrivate *priv;
};

struct _ClippyBoardPrivate {
  GtkClipboard *clipboard;

  gint n;

  GtkTargetEntry *targets;

  gchar **texts;
};

enum
{
  PROP_0,

  PROP_CLIPBOARD,

  LAST_PROP
};

static GParamSpec *props[LAST_PROP];

G_DEFINE_TYPE_WITH_PRIVATE (ClippyBoard, clippy_board, G_TYPE_OBJECT)


static void
targets_get_func (GtkClipboard     *clipboard,
                  GtkSelectionData *selection_data,
                  guint             info,
                  gpointer          data)
{
  gint i;
  ClippyBoardPrivate *priv = CLIPPY_BOARD (data)->priv;
  GdkAtom target = gtk_selection_data_get_target (selection_data);

  for (i = 0; i < priv->n; i++) {
    if (target == gdk_atom_intern_static_string (priv->targets[i].target)) {
      gtk_selection_data_set_text (selection_data, priv->texts[i], strlen (priv->texts[i]));
      break;
    }
  }
}

static void
targets_clear_func (GtkClipboard *clipboard,
                    gpointer      data)
{
  ClippyBoardPrivate *priv = CLIPPY_BOARD (data)->priv;

  gtk_target_table_free (priv->targets, priv->n);
  g_strfreev(priv->texts);
  priv->n = 0;
}

/**
 * clippy_board_set_targets:
 * @board:     a #ClippyBoard object
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
clippy_board_set_targets (ClippyBoard  *board,
                          gchar       **targets,
                          gint          n_targets,
                          gchar       **texts,
                          gint          n_texts)
{
  gint i;
  GtkTargetList *target_list;

  g_return_if_fail (CLIPPY_IS_BOARD (board));
  g_return_if_fail (n_targets == n_texts);

  ClippyBoardPrivate *priv = board->priv;

  targets_clear_func(priv->clipboard, board);

  priv->texts = g_strdupv (texts);
  target_list = gtk_target_list_new (NULL, 0);

  for (i = 0; i < n_targets; i++) {
    gtk_target_list_add (target_list,
                         gdk_atom_intern_static_string (targets[i]), 0, 0);
  }

  priv->targets = gtk_target_table_new_from_list (target_list,
                                                  &priv->n);

  gtk_clipboard_set_with_data (priv->clipboard,
                               priv->targets, n_targets,
                               targets_get_func, NULL,
                               board);
  gtk_clipboard_set_can_store (priv->clipboard, NULL, 0);

  gtk_target_list_unref (target_list);
}

/**
 * clippy_board_request_target:
 * @board: a #ClippyBoard
 * @target:   information about the form into which the clipboard
 *     owner should convert the selection
 * @callback: (scope async): A function to call when the results are received
 *     (or the retrieval fails). If the retrieval fails the length field of
 *     @selection_data will be negative.
 * @user_data: user data to pass to @callback
 *
 * Requests the contents of clipboard as the given target.
 * When the results of the result are later received the supplied callback
 * will be called.
 **/
void
clippy_board_request_target (ClippyBoard              *board,
                             const gchar              *target,
                             GtkClipboardReceivedFunc  callback,
                             gpointer                  user_data)
{
  ClippyBoardPrivate *priv;

  g_return_if_fail (board != NULL);
  g_return_if_fail (callback != NULL);

  priv = CLIPPY_BOARD (board)->priv;
  gtk_clipboard_request_contents (priv->clipboard, gdk_atom_intern_static_string (target),
                                  callback, user_data);
}

static void
clippy_board_set_clipboard (ClippyBoard *board,
                            const gchar *clipboard)
{
  ClippyBoardPrivate *priv;

  g_return_if_fail (CLIPPY_IS_BOARD (board));

  priv = board->priv;

  if (g_set_object (&priv->clipboard, gtk_clipboard_get (gdk_atom_intern_static_string (clipboard))))
    g_object_notify_by_pspec (G_OBJECT (board), props[PROP_CLIPBOARD]);
}

static void
clippy_board_get_property (GObject    *object,
                           guint       prop_id,
                           GValue     *value,
                           GParamSpec *pspec)
{
  switch (prop_id)
    {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
clippy_board_set_property (GObject      *object,
                           guint         prop_id,
                           const GValue *value,
                           GParamSpec   *pspec)
{
  ClippyBoard *board;

  g_return_if_fail (CLIPPY_IS_BOARD (object));
  g_return_if_fail (G_IS_OBJECT (object));

  board = CLIPPY_BOARD (object);

  switch (prop_id)
    {
    case PROP_CLIPBOARD:
      clippy_board_set_clipboard (board, g_value_get_string (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
clippy_board_dispose (GObject *object)
{
  ClippyBoardPrivate *priv = CLIPPY_BOARD (object)->priv;

  targets_clear_func(priv->clipboard, object);
  g_clear_object (&priv->clipboard);
  G_OBJECT_CLASS (clippy_board_parent_class)->dispose (object);
}

static void
clippy_board_finalize (GObject *object)
{
  ClippyBoardPrivate *priv = CLIPPY_BOARD (object)->priv;

  targets_clear_func(priv->clipboard, object);
  G_OBJECT_CLASS (clippy_board_parent_class)->finalize (object);
}

static void
clippy_board_class_init (ClippyBoardClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->get_property = clippy_board_get_property;
  object_class->set_property = clippy_board_set_property;
  object_class->dispose = clippy_board_dispose;
  object_class->finalize = clippy_board_finalize;

  props[PROP_CLIPBOARD] =
    g_param_spec_string ("clipboard",
                         "Clipboard",
                         "Gtk clipboard name",
                         NULL,
                         G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY);

  g_object_class_install_properties (object_class, LAST_PROP, props);
}

static void
clippy_board_init (ClippyBoard *board)
{
  board->priv = clippy_board_get_instance_private (board);
}
