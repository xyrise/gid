/* gidc.c */

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

  GidGitProfile const* profile;

  char git_command[COMMAND_MAX];
  strcpy(git_command, "git config --");
  char git_command_copy[COMMAND_MAX];
  strcpy(git_command_copy, git_command);

  int active_git_profile = -1;

  if (argc > 1) {
    if (!strcmp(argv[1], "set")) {
      if (argc == 4) {
        strcat(git_command, argv[2]);  // Add "global" or "local"
        strcat(git_command, " ");
        active_git_profile = gitProfileExists(&configuration, argv[3]);
        profile = &configuration.git_profiles[active_git_profile];
        if (strlen(profile->user_name)) {
          strcpy(git_command_copy, git_command);
          strcat(git_command_copy, "user.name \"");
          strcat(git_command_copy, profile->user_name);
          strcat(git_command_copy, "\" ");
          system(git_command_copy);
        }
        if (strlen(profile->user_email)) {
          strcpy(git_command_copy, git_command);
          strcat(git_command_copy, "user.email \"");
          strcat(git_command_copy, profile->user_email);
          strcat(git_command_copy, "\" ");
          system(git_command_copy);
        }
        if (strlen(profile->commit_template)) {
          strcpy(git_command_copy, git_command);
          strcat(git_command_copy, "commit.template \"");
          strcat(git_command_copy, profile->commit_template);
          strcat(git_command_copy, "\" ");
          system(git_command_copy);
        }
        if (strlen(profile->user_signingkey)) {
          strcpy(git_command_copy, git_command);
          strcat(git_command_copy, "user.signingkey \"");
          strcat(git_command_copy, profile->user_signingkey);
          strcat(git_command_copy, "\" ");
          system(git_command_copy);
        }
        if (strlen(profile->ssh_key_path)) {
          strcpy(git_command_copy, git_command);
          strcat(git_command_copy, "core.sshCommand \"ssh -i ");
          strcat(git_command_copy, profile->ssh_key_path);
          strcat(git_command_copy, "\" ");
          system(git_command_copy);
        }
      }
      else {
        fprintf(
            stderr,
            "set must be accompanied by global/local and a profile name"
        );
        exit(EXIT_FAILURE);
      }
    }
  }
  return 0;
}
