/*
 *  Copyright © 2009 Jonny Lamb <jonny@debian.org>
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

#include "ephy-tabkeynavigate-extension.h"

#include <gdk/gdkkeysyms.h>
#include <epiphany/epiphany.h>
#include <gmodule.h>

#define KEY_PRESS_EVENT_QUARK \
  (g_quark_from_static_string ("key-press-event-quark-tabkeynavigate"))

static GObjectClass *parent_class = NULL;

static GType type = 0;

static void
ephy_tabkeynavigate_extension_init (
    EphyTabkeynavigateExtension *extension)
{
}

static void
set_tab_offset (EphyWindow *window,
    gint offset)
{
  GtkNotebook *notebook;
  gint count, new_index, index;

  notebook = GTK_NOTEBOOK (ephy_window_get_notebook (window));

  index = gtk_notebook_get_current_page (notebook);
  count = gtk_notebook_get_n_pages (notebook);

  new_index = (index + offset) % count;
  gtk_notebook_set_current_page (notebook, new_index);
}

static gboolean
key_press_event_cb (GtkWidget *widget,
    GdkEventKey *event,
    gpointer user_data)
{
  EphyWindow *window = EPHY_WINDOW (widget);
  gboolean out = FALSE;

  if (event->state & GDK_CONTROL_MASK)
    {
      if (event->keyval == GDK_Tab)
        {
          set_tab_offset (window, 1);
          out = TRUE;
        }
      else if (event->keyval == GDK_ISO_Left_Tab)
        {
          set_tab_offset (window, -1);
          out = TRUE;
        }
    }

  return out;
}

static void
attach_window (EphyExtension *ext,
    EphyWindow *window)
{
  gulong id;

  id = g_signal_connect (window, "key-press-event",
      G_CALLBACK (key_press_event_cb), NULL);
  g_object_set_qdata (G_OBJECT (window), KEY_PRESS_EVENT_QUARK,
      GUINT_TO_POINTER (id));
}

static void
detach_window (EphyExtension *ext,
    EphyWindow *window)
{
  gulong id;
  gpointer data;

  data = g_object_get_qdata (G_OBJECT (window), KEY_PRESS_EVENT_QUARK);

  if (data == NULL)
    return;

  id = GPOINTER_TO_UINT (data);

  g_signal_handler_disconnect (window, id);
}

static void
ephy_tabkeynavigate_extension_iface_init (EphyExtensionIface *iface)
{
  iface->attach_window = attach_window;
  iface->detach_window = detach_window;
}

static void
ephy_tabkeynavigate_extension_class_init (
    EphyTabkeynavigateExtensionClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);
}

GType
ephy_tabkeynavigate_extension_get_type (void)
{
  return type;
}

GType
ephy_tabkeynavigate_extension_register_type (GTypeModule *module)
{
  const GTypeInfo our_info = {
    sizeof (EphyTabkeynavigateExtensionClass),
    NULL, /* base_init */
    NULL, /* base_finalize */
    (GClassInitFunc) ephy_tabkeynavigate_extension_class_init,
    NULL,
    NULL, /* class_data */
    sizeof (EphyTabkeynavigateExtension),
    0, /* n_preallocs */
    (GInstanceInitFunc) ephy_tabkeynavigate_extension_init
  };

  const GInterfaceInfo extension_info = {
    (GInterfaceInitFunc) ephy_tabkeynavigate_extension_iface_init,
    NULL,
    NULL
  };

  type = g_type_module_register_type (module, G_TYPE_OBJECT,
      "EphyTabkeynavigateExtension", &our_info, 0);

  g_type_module_add_interface (module, type,
      EPHY_TYPE_EXTENSION, &extension_info);

  return type;
}
