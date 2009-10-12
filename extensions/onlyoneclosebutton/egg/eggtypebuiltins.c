
/* Generated data (by glib-mkenums) */

#include <config.h>

#include "eggtypebuiltins.h"


/* enumerations from "eggdesktopfile.h" */
#include "eggdesktopfile.h"
GType
egg_desktop_file_type_get_type (void)
{
  static GType type = 0;

  if (G_UNLIKELY (type == 0))
  {
    static const GEnumValue _egg_desktop_file_type_values[] = {
      { EGG_DESKTOP_FILE_TYPE_UNRECOGNIZED, "EGG_DESKTOP_FILE_TYPE_UNRECOGNIZED", "unrecognized" },
      { EGG_DESKTOP_FILE_TYPE_APPLICATION, "EGG_DESKTOP_FILE_TYPE_APPLICATION", "application" },
      { EGG_DESKTOP_FILE_TYPE_LINK, "EGG_DESKTOP_FILE_TYPE_LINK", "link" },
      { EGG_DESKTOP_FILE_TYPE_DIRECTORY, "EGG_DESKTOP_FILE_TYPE_DIRECTORY", "directory" },
      { 0, NULL, NULL }
    };

    type = g_enum_register_static ("EggDesktopFileType", _egg_desktop_file_type_values);
  }

  return type;
}

GType
egg_desktop_file_launch_option_get_type (void)
{
  static GType type = 0;

  if (G_UNLIKELY (type == 0))
  {
    static const GEnumValue _egg_desktop_file_launch_option_values[] = {
      { EGG_DESKTOP_FILE_LAUNCH_CLEARENV, "EGG_DESKTOP_FILE_LAUNCH_CLEARENV", "clearenv" },
      { EGG_DESKTOP_FILE_LAUNCH_PUTENV, "EGG_DESKTOP_FILE_LAUNCH_PUTENV", "putenv" },
      { EGG_DESKTOP_FILE_LAUNCH_SCREEN, "EGG_DESKTOP_FILE_LAUNCH_SCREEN", "screen" },
      { EGG_DESKTOP_FILE_LAUNCH_WORKSPACE, "EGG_DESKTOP_FILE_LAUNCH_WORKSPACE", "workspace" },
      { EGG_DESKTOP_FILE_LAUNCH_DIRECTORY, "EGG_DESKTOP_FILE_LAUNCH_DIRECTORY", "directory" },
      { EGG_DESKTOP_FILE_LAUNCH_TIME, "EGG_DESKTOP_FILE_LAUNCH_TIME", "time" },
      { EGG_DESKTOP_FILE_LAUNCH_FLAGS, "EGG_DESKTOP_FILE_LAUNCH_FLAGS", "flags" },
      { EGG_DESKTOP_FILE_LAUNCH_SETUP_FUNC, "EGG_DESKTOP_FILE_LAUNCH_SETUP_FUNC", "setup-func" },
      { EGG_DESKTOP_FILE_LAUNCH_RETURN_PID, "EGG_DESKTOP_FILE_LAUNCH_RETURN_PID", "return-pid" },
      { EGG_DESKTOP_FILE_LAUNCH_RETURN_STDIN_PIPE, "EGG_DESKTOP_FILE_LAUNCH_RETURN_STDIN_PIPE", "return-stdin-pipe" },
      { EGG_DESKTOP_FILE_LAUNCH_RETURN_STDOUT_PIPE, "EGG_DESKTOP_FILE_LAUNCH_RETURN_STDOUT_PIPE", "return-stdout-pipe" },
      { EGG_DESKTOP_FILE_LAUNCH_RETURN_STDERR_PIPE, "EGG_DESKTOP_FILE_LAUNCH_RETURN_STDERR_PIPE", "return-stderr-pipe" },
      { EGG_DESKTOP_FILE_LAUNCH_RETURN_STARTUP_ID, "EGG_DESKTOP_FILE_LAUNCH_RETURN_STARTUP_ID", "return-startup-id" },
      { 0, NULL, NULL }
    };

    type = g_enum_register_static ("EggDesktopFileLaunchOption", _egg_desktop_file_launch_option_values);
  }

  return type;
}

GType
egg_desktop_file_error_get_type (void)
{
  static GType type = 0;

  if (G_UNLIKELY (type == 0))
  {
    static const GEnumValue _egg_desktop_file_error_values[] = {
      { EGG_DESKTOP_FILE_ERROR_INVALID, "EGG_DESKTOP_FILE_ERROR_INVALID", "invalid" },
      { EGG_DESKTOP_FILE_ERROR_NOT_LAUNCHABLE, "EGG_DESKTOP_FILE_ERROR_NOT_LAUNCHABLE", "not-launchable" },
      { EGG_DESKTOP_FILE_ERROR_UNRECOGNIZED_OPTION, "EGG_DESKTOP_FILE_ERROR_UNRECOGNIZED_OPTION", "unrecognized-option" },
      { 0, NULL, NULL }
    };

    type = g_enum_register_static ("EggDesktopFileError", _egg_desktop_file_error_values);
  }

  return type;
}


