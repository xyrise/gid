/* gid.c */

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
  if (strlen(profile->user_signingkey)) {
    strcat(git_command, "-c user.signingkey=\"");
    strcat(git_command, profile->user_signingkey);
    strcat(git_command, "\" ");
  }
  if (strlen(profile->commit_gpgsign)) {
    strcat(git_command, "-c commit.gpgsign=\"");
    strcat(git_command, profile->commit_gpgsign);
    strcat(git_command, "\" ");
  }
  if (strlen(profile->commit_template)) {
    strcat(git_command, "-c commit.template=\"");
    strcat(git_command, profile->commit_template);
    strcat(git_command, "\" ");
  }
  if (strlen(profile->pull_rebase)) {
    strcat(git_command, "-c pull.rebase=\"");
    strcat(git_command, profile->pull_rebase);
    strcat(git_command, "\" ");
  }
  if (strlen(profile->tag_gpgsign)) {
    strcat(git_command, "-c tag.gpgsign=\"");
    strcat(git_command, profile->tag_gpgsign);
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
