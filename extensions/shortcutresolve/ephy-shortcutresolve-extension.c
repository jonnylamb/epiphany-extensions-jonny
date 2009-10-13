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

#include <string.h>

#include "ephy-shortcutresolve-extension.h"

#include <epiphany/epiphany.h>

#include <gmodule.h>

#define EPHY_SHORTCUTRESOLVE_EXTENSION_GET_PRIVATE(object) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((object), \
      EPHY_TYPE_SHORTCUTRESOLVE_EXTENSION, \
      EphyShortcutresolveExtensionPrivate))

struct _EphyShortcutresolveExtensionPrivate
{
  GHashTable *shortcuts;
  gchar *filename;
  GFileMonitor *monitor;
  gulong resolve_address_id;
  gulong changed_id;
};

static GObjectClass *parent_class = NULL;

static GType type = 0;

typedef struct {
  gchar *prefix;
  gchar *url;
  gchar *pattern;
  gchar *replacement;
} ShortcutData;

static gchar *
resolve_cb (EphyBookmarks *eb,
    const gchar *original_address,
    const gchar *content,
    gpointer user_data)
{
  EphyShortcutresolveExtension *extension = user_data;
  gchar *address, *tmp, *one, *two;
  gchar *out = NULL;
  ShortcutData *data;

  address = g_strdup (original_address);

  g_strstrip (address);

  tmp = strchr (address, ' ');

  if (tmp == NULL)
    return NULL;

  *tmp = '\0';
  one = address;
  two = tmp + 1;

  data = g_hash_table_lookup (extension->priv->shortcuts, one);

  if (data != NULL)
    {
      if (data->pattern != NULL)
        {
          GRegex *regex;
          gchar *append;
          const gchar *replacement;

          if (data->replacement == NULL)
            replacement = "";
          else
            replacement = data->replacement;

          regex = g_regex_new (data->pattern, 0, 0, NULL);
          append = g_regex_replace (regex, two, -1, 0,
              data->replacement, 0, NULL);

          out = g_strdup_printf (data->url, append);
          g_free (append);
          g_regex_unref (regex);
        }
      else
        {
          out = g_strdup_printf (data->url, two);
        }
    }

  g_free (address);

  return out;
}

static gboolean
load_shortcuts (EphyShortcutresolveExtension *extension)
{
  GKeyFile *keyfile;
  gboolean out = TRUE;
  gchar **groups, **g;

  g_hash_table_remove_all (extension->priv->shortcuts);

  keyfile = g_key_file_new ();

  if (!g_key_file_load_from_file (keyfile, extension->priv->filename,
          G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS, NULL))
    {
      g_warning ("Failed to load keyfile: %s", extension->priv->filename);
      out = FALSE;
    }

  groups = g_key_file_get_groups (keyfile, NULL);

  for (g = groups; *g != NULL; g++)
    {
      ShortcutData *data = g_slice_new0 (ShortcutData);
      gchar *prefix;

      prefix = g_key_file_get_string (keyfile, *g, "prefix", NULL);

      data->url = g_key_file_get_string (keyfile, *g, "url", NULL);
      data->pattern = g_key_file_get_string (keyfile, *g, "pattern", NULL);
      data->replacement = g_key_file_get_string (keyfile, *g, "replacement",
          NULL);

      g_hash_table_insert (extension->priv->shortcuts, prefix, data);
      /* prefix gets freed by the hash table */
    }

  g_strfreev (groups);
  g_key_file_free (keyfile);

  return out;
}

static void
shortcut_data_free (gpointer data)
{
  ShortcutData *d = data;

  g_free (d->url);
  g_free (d->pattern);
  g_free (d->replacement);

  g_slice_free (ShortcutData, d);
}

