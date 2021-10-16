/* gid.c */

/* Standard Library */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* POSIX */
#include <libgen.h>

#include "gid/configuration.h"
#include "gid/gitprofile.h"

#define PATH_MAX 512
#define COMMAND_MAX (sizeof(GidGitProfile) - GID_GITPROFILE_NAME_LEN)


int main(int argc, char * argv[]) {
  // Parse gid configuration
  char config_path[PATH_MAX];
  GidConfiguration configuration;

  config_path[0] = 0;
  // Environment variable configuration path
  if (strlen(config_path) == 0) {
    char const*const env_path = getenv("GID_CONFIGURATION");
    if (env_path && strlen(env_path) < PATH_MAX) {
      strcpy(config_path, env_path);
    }
    if (!access(config_path, F_OK)) {
      configuration = parseFile(config_path);
    }
    else config_path[0] = 0;
  }

  // Local configuration path
  if (strlen(config_path) == 0) {
    strcat(config_path, dirname(argv[0]));
    if (strlen(config_path) < PATH_MAX - 18) {
      strcat(config_path, "/configuration.gid");
      if (!access(config_path, F_OK)) {
        configuration = parseFile(config_path);
      }
      else config_path[0] = 0;
    }
  }

  // Home `.config` folder configuration path
  if (strlen(config_path) == 0) {
    char const*const home_path = getenv("HOME");
    if (home_path) {
      strcpy(config_path, home_path);
    }
    if (strlen(config_path) < PATH_MAX - 18) {
      strcat(config_path, "/.config/gid/configuration.gid");
    }
    if (!access(config_path, F_OK)) {
      configuration = parseFile(config_path);
    }
    else config_path[0] = 0;
  }

  if (strlen(config_path) == 0) {
    fprintf(stderr, "no configuration file found\n");
    exit(EXIT_FAILURE);
  }

  char git_command[COMMAND_MAX];
  strcpy(git_command, "git ");

  int start_git_args = 1;
  if (argc > 1) {
    // Check for profile name
    int active_git_profile = gitProfileExists(&configuration, argv[1]);
    if (active_git_profile >= 0) {
      configuration.active_git_profile = active_git_profile;
      start_git_args = 2;
    }
  }

  // Append arguments to internal Git call
  GidGitProfile const* profile =
    &configuration.git_profiles[configuration.active_git_profile];
  if (strlen(profile->user_name)) {
    strcat(git_command, "-c user.name=\"");
    strcat(git_command, profile->user_name);
    strcat(git_command, "\" ");
  }
  if (strlen(profile->user_email)) {
    strcat(git_command, "-c user.email=\"");
    strcat(git_command, profile->user_email);
    strcat(git_command, "\" ");
  }
  if (strlen(profile->commit_template)) {
    strcat(git_command, "-c commit.template=\"");
    strcat(git_command, profile->commit_template);
    strcat(git_command, "\" ");
  }
  if (strlen(profile->user_signingkey)) {
    strcat(git_command, "-c user.signingkey=\"");
    strcat(git_command, profile->user_signingkey);
    strcat(git_command, "\" ");
  }
  if (strlen(profile->ssh_key_path)) {
    strcat(git_command, "-c core.sshCommand=\"ssh -i ");
    strcat(git_command, profile->ssh_key_path);
    strcat(git_command, "\" ");
  }
  for (int i = start_git_args; i < argc; ++i) {
    strcat(git_command, "\"");
    strcat(git_command, argv[i]);
    strcat(git_command, "\" ");
  }

  system(git_command);
  return 0;
}
