#include <gio/gio.h>
#include <glib-unix.h>
#include "../hack-sound-client.h"

#define APP_ID "com.endlessm.HackSoundServer2.Client.Examples.Play"
#define SOUND_EVENT_ID "hack-toolbox/lockscreen/active"

HackSoundClient *client;

static gboolean
on_sigint (gpointer user_data)
{
  g_application_quit (G_APPLICATION (user_data));
  return FALSE;
}

static void
on_play (HackSoundClient *client, GAsyncResult *res, gpointer user_data)
{
  g_autoptr(GError) error = NULL;
  gchar *object_path;

  object_path = hack_sound_client_play_finish (client, res, &error);

  if (error) {
    g_printerr ("Cannot get the startup sound UUID: %s", error->message);
    return;
  }
}

static void
on_activate (GApplication *app, gpointer user_data)
{
  HackSoundClient *client;

  g_application_hold (app);

  client = hack_sound_client_new (g_application_get_application_id (app));
  hack_sound_client_play (client,
                          SOUND_EVENT_ID,
                          (GAsyncReadyCallback) on_play,
                          NULL);
}

int
main (int argc, char **argv)
{
  GApplication *app;
  guint status;

  g_spawn_command_line_sync ("pkill -f -9 /app/bin/hack-sound-server",
                             NULL, NULL, NULL, NULL);

  app = g_application_new (APP_ID, G_APPLICATION_FLAGS_NONE);

  g_signal_connect (G_OBJECT (app), "activate", G_CALLBACK (on_activate), NULL);
  g_unix_signal_add(SIGINT, on_sigint, app);

  status = g_application_run (app, argc, argv);
  return status;
}

