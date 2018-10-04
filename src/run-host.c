#include <glib-unix.h>
#include <gio/gio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#include "host-command.h"

static void
on_host_command_exited (int      pid,
                        int      exit_status,
                        gpointer data)
{
  pegg_restore_stdin ();

  if (WIFEXITED (exit_status))
    exit (WEXITSTATUS (exit_status));
  else if (WIFSIGNALED (exit_status))
    exit (128 + WTERMSIG (exit_status));
  else
    exit (255);
}

typedef struct
{
  GDBusConnection *connection;
  int pid;
  int signum;
  gboolean to_process_group;
} WatchSignalData;


static gboolean
on_watched_signal (gpointer user_data)
{
  WatchSignalData *data = user_data;
  GError *error = NULL;

  if (!pegg_send_host_command_signal (data->connection,
                                      data->pid, data->signum,
                                      data->to_process_group,
                                      NULL, &error))
    {
      g_printerr ("Failed to send signal to child process: %s\n", error->message);
      g_clear_error (&error);
    }

  return G_SOURCE_CONTINUE;
}

static void
watch_signal (GDBusConnection *connection,
              int              pid,
              int              signum,
              gboolean         to_process_group)
{
  WatchSignalData *data = g_new0 (WatchSignalData, 1);
  data->connection = connection;
  data->pid = pid;
  data->signum = signum;
  data->to_process_group = to_process_group;

  g_unix_signal_add (signum, on_watched_signal, data);
}

int
main(int argc, char **argv)
{
  GError *error = NULL;
  PeggHostCommandFlags flags = PEGG_HOST_COMMAND_NONE;

  if (!pegg_in_flatpak ())
    {
      g_printerr ("Not inside a flatpak\n");
      return 1;
    }

  GDBusConnection *connection = g_bus_get_sync (G_BUS_TYPE_SESSION, NULL, &error);
  if (!connection)
    {
      g_printerr ("Could not get connection to bus\n");
      return 1;
    }

  GPtrArray *environment_array = g_ptr_array_new_full (0, g_free);

  int first_arg = 1;
  while (first_arg < argc)
    {
      if (strcmp (argv[first_arg], "--pty") == 0)
        {
          first_arg++;
          flags |= PEGG_HOST_COMMAND_USE_PTY;
        }
      else if (strcmp (argv[first_arg], "--setenv") == 0)
        {
          if (first_arg + 2 >= argc)
            {
              g_printerr ("--setenv requires two arguments\n");
              return 1;
            }

          g_ptr_array_add (environment_array, g_strdup (argv[first_arg + 1]));
          g_ptr_array_add (environment_array, g_strdup (argv[first_arg + 2]));
          first_arg += 3;
        }
      else
        break;
    }

  GPtrArray *arg_array = g_ptr_array_new();
  for (int i = first_arg; i < argc; i++)
    g_ptr_array_add (arg_array, g_strdup (argv[i]));
  g_ptr_array_add (arg_array, NULL);
  char **args = (char **)g_ptr_array_free (arg_array, FALSE);

  g_ptr_array_add (environment_array, NULL);
  char **extra_environment = (char **)g_ptr_array_free (environment_array, FALSE);

  int pid = pegg_call_host_command (connection, args, extra_environment,
                                    flags,
                                    on_host_command_exited, NULL, NULL, &error);
  if (pid == -1)
    {
      g_printerr ("Could not call command: %s\n", error->message);
      return 1;
  }

  if (flags & PEGG_HOST_COMMAND_USE_PTY)
    pegg_make_stdin_raw ();

  watch_signal (connection, pid, SIGHUP, TRUE);
  watch_signal (connection, pid, SIGINT, TRUE);
  watch_signal (connection, pid, SIGTERM, FALSE);

  GMainLoop *loop = g_main_loop_new (NULL, FALSE);
  g_main_loop_run (loop);

  return 0;
}
