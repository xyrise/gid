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
  int num_params = concatParamStrings(
      profile,
      git_command,
      COMMAND_MAX,
      "-c ",
      "=\"",
      "\" ",
      false
  );
  // TODO: Change to memcpy instead of strcat
  int i;
  for (i = start_git_args; i < argc; ++i) {
    strcat(git_command, "\"");
    strcat(git_command, argv[i]);
    strcat(git_command, "\" ");
  }

  system(git_command);
  return 0;
}
