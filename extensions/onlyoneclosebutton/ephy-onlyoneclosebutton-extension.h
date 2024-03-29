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

#ifndef EPHY_ONLYONECLOSEBUTTON_EXTENSION_H
#define EPHY_ONLYONECLOSEBUTTON_EXTENSION_H

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define EPHY_TYPE_ONLYONECLOSEBUTTON_EXTENSION \
  (ephy_onlyoneclosebutton_extension_get_type ())
#define EPHY_ONLYONECLOSEBUTTON_EXTENSION(o) \
  (G_TYPE_CHECK_INSTANCE_CAST ((o), EPHY_TYPE_ONLYONECLOSEBUTTON_EXTENSION, \
      EphyOnlyoneclosebuttonExtension))
#define EPHY_ONLYONECLOSEBUTTON_EXTENSION_CLASS(k) \
  (G_TYPE_CHECK_CLASS_CAST((k), EPHY_TYPE_ONLYONECLOSEBUTTON_EXTENSION, \
      EphyOnlyoneclosebuttonExtensionClass))
#define EPHY_IS_ONLYONECLOSEBUTTON_EXTENSION(o) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((o), EPHY_TYPE_ONLYONECLOSEBUTTON_EXTENSION))
#define EPHY_IS_ONLYONECLOSEBUTTON_EXTENSION_CLASS(k) \
  (G_TYPE_CHECK_CLASS_TYPE ((k), EPHY_TYPE_ONLYONECLOSEBUTTON_EXTENSION))
#define EPHY_ONLYONECLOSEBUTTON_EXTENSION_GET_CLASS(o) \
  (G_TYPE_INSTANCE_GET_CLASS ((o), EPHY_TYPE_ONLYONECLOSEBUTTON_EXTENSION, \
      EphyOnlyoneclosebuttonExtensionClass))

typedef struct _EphyOnlyoneclosebuttonExtension
  EphyOnlyoneclosebuttonExtension;
typedef struct _EphyOnlyoneclosebuttonExtensionClass
  EphyOnlyoneclosebuttonExtensionClass;
typedef struct _EphyOnlyoneclosebuttonExtensionPrivate
  EphyOnlyoneclosebuttonExtensionPrivate;

struct _EphyOnlyoneclosebuttonExtensionClass
{
  GObjectClass parent_class;
};

struct _EphyOnlyoneclosebuttonExtension
{
  GObject parent_instance;

  /*< private >*/
  EphyOnlyoneclosebuttonExtensionPrivate *priv;
};

GType ephy_onlyoneclosebutton_extension_get_type (void);

GType ephy_onlyoneclosebutton_extension_register_type (GTypeModule *module);

G_END_DECLS

#endif
