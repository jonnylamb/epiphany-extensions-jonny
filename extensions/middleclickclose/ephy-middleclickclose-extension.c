/*
 *  Copyright © 2009 Jonny Lamb <jonny@debian.org>
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

#include "ephy-middleclickclose-extension.h"

#include <epiphany/epiphany.h>

#include <gmodule.h>

#define HANDLER_ID \
  (g_quark_from_static_string ("middleclickclose_handler_id"))

static GObjectClass *parent_class = NULL;

static GType type = 0;

static void
ephy_middleclickclose_extension_init (
    EphyMiddleclickcloseExtension *extension)
{
}

/* The following three functions were stolen from ephy-notebook.c */

/* FIXME remove when gtknotebook's func for this becomes public, bug #.... */
static EphyNotebook *
find_notebook_at_pointer (gint abs_x, gint abs_y)
{
  GdkWindow *win_at_pointer, *toplevel_win;
  gpointer toplevel = NULL;
  gint x, y;

  /* FIXME multi-head */
  win_at_pointer = gdk_window_at_pointer (&x, &y);
  if (win_at_pointer == NULL)
    {
      /* We are outside all windows containing a notebook */
      return NULL;
    }

  toplevel_win = gdk_window_get_toplevel (win_at_pointer);

  /* get the GtkWidget which owns the toplevel GdkWindow */
  gdk_window_get_user_data (toplevel_win, &toplevel);

  /* toplevel should be an EphyWindow */
  if (toplevel != NULL && EPHY_IS_WINDOW (toplevel))
    {
      return EPHY_NOTEBOOK (ephy_window_get_notebook
          (EPHY_WINDOW (toplevel)));
    }

  return NULL;
}

static gboolean
is_in_notebook_window (EphyNotebook *notebook,
    gint abs_x,
    gint abs_y)
{
  EphyNotebook *nb_at_pointer;

  nb_at_pointer = find_notebook_at_pointer (abs_x, abs_y);

  return nb_at_pointer == notebook;
}

static gint
find_tab_num_at_pos (EphyNotebook *notebook,
    gint abs_x,
    gint abs_y)
{
  GtkPositionType tab_pos;
  int page_num = 0;
  GtkNotebook *nb = GTK_NOTEBOOK (notebook);
  GtkWidget *page;

  /* For some reason unfullscreen + quick click can
     cause a wrong click event to be reported to the tab */
  if (!is_in_notebook_window (notebook, abs_x, abs_y))
    {
      return -1;
    }

  tab_pos = gtk_notebook_get_tab_pos (nb);

  while ((page = gtk_notebook_get_nth_page (nb, page_num)))
    {
      GtkWidget *tab;
      GtkAllocation allocation;
      gint max_x, max_y;
      gint x_root, y_root;

      tab = gtk_notebook_get_tab_label (nb, page);
      g_return_val_if_fail (tab != NULL, -1);

      if (!GTK_WIDGET_MAPPED (GTK_WIDGET (tab)))
        {
          page_num++;
          continue;
        }

      gdk_window_get_origin (gtk_widget_get_window (tab),
          &x_root, &y_root);

      gtk_widget_get_allocation (tab, &allocation);
      max_x = x_root + allocation.x + allocation.width;
      max_y = y_root + allocation.y + allocation.height;

      if ((tab_pos == GTK_POS_TOP
              || tab_pos == GTK_POS_BOTTOM)
          && abs_x <= max_x
          && abs_y <= max_y)
        {
          return page_num;
        }
      else if ((tab_pos == GTK_POS_LEFT
              || tab_pos == GTK_POS_RIGHT)
          && abs_x <= max_x
          && abs_y <= max_y)
        {
          return page_num;
        }

      page_num++;
    }
  return -1;
}

static gboolean
button_press_event_cb (GtkWidget *widget,
    GdkEventButton *event,
    EphyWindow *window)
{
  gint tab_number;
  EphyEmbed *embed;

  if (event->button != 2 || event->type != GDK_BUTTON_PRESS)
    return FALSE;

  /* I wish this wasn't so gash and copy-pasty... Connecting
   * to button-press-event on the tab doesn't ever get called.*/
  tab_number = find_tab_num_at_pos (EPHY_NOTEBOOK (widget),
      event->x_root, event->y_root);

  if (tab_number < 0)
    return FALSE;

  embed = EPHY_EMBED (gtk_notebook_get_nth_page (GTK_NOTEBOOK (widget),
          tab_number));

  if (embed == NULL)
    return FALSE;

  ephy_embed_container_remove_child (EPHY_EMBED_CONTAINER (window), embed);

  return TRUE;
}

static void
attach_window (EphyExtension *ext,
    EphyWindow *window)
{
  GtkWidget *notebook;
  gulong handler_id;

  notebook = ephy_window_get_notebook (window);

  handler_id = g_signal_connect (notebook, "button-press-event",
      G_CALLBACK (button_press_event_cb), window);

  g_object_set_qdata (G_OBJECT (notebook), HANDLER_ID,
      GUINT_TO_POINTER (handler_id));
}

static void
detach_window (EphyExtension *ext,
    EphyWindow *window)
{
  GtkWidget *notebook;
  gulong handler_id;
  gpointer data;

  notebook = ephy_window_get_notebook (window);

  data = g_object_get_qdata (G_OBJECT (notebook), HANDLER_ID);

  if (data == NULL)
    return;

  handler_id = GPOINTER_TO_UINT (data);

  g_signal_handler_disconnect (notebook, handler_id);
}

static void
ephy_middleclickclose_extension_iface_init (EphyExtensionIface *iface)
{
  iface->attach_window = attach_window;
  iface->detach_window = detach_window;
}

static void
ephy_middleclickclose_extension_class_init (
    EphyMiddleclickcloseExtensionClass *klass)
{
  parent_class = g_type_class_peek_parent (klass);
}

GType
ephy_middleclickclose_extension_get_type (void)
{
  return type;
}

GType
ephy_middleclickclose_extension_register_type (GTypeModule *module)
{
  const GTypeInfo our_info = {
    sizeof (EphyMiddleclickcloseExtensionClass),
    NULL, /* base_init */
    NULL, /* base_finalize */
    (GClassInitFunc) ephy_middleclickclose_extension_class_init,
    NULL,
    NULL, /* class_data */
    sizeof (EphyMiddleclickcloseExtension),
    0, /* n_preallocs */
    (GInstanceInitFunc) ephy_middleclickclose_extension_init
  };

  const GInterfaceInfo extension_info = {
    (GInterfaceInitFunc) ephy_middleclickclose_extension_iface_init,
    NULL,
    NULL
  };

  type = g_type_module_register_type (module, G_TYPE_OBJECT,
      "EphyMiddleclickcloseExtension", &our_info, 0);

  g_type_module_add_interface (module, type,
      EPHY_TYPE_EXTENSION, &extension_info);

  return type;
}
