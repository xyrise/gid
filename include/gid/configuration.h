/* configuration.h */
#ifndef GID_CONFIGURATION_H
#define GID_CONFIGURATION_H

#include "gid/gitprofile.h"

#include <stdbool.h>
#include <stddef.h>


#define GID_CONFIGURATION_MAX_NUM_PROFILES 10
#define GID_CONFIGURATION_PARSE_LINEBUF_LEN 1024
#define GID_CONFIGURATION_PATH_MAX 512

typedef struct {
  GidGitProfile git_profiles[GID_CONFIGURATION_MAX_NUM_PROFILES];
  int git_profiles_length;
  int active_git_profile;
} GidConfiguration;

bool detectFile(char *const, size_t, char const *const);
GidConfiguration parseFile(char const *const);

int gitProfileExists(
    GidConfiguration const *const,
    char const *const
);



#endif /* GID_CONFIGURATION_H */
