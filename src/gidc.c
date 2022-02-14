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

  char git_command_base[COMMAND_MAX];
  strcpy(git_command_base, "git config --");
  char git_command[COMMAND_MAX];

  int active_git_profile = -1;

  if (argc > 1) {
    if (!strcmp(argv[1], "set")) {
      if (argc == 4) {
        strcat(git_command_base, argv[2]);  // Add "global" or "local"
        strcat(git_command_base, " ");
        active_git_profile = gitProfileExists(&configuration, argv[3]);
        profile = &configuration.git_profiles[active_git_profile];
        int num_params = concatParamStrings(
            profile,
            git_command,
            COMMAND_MAX,
            git_command_base,
            " \"",
            "\"",
            true
        );
      }
      else {
        fprintf(
            stderr,
            "set must be accompanied by global/local and a profile name\n"
        );
        exit(EXIT_FAILURE);
      }
    }
    else {
      fprintf(
          stderr,
          "`set` is currently the only supported command\n"
      );
      exit(EXIT_FAILURE);
    }
  }
  else {
    fprintf(
        stderr,
        "`gidc` must be followed by a command\n"
    );
    exit(EXIT_FAILURE);
  }
  return 0;
}
