#pragma once

#include <glib-object.h>
#include "../hack-sound-server2.h"

G_BEGIN_DECLS

#define HACK_TYPE_SOUND_CLIENT (hack_sound_client_get_type())

G_DECLARE_DERIVABLE_TYPE (HackSoundClient, hack_sound_client, HACK, SOUND_CLIENT, GObject)

struct _HackSoundClientClass
{
  GObjectClass parent_class;
  void (* player_proxy_added) (HackSoundClient *self);
  void (* player_proxy_removed) (HackSoundClient *self);
};

HackSoundClient *hack_sound_client_new (const gchar * app_id);

void hack_sound_client_play (HackSoundClient     *self,
                             const gchar         *sound_event_id,
                             GAsyncReadyCallback  callback,
                             gpointer             user_data);
gchar * hack_sound_client_play_finish (HackSoundClient *client,
                                       GAsyncResult    *result,
                                       GError         **error);



G_END_DECLS