static void
changed_cb (GFileMonitor *monitor,
    GFile *file,
    GFile *other_file,
    GFileMonitorEvent event_type,
    gpointer user_data)
{
  EphyShortcutresolveExtension *extension =
    EPHY_SHORTCUTRESOLVE_EXTENSION (user_data);

  if (event_type != G_FILE_MONITOR_EVENT_CHANGES_DONE_HINT)
    return;

  load_shortcuts (extension);
}

static void
ephy_shortcutresolve_extension_init (
    EphyShortcutresolveExtension *extension)
{
  EphyShell *shell;
  EphyBookmarks *bookmarks;
  GFile *file;

  extension->priv = EPHY_SHORTCUTRESOLVE_EXTENSION_GET_PRIVATE (extension);
  extension->priv->shortcuts = g_hash_table_new_full (g_str_hash, g_str_equal,
      g_free, shortcut_data_free);

  extension->priv->resolve_address_id = 0;
  extension->priv->changed_id = 0;

  extension->priv->filename = g_build_filename (g_get_user_config_dir (),
      "epiphany-shortcutresolve", "shortcuts", NULL);

  file = g_file_new_for_path (extension->priv->filename);
  extension->priv->monitor = g_file_monitor_file (file, G_FILE_MONITOR_NONE,
      NULL, NULL);
  g_signal_connect (extension->priv->monitor, "changed",
      G_CALLBACK (changed_cb), extension);
  g_object_unref (file);

  if (!load_shortcuts (extension))
    return;

  shell = ephy_shell_get_default ();

  bookmarks = ephy_shell_get_bookmarks (shell);

  extension->priv->resolve_address_id = g_signal_connect (bookmarks,
      "resolve-address", G_CALLBACK (resolve_cb), extension);
}

static void
finalize (GObject *object)
{
  EphyShortcutresolveExtension *extension =
    EPHY_SHORTCUTRESOLVE_EXTENSION (object);
  EphyShell *shell;
  EphyBookmarks *bookmarks;

  shell = ephy_shell_get_default ();
  bookmarks = ephy_shell_get_bookmarks (shell);

  g_signal_handler_disconnect (bookmarks, extension->priv->resolve_address_id);
  extension->priv->resolve_address_id = 0;

  g_free (extension->priv->filename);
  extension->priv->filename = NULL;

  g_hash_table_destroy (extension->priv->shortcuts);
  extension->priv->shortcuts = NULL;

  g_signal_handler_disconnect (extension->priv->monitor,
      extension->priv->changed_id);
  extension->priv->changed_id = 0;

  g_object_unref (extension->priv->monitor);
  extension->priv->monitor = NULL;
}

static void
ephy_shortcutresolve_extension_iface_init (EphyExtensionIface *iface)
{
}

static void
ephy_shortcutresolve_extension_class_init (
    EphyShortcutresolveExtensionClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);

  object_class->finalize = finalize;

  g_type_class_add_private (object_class,
      sizeof (EphyShortcutresolveExtensionPrivate));
}

GType
ephy_shortcutresolve_extension_get_type (void)
{
  return type;
}

GType
ephy_shortcutresolve_extension_register_type (GTypeModule *module)
{
  const GTypeInfo our_info = {
    sizeof (EphyShortcutresolveExtensionClass),
    NULL, /* base_init */
    NULL, /* base_finalize */
    (GClassInitFunc) ephy_shortcutresolve_extension_class_init,
    NULL,
    NULL, /* class_data */
    sizeof (EphyShortcutresolveExtension),
    0, /* n_preallocs */
    (GInstanceInitFunc) ephy_shortcutresolve_extension_init
  };

  const GInterfaceInfo extension_info = {
    (GInterfaceInitFunc) ephy_shortcutresolve_extension_iface_init,
    NULL,
    NULL
  };

  type = g_type_module_register_type (module, G_TYPE_OBJECT,
      "EphyShortcutresolveExtension", &our_info, 0);

  g_type_module_add_interface (module, type,
      EPHY_TYPE_EXTENSION, &extension_info);

  return type;
}
