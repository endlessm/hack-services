#include "hack-sound-client.h"

typedef struct
{
  gchar *app_id;
  HackSoundServer2 *server_proxy;
  HackSoundServer2Player *player;
} HackSoundClientPrivate;

typedef struct
{
  GTask *task;
  const gchar *sound_event_id;
  gulong signal_handler_id;
} PlayUserData;

G_DEFINE_TYPE_WITH_PRIVATE (HackSoundClient, hack_sound_client, G_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_APP_ID,
  N_PROPS
};

static GParamSpec *properties [N_PROPS];

enum {
  PLAYER_OBJECT_ADDED,
  PLAYER_OBJECT_REMOVED,
  LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 };


/**
 * hack_sound_client_new:
 *
 * Create a new #HackSoundClient.
 *
 * Returns: (transfer full): a newly created #HackSoundClient
 */
HackSoundClient *
hack_sound_client_new (const gchar * app_id)
{
  return g_object_new (HACK_TYPE_SOUND_CLIENT, "app-id", app_id, NULL);
}

static void
hack_sound_client_finalize (GObject *object)
{
  HackSoundClient *self = (HackSoundClient *)object;
  HackSoundClientPrivate *priv = hack_sound_client_get_instance_private (self);

  g_clear_pointer (&priv->app_id, g_free);
  G_OBJECT_CLASS (hack_sound_client_parent_class)->finalize (object);
}