/* enumerations from "eggsmclient.h" */
#include "eggsmclient.h"
GType
egg_sm_client_end_style_get_type (void)
{
  static GType type = 0;

  if (G_UNLIKELY (type == 0))
  {
    static const GEnumValue _egg_sm_client_end_style_values[] = {
      { EGG_SM_CLIENT_END_SESSION_DEFAULT, "EGG_SM_CLIENT_END_SESSION_DEFAULT", "end-session-default" },
      { EGG_SM_CLIENT_LOGOUT, "EGG_SM_CLIENT_LOGOUT", "logout" },
      { EGG_SM_CLIENT_REBOOT, "EGG_SM_CLIENT_REBOOT", "reboot" },
      { EGG_SM_CLIENT_SHUTDOWN, "EGG_SM_CLIENT_SHUTDOWN", "shutdown" },
      { 0, NULL, NULL }
    };

    type = g_enum_register_static ("EggSMClientEndStyle", _egg_sm_client_end_style_values);
  }

  return type;
}

GType
egg_sm_client_mode_get_type (void)
{
  static GType type = 0;

  if (G_UNLIKELY (type == 0))
  {
    static const GEnumValue _egg_sm_client_mode_values[] = {
      { EGG_SM_CLIENT_MODE_DISABLED, "EGG_SM_CLIENT_MODE_DISABLED", "disabled" },
      { EGG_SM_CLIENT_MODE_NO_RESTART, "EGG_SM_CLIENT_MODE_NO_RESTART", "no-restart" },
      { EGG_SM_CLIENT_MODE_NORMAL, "EGG_SM_CLIENT_MODE_NORMAL", "normal" },
      { 0, NULL, NULL }
    };

    type = g_enum_register_static ("EggSMClientMode", _egg_sm_client_mode_values);
  }

  return type;
}


/* enumerations from "egg-toolbars-model.h" */
#include "egg-toolbars-model.h"
GType
egg_tb_model_flags_get_type (void)
{
  static GType type = 0;

  if (G_UNLIKELY (type == 0))
  {
    static const GFlagsValue _egg_tb_model_flags_values[] = {
      { EGG_TB_MODEL_NOT_REMOVABLE, "EGG_TB_MODEL_NOT_REMOVABLE", "not-removable" },
      { EGG_TB_MODEL_NOT_EDITABLE, "EGG_TB_MODEL_NOT_EDITABLE", "not-editable" },
      { EGG_TB_MODEL_BOTH, "EGG_TB_MODEL_BOTH", "both" },
      { EGG_TB_MODEL_BOTH_HORIZ, "EGG_TB_MODEL_BOTH_HORIZ", "both-horiz" },
      { EGG_TB_MODEL_ICONS, "EGG_TB_MODEL_ICONS", "icons" },
      { EGG_TB_MODEL_TEXT, "EGG_TB_MODEL_TEXT", "text" },
      { EGG_TB_MODEL_STYLES_MASK, "EGG_TB_MODEL_STYLES_MASK", "styles-mask" },
      { EGG_TB_MODEL_ACCEPT_ITEMS_ONLY, "EGG_TB_MODEL_ACCEPT_ITEMS_ONLY", "accept-items-only" },
      { EGG_TB_MODEL_HIDDEN, "EGG_TB_MODEL_HIDDEN", "hidden" },
      { 0, NULL, NULL }
    };

    type = g_flags_register_static ("EggTbModelFlags", _egg_tb_model_flags_values);
  }

  return type;
}

GType
egg_tb_model_name_flags_get_type (void)
{
  static GType type = 0;

  if (G_UNLIKELY (type == 0))
  {
    static const GFlagsValue _egg_tb_model_name_flags_values[] = {
      { EGG_TB_MODEL_NAME_USED, "EGG_TB_MODEL_NAME_USED", "used" },
      { EGG_TB_MODEL_NAME_INFINITE, "EGG_TB_MODEL_NAME_INFINITE", "infinite" },
      { EGG_TB_MODEL_NAME_KNOWN, "EGG_TB_MODEL_NAME_KNOWN", "known" },
      { 0, NULL, NULL }
    };

    type = g_flags_register_static ("EggTbModelNameFlags", _egg_tb_model_name_flags_values);
  }

  return type;
}


/* Generated data ends here */

