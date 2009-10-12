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

#include "ephy-onlyoneclosebutton-extension.h"
#include <egg/egg-toolbars-model.h>

#include <epiphany/epiphany.h>

#include <gmodule.h>

#define EPHY_ONLYONECLOSEBUTTON_EXTENSION_GET_PRIVATE(object) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((object), \
      EPHY_TYPE_ONLYONECLOSEBUTTON_EXTENSION, \
      EphyOnlyoneclosebuttonExtensionPrivate))

struct _EphyOnlyoneclosebuttonExtensionPrivate
{
  GHashTable *win_data;
  GHashTable *tab_datas;
};

static GObjectClass *parent_class = NULL;

static GType type = 0;

#define TAB_POPUP_XML \
  "<ui>" \
  "<popup name=\"EphyNotebookPopup\" action=\"NotebookPopupAction\">" \
  "<separator/>" \
  "<menuitem name=\"TabCloseENP\" action=\"FileCloseTab\"/>" \
  "<separator/>" \
  "</popup>" \
  "</ui>"

static void
ephy_onlyoneclosebutton_extension_init (
    EphyOnlyoneclosebuttonExtension *extension)
{
  EphyShell *shell;
  EggToolbarsModel *model;
  gint name_flags;

  extension->priv = EPHY_ONLYONECLOSEBUTTON_EXTENSION_GET_PRIVATE (extension);

  extension->priv->win_data = g_hash_table_new_full (NULL, NULL, NULL,
      (GDestroyNotify) g_hash_table_destroy);
  extension->priv->tab_datas = g_hash_table_new_full (NULL, NULL, NULL,
      (GDestroyNotify) g_hash_table_destroy);

  shell = ephy_shell_get_default ();
  model = EGG_TOOLBARS_MODEL (ephy_shell_get_toolbars_model (shell, FALSE));

  name_flags = egg_toolbars_model_get_name_flags (model, "FileCloseTab");
  egg_toolbars_model_set_name_flags (model, "FileCloseTab",
      name_flags | EGG_TB_MODEL_NAME_KNOWN);
}

static void
finalize (GObject *object)
{
  EphyOnlyoneclosebuttonExtension *extension =
    EPHY_ONLYONECLOSEBUTTON_EXTENSION (object);
  EphyShell *shell;
  EggToolbarsModel *model;
  gint name_flags;

  g_hash_table_destroy (extension->priv->win_data);
  extension->priv->win_data = NULL;

  g_hash_table_destroy (extension->priv->tab_datas);
  extension->priv->tab_datas = NULL;

  shell = ephy_shell_get_default ();
  model = EGG_TOOLBARS_MODEL (ephy_shell_get_toolbars_model (shell, FALSE));

  name_flags = egg_toolbars_model_get_name_flags (model, "FileCloseTab");
  egg_toolbars_model_set_name_flags (model, "FileCloseTab",
      name_flags & ~EGG_TB_MODEL_NAME_KNOWN);
}

static void
attach_window (EphyExtension *ext,
    EphyWindow *window)
{
  EphyOnlyoneclosebuttonExtension *extension =
    EPHY_ONLYONECLOSEBUTTON_EXTENSION (ext);
  GHashTable *data, *tab_data;
  GtkUIManager *ui_manager;
  guint merge_id;
  GtkWidget *notebook;
  gboolean initially_homogeneous;
  GError *error = NULL;

  data = g_hash_table_new_full (g_str_hash, g_str_equal, NULL, NULL);

  tab_data = g_hash_table_new_full (NULL, NULL, NULL,
      (GDestroyNotify) g_hash_table_destroy);

  g_hash_table_insert (extension->priv->win_data, window, data);
  g_hash_table_insert (extension->priv->tab_datas, window, tab_data);

  ui_manager = GTK_UI_MANAGER (ephy_window_get_ui_manager (window));

  merge_id = gtk_ui_manager_add_ui_from_string (ui_manager, TAB_POPUP_XML,
      -1, &error);

  if (error != NULL)
    {
      g_error ("%s", error->message);
      g_error_free (error);
    }

  g_hash_table_insert (data, "ui_merge_id", GUINT_TO_POINTER (merge_id));

  notebook = ephy_window_get_notebook (window);

  g_object_get (notebook, "homogeneous", &initially_homogeneous, NULL);

  g_hash_table_insert (data, "initially_homogeneous",
      GUINT_TO_POINTER (initially_homogeneous));

  g_object_set (notebook, "homogeneous", TRUE, NULL);
}

