/*
 * This program helps to check that a sound can be played even when the
 * hack-sound-server has been closed.
 */

#include <gio/gio.h>
#include <glib-unix.h>
#include "../hack-sound-client.h"

#define APP_ID "com.endlessm.HackSoundServer2.Client.Examples.Play"
#define SOUND_EVENT_ID "clubhouse/dialog/open"
#define MAX_PLAYBACKS 5

static int n_playbacks = 0;

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

  g_print ("object-path: %s\n", object_path);
}

static gboolean
kill_server (gpointer user_data)
{
  g_spawn_command_line_sync ("pkill -f -9 /app/bin/hack-sound-server",
                             NULL, NULL, NULL, NULL);
  return FALSE;
}

static gboolean
play (HackSoundClient *client)
{
  if (n_playbacks++ == MAX_PLAYBACKS)
    return FALSE;

  hack_sound_client_play (client,
                          SOUND_EVENT_ID,
                          (GAsyncReadyCallback) on_play,
                          NULL);
  g_timeout_add_seconds (3, (GSourceFunc) kill_server, NULL);

  return TRUE;
}

static void
on_activate (GApplication *app, gpointer user_data)
{
  HackSoundClient *client;

  g_application_hold (app);

  client = hack_sound_client_new (g_application_get_application_id (app));

  g_timeout_add_seconds (10, (GSourceFunc) play, client);
}

int
main (int argc, char **argv)
{
  GApplication *app;
  guint status;

  app = g_application_new (APP_ID, G_APPLICATION_FLAGS_NONE);

  g_signal_connect (G_OBJECT (app), "activate", G_CALLBACK (on_activate), NULL);
  g_unix_signal_add(SIGINT, on_sigint, app);

  status = g_application_run (app, argc, argv);
  return status;
}