static void
hack_sound_client_get_property (GObject    *object,
                                guint       prop_id,
                                GValue     *value,
                                GParamSpec *pspec)
{
  HackSoundClient *self = HACK_SOUND_CLIENT (object);
  HackSoundClientPrivate *priv = hack_sound_client_get_instance_private (self);

  switch (prop_id)
    {
    case PROP_APP_ID:
      g_value_set_string (value, priv->app_id);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
hack_sound_client_set_property (GObject      *object,
                                guint         prop_id,
                                const GValue *value,
                                GParamSpec   *pspec)
{
  HackSoundClient *self = HACK_SOUND_CLIENT (object);
  HackSoundClientPrivate *priv = hack_sound_client_get_instance_private (self);

  switch (prop_id)
    {
    case PROP_APP_ID:
      priv->app_id = g_strdup (g_value_get_string (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
hack_sound_client_class_init (HackSoundClientClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = hack_sound_client_finalize;
  object_class->get_property = hack_sound_client_get_property;
  object_class->set_property = hack_sound_client_set_property;

  properties [PROP_APP_ID] =
    g_param_spec_string ("app-id",
                         "app-id",
                         "A well-known d-bus name.",
                         NULL,
                         (G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, PROP_APP_ID,
                                   properties [PROP_APP_ID]);

  signals [PLAYER_OBJECT_ADDED] =
    g_signal_new ("player-object-added",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST,
                  G_STRUCT_OFFSET (HackSoundClientClass, player_proxy_added),
                  NULL,
                  NULL,
                  g_cclosure_marshal_generic,
                  G_TYPE_NONE,
                  0);

  signals [PLAYER_OBJECT_REMOVED] =
    g_signal_new ("player-object-removed",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST,
                  G_STRUCT_OFFSET (HackSoundClientClass, player_proxy_removed),
                  NULL,
                  NULL,
                  g_cclosure_marshal_generic,
                  G_TYPE_NONE,
                  0);
}

static void
hack_sound_client_init (HackSoundClient *self)
{
  HackSoundClientPrivate *priv = hack_sound_client_get_instance_private (self);
  priv->app_id = NULL;

  priv->server_proxy = hack_sound_server2_proxy_new_for_bus_sync (
      G_BUS_TYPE_SESSION,
      G_DBUS_PROXY_FLAGS_NONE,
      "com.endlessm.HackSoundServer2",
      "/com/endlessm/HackSoundServer2",
      NULL, NULL);
}

static void
_player_proxy_name_owner_changed_cb (HackSoundServer2Player *player_object,
                                     GParamSpec             *pspec,
                                     HackSoundClient        *self)
{
  HackSoundClientPrivate *priv = hack_sound_client_get_instance_private (self);

  g_clear_object (&priv->player);
  g_signal_emit (self, signals[PLAYER_OBJECT_REMOVED], 0);
}

static void
_new_player_proxy_cb (HackSoundServer2PlayerProxy *proxy,
                      GAsyncResult                *res,
                      HackSoundClient             *self)
{
  HackSoundClientPrivate *priv = hack_sound_client_get_instance_private (self);

  g_assert (priv->player == NULL);

  /* TODO: Handle error. */
  priv->player =
      hack_sound_server2_player_proxy_new_for_bus_finish (res, NULL);
  g_signal_connect (G_DBUS_PROXY (priv->player), "notify::g-name-owner",
                    G_CALLBACK (_player_proxy_name_owner_changed_cb), self);
  g_signal_emit (self, signals[PLAYER_OBJECT_ADDED], 0);
}

static void
hack_sound_client_create_player_proxy_async (HackSoundClient *self,
                                             const gchar     *object_path)
{
  hack_sound_server2_player_proxy_new_for_bus (
      G_BUS_TYPE_SESSION,
      G_DBUS_PROXY_FLAGS_NONE,
      "com.endlessm.HackSoundServer2",
      object_path,
      NULL,
      (GAsyncReadyCallback) _new_player_proxy_cb,
      self
  );
}

static PlayUserData *
play_user_data_new (GTask       *task,
                    const gchar *sound_event_id)
{
  PlayUserData *data;
  data = g_new (PlayUserData, 1);
  data->task = task;
  data->sound_event_id = sound_event_id;
  return data;
}

static void
play_cb (GObject      *source_object,
         GAsyncResult *res,
         gpointer      data)
{
  GVariant *variant;
  GDBusProxy *proxy = G_DBUS_PROXY (source_object);
  g_autoptr(GTask) task = G_TASK (data);
  GError *error = NULL;

  variant = g_dbus_proxy_call_finish (proxy, res, &error);
  if (error)
    g_task_return_error (task, error);
  else {
    gchar *sound_object_path;
    g_variant_get (variant, "(o)", &sound_object_path);
    g_task_return_pointer (task, sound_object_path, g_free);
  }
}

static void
play_async (HackSoundClient *self, PlayUserData *user_data)
{
  HackSoundClientPrivate *priv = hack_sound_client_get_instance_private (self);

  hack_sound_server2_player_call_play (priv->player,
                                       user_data->sound_event_id,
                                       NULL,
                                       play_cb, user_data->task);
  g_free (user_data);
}

static void
play_on_proxy_added_cb (HackSoundClient *self, PlayUserData *user_data)
{
  HackSoundClientPrivate *priv = hack_sound_client_get_instance_private (self);
  g_assert (priv->player != NULL);
  g_signal_handler_disconnect (self, user_data->signal_handler_id);
  play_async (self, user_data);
}

static void
get_player_and_play_cb (GObject      *source_object,
                        GAsyncResult *res,
                        gpointer      data)
{
  GVariant *variant;
  gchar *player_object_path;
  GDBusProxy *proxy = G_DBUS_PROXY (source_object);
  PlayUserData *user_data = (PlayUserData *) data;
  HackSoundClient *self =
      HACK_SOUND_CLIENT (g_task_get_source_object (user_data->task));
  HackSoundClientPrivate *priv = hack_sound_client_get_instance_private (self);
  GError *error = NULL;

  variant = g_dbus_proxy_call_finish (proxy, res, &error);
  if (error) {
    g_task_return_error (user_data->task, error);
    return;
  }

  g_variant_get (variant, "(o)", &player_object_path);

  if (priv->player == NULL) {
    user_data->signal_handler_id =
        g_signal_connect (self, "player-object-added",
                          G_CALLBACK (play_on_proxy_added_cb), user_data);
    hack_sound_client_create_player_proxy_async (self, player_object_path);
  } else
    play_async (self, user_data);
}

gchar *
hack_sound_client_play_finish (HackSoundClient *client,
                               GAsyncResult    *result,
                               GError         **error)
{
  g_return_val_if_fail (g_task_is_valid (result, client), NULL);

  return g_task_propagate_pointer (G_TASK (result), error);
}

void
hack_sound_client_play (HackSoundClient     *self,
                        const gchar         *sound_event_id,
                        GAsyncReadyCallback  callback,
                        gpointer             user_data)
{
  HackSoundClientPrivate *priv = hack_sound_client_get_instance_private (self);
  GTask *task;
	GVariantBuilder builder;
  GVariant *options;
  PlayUserData *data;

  task = g_task_new (self, NULL, callback, user_data);
  data = play_user_data_new (task, sound_event_id);

	g_variant_builder_init (&builder, G_VARIANT_TYPE ("a{sv}"));
  options = g_variant_new ("a{sv}", &builder);
  hack_sound_server2_call_get_player (priv->server_proxy,
                                      priv->app_id, options,
                                      NULL, get_player_and_play_cb, data);
}