static void
detach_window (EphyExtension *ext,
    EphyWindow *window)
{
  EphyOnlyoneclosebuttonExtension *extension =
    EPHY_ONLYONECLOSEBUTTON_EXTENSION (ext);
  GHashTable *data;
  GtkUIManager *ui_manager;
  guint merge_id;
  gboolean homogeneous;
  GtkWidget *notebook;

  data = g_hash_table_lookup (extension->priv->win_data, window);

  ui_manager = GTK_UI_MANAGER (ephy_window_get_ui_manager (window));

  merge_id = GPOINTER_TO_UINT (g_hash_table_lookup (data, "ui_merge_id"));

  gtk_ui_manager_remove_ui (ui_manager, merge_id);
  gtk_ui_manager_ensure_update (ui_manager);

  notebook = ephy_window_get_notebook (window);

  homogeneous = GPOINTER_TO_UINT (g_hash_table_lookup (data,
          "initially_homogeneous"));

  g_object_set (notebook, "homogeneous", homogeneous, NULL);

  g_hash_table_remove (extension->priv->win_data, window);
  g_hash_table_remove (extension->priv->tab_datas, window);
}

typedef struct
{
  EphyOnlyoneclosebuttonExtension *extension;
  EphyWindow *window;
  EphyEmbed *embed;
} StyleSetData;

static void
hbox_style_set_cb (GtkWidget *hbox,
    GtkStyle *previous_style,
    StyleSetData *user_data)
{
  GHashTable *data, *tab_data, *single_tab_data;
  gint width;

  data = g_hash_table_lookup (user_data->extension->priv->win_data,
      user_data->window);
  tab_data = g_hash_table_lookup (user_data->extension->priv->tab_datas,
      user_data->window);
  single_tab_data = g_hash_table_lookup (tab_data, user_data->embed);

  gtk_widget_get_size_request (hbox, &width, NULL);

  g_hash_table_insert (single_tab_data, "tab_initial_width",
      GINT_TO_POINTER (width));

  gtk_widget_set_size_request (hbox, 16, -1);
}

static void
attach_tab (EphyExtension *ext,
    EphyWindow *window,
    EphyEmbed *embed)
{
  EphyOnlyoneclosebuttonExtension *extension =
    EPHY_ONLYONECLOSEBUTTON_EXTENSION (ext);
  GHashTable *data, *tab_data, *single_tab_data;
  GtkWidget *notebook, *widget, *last_child;
  GList *children;
  gint width;
  gulong handler_id;
  StyleSetData *user_data;

  data = g_hash_table_lookup (extension->priv->win_data, window);
  tab_data = g_hash_table_lookup (extension->priv->tab_datas, window);

  single_tab_data = g_hash_table_new (g_str_hash, g_str_equal);

  g_hash_table_insert (tab_data, embed, single_tab_data);

  notebook = ephy_window_get_notebook (window);
  widget = gtk_notebook_get_tab_label (GTK_NOTEBOOK (notebook),
      GTK_WIDGET (embed));

  if (!GTK_IS_HBOX (widget))
    return;

  children = gtk_container_get_children (GTK_CONTAINER (widget));

  gtk_widget_get_size_request (widget, &width, NULL);

  g_hash_table_insert (single_tab_data, "tab_initial_width",
      GINT_TO_POINTER (width));

  gtk_widget_set_size_request (widget, 16, -1);

  user_data = g_slice_new0 (StyleSetData);
  user_data->extension = extension;
  user_data->window = window;
  user_data->embed = embed;

  /* FIXME: LEAK */
  handler_id = g_signal_connect (widget, "style-set",
      G_CALLBACK (hbox_style_set_cb), user_data);

  g_hash_table_insert (single_tab_data, "hbox_style_set_hid",
      GUINT_TO_POINTER (handler_id));

  last_child = g_list_last (children)->data;

  if (GTK_IS_BUTTON (last_child))
    gtk_widget_hide (last_child);

  gtk_container_child_set (GTK_CONTAINER (notebook), GTK_WIDGET (embed),
      "tab-expand", TRUE, NULL);

  g_list_free (children);
}

