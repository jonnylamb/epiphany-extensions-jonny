/*
 *  Copyright © 2009 Jonny Lamb
 *
 *  based on the Python extension by Stefan Stuhr:
 *    Copyright (C) 2006 Stefan Stuhr
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "config.h"

#include "ephy-confirmwindowclose-extension.h"

#include <epiphany/epiphany.h>

#include <gmodule.h>

#define DELETE_EVENT_SIGNAL \
  (g_quark_from_static_string ("confirmwindowclose_delete_event"))

static GObjectClass *parent_class = NULL;

static GType type = 0;

static void
ephy_confirmwindowclose_extension_init (
    EphyConfirmwindowcloseExtension *extension)
{
}

static gboolean
delete_event_cb (GtkWindow *win,
    GdkEvent *event,
    gpointer user_data)
{
  EphyWindow *window = EPHY_WINDOW (win);
  GtkWidget *notebook, *dialog;
  gint tab_count, result;

  notebook = ephy_window_get_notebook (window);

  tab_count = gtk_notebook_get_n_pages (GTK_NOTEBOOK (notebook));

  if (tab_count <= 1)
    return FALSE;

  dialog = gtk_message_dialog_new (win,
      GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
      GTK_MESSAGE_WARNING, GTK_BUTTONS_NONE,
      "You are about to close a window with %i open tabs", tab_count);

  gtk_dialog_add_button (GTK_DIALOG (dialog), "Cancel", GTK_RESPONSE_CANCEL);
  gtk_dialog_add_button (GTK_DIALOG (dialog), "Close", GTK_RESPONSE_OK);
  gtk_dialog_set_default_response (GTK_DIALOG (dialog), GTK_RESPONSE_CANCEL);

  result = gtk_dialog_run (GTK_DIALOG (dialog));

  gtk_widget_destroy (dialog);

  return (result != GTK_RESPONSE_OK);
}

static void
attach_window (EphyExtension *ext,
    EphyWindow *window)
{
  gulong delete_event;

  delete_event = g_signal_connect (window, "delete-event",
      G_CALLBACK (delete_event_cb), NULL);

  g_object_set_qdata (G_OBJECT (window), DELETE_EVENT_SIGNAL,
      GUINT_TO_POINTER (delete_event));
}

static void
detach_window (EphyExtension *ext,
    EphyWindow *window)
{
  gulong delete_event;
  gpointer data;

  data = g_object_get_qdata (G_OBJECT (window), DELETE_EVENT_SIGNAL);

  if (data == NULL)
    return;

  delete_event = GPOINTER_TO_UINT (data);
  g_signal_handler_disconnect (window, delete_event);
}

static void
ephy_confirmwindowclose_extension_iface_init (EphyExtensionIface *iface)
{
  iface->attach_window = attach_window;
  iface->detach_window = detach_window;
}

static void
ephy_confirmwindowclose_extension_class_init (
    EphyConfirmwindowcloseExtensionClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);
}

GType
ephy_confirmwindowclose_extension_get_type (void)
{
  return type;
}

GType
ephy_confirmwindowclose_extension_register_type (GTypeModule *module)
{
  const GTypeInfo our_info = {
    sizeof (EphyConfirmwindowcloseExtensionClass),
    NULL, /* base_init */
    NULL, /* base_finalize */
    (GClassInitFunc) ephy_confirmwindowclose_extension_class_init,
    NULL,
    NULL, /* class_data */
    sizeof (EphyConfirmwindowcloseExtension),
    0, /* n_preallocs */
    (GInstanceInitFunc) ephy_confirmwindowclose_extension_init
  };

  const GInterfaceInfo extension_info = {
    (GInterfaceInitFunc) ephy_confirmwindowclose_extension_iface_init,
    NULL,
    NULL
  };

  type = g_type_module_register_type (module, G_TYPE_OBJECT,
      "EphyConfirmwindowcloseExtension", &our_info, 0);

  g_type_module_add_interface (module, type,
      EPHY_TYPE_EXTENSION, &extension_info);

  return type;
}
