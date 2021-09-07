/* gidc.c */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "gid/configuration.h"
#include "gid/gitprofile.h"

#define PATH_MAX 512
#define COMMAND_MAX (sizeof(GidGitProfile) - GID_GITPROFILE_NAME_LEN)


int main(int argc, char * argv[]) {
  /* Parse gid configuration */
  char local_config_path[PATH_MAX];
  char global_config_path[PATH_MAX];
  GidConfiguration configuration;

  local_config_path[0] = 0;
  getcwd(local_config_path, PATH_MAX);
  if (strlen(local_config_path) < PATH_MAX - 18) {
    strcat(local_config_path, "/configuration.gid");
  }

  global_config_path[0] = 0;
  char const*const home_path = getenv("HOME");
  if (home_path) {
    strcpy(global_config_path, home_path);
  }
  if (strlen(global_config_path) < PATH_MAX - 18) {
    strcat(global_config_path, "/.config/gid/configuration.gid");
  }

  if (!access(local_config_path, F_OK)) {
    configuration = parseFile(local_config_path);
  }
  else if (!access(global_config_path, F_OK)) {
    configuration = parseFile(global_config_path);
  }
  else {
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