static void
detach_tab (EphyExtension *ext,
    EphyWindow *window,
    EphyEmbed *embed)
{
  EphyOnlyoneclosebuttonExtension *extension =
    EPHY_ONLYONECLOSEBUTTON_EXTENSION (ext);
  GHashTable *data, *tab_data, *single_tab_data;
  GtkWidget *notebook, *widget, *last_child;
  GList *children;
  gulong handler_id;
  gint width;

  data = g_hash_table_lookup (extension->priv->win_data, window);
  tab_data = g_hash_table_lookup (extension->priv->tab_datas, window);
  single_tab_data = g_hash_table_lookup (tab_data, embed);

  notebook = ephy_window_get_notebook (window);
  widget = gtk_notebook_get_tab_label (GTK_NOTEBOOK (notebook),
      GTK_WIDGET (embed));

  if (gtk_widget_get_parent (GTK_WIDGET (embed)) == notebook)
    {
      gtk_container_child_set (GTK_CONTAINER (notebook), GTK_WIDGET (embed),
          "tab-expand", FALSE, NULL);
    }

  if (!GTK_IS_HBOX (widget))
    goto out;

  children = gtk_container_get_children (GTK_CONTAINER (widget));

  handler_id = GPOINTER_TO_UINT (g_hash_table_lookup (single_tab_data,
          "hbox_style_set_hid"));

  g_signal_handler_disconnect (widget, handler_id);

  width = GPOINTER_TO_INT (g_hash_table_lookup (single_tab_data,
          "tab_initial_width"));

  gtk_widget_set_size_request (widget, width, -1);

  last_child = g_list_last (children)->data;

  if (GTK_IS_BUTTON (last_child))
    gtk_widget_show (last_child);

  g_list_free (children);

out:
  g_hash_table_remove (tab_data, embed);
}

static void
ephy_onlyoneclosebutton_extension_iface_init (EphyExtensionIface *iface)
{
  iface->attach_window = attach_window;
  iface->detach_window = detach_window;
  iface->attach_tab = attach_tab;
  iface->detach_tab = detach_tab;
}

static void
ephy_onlyoneclosebutton_extension_class_init (
    EphyOnlyoneclosebuttonExtensionClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);

  object_class->finalize = finalize;

  g_type_class_add_private (object_class,
      sizeof (EphyOnlyoneclosebuttonExtensionPrivate));
}

GType
ephy_onlyoneclosebutton_extension_get_type (void)
{
  return type;
}

GType
ephy_onlyoneclosebutton_extension_register_type (GTypeModule *module)
{
  const GTypeInfo our_info = {
    sizeof (EphyOnlyoneclosebuttonExtensionClass),
    NULL, /* base_init */
    NULL, /* base_finalize */
    (GClassInitFunc) ephy_onlyoneclosebutton_extension_class_init,
    NULL,
    NULL, /* class_data */
    sizeof (EphyOnlyoneclosebuttonExtension),
    0, /* n_preallocs */
    (GInstanceInitFunc) ephy_onlyoneclosebutton_extension_init
  };

  const GInterfaceInfo extension_info = {
    (GInterfaceInitFunc) ephy_onlyoneclosebutton_extension_iface_init,
    NULL,
    NULL
  };

  type = g_type_module_register_type (module, G_TYPE_OBJECT,
      "EphyOnlyoneclosebuttonExtension", &our_info, 0);

  g_type_module_add_interface (module, type,
      EPHY_TYPE_EXTENSION, &extension_info);

  return type;
}
