/* gidc.c */

#include "gid/configuration.h"
#include "gid/gitprofile.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define COMMAND_MAX (sizeof(GidGitProfile) - GID_GITPROFILE_NAME_LEN)

int main(int argc, char * argv[]) {
  // Parse gid configuration
  char config_path[GID_CONFIGURATION_PATH_MAX];
  GidConfiguration configuration;

  if (!detectFile(config_path, GID_CONFIGURATION_PATH_MAX, argv[0])) {
    fprintf(stderr, "no configuration file found\n");
    exit(EXIT_FAILURE);
  }
  configuration = parseFile(config_path);

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
        if (strlen(profile->user_signingkey)) {
          strcpy(git_command_copy, git_command);
          strcat(git_command_copy, "user.signingkey \"");
          strcat(git_command_copy, profile->user_signingkey);
          strcat(git_command_copy, "\" ");
          system(git_command_copy);
        }
        if (strlen(profile->commit_gpgsign)) {
          strcpy(git_command_copy, git_command);
          strcat(git_command_copy, "commit.gpgsign \"");
          strcat(git_command_copy, profile->commit_gpgsign);
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
        if (strlen(profile->pull_rebase)) {
          strcpy(git_command_copy, git_command);
          strcat(git_command_copy, "pull.rebase \"");
          strcat(git_command_copy, profile->pull_rebase);
          strcat(git_command_copy, "\" ");
          system(git_command_copy);
        }
        if (strlen(profile->tag_gpgsign)) {
          strcpy(git_command_copy, git_command);
          strcat(git_command_copy, "tag_gpgsign \"");
          strcat(git_command_copy, profile->tag_gpgsign);
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
    else {
      fprintf(
          stderr,
          "`set` is currently the only supported command"
      );
      exit(EXIT_FAILURE);
    }
  }
  else {
    fprintf(
        stderr,
        "`gidc` must be followed by a command"
    );
    exit(EXIT_FAILURE);
  }
  return 0;
}
