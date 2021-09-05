/* configuration.h */

#ifndef GID_CONFIGURATION_H
#define GID_CONFIGURATION_H

#include <stdbool.h>
#include <stddef.h>

#include "gid/gitprofile.h"

#define GID_CONFIGURATION_MAX_NUM_PROFILES 10
#define GID_CONFIGURATION_PARSE_LINEBUF_LEN 1024


typedef struct {
  GidGitProfile git_profiles[GID_CONFIGURATION_MAX_NUM_PROFILES];
  size_t git_profiles_length;
  int active_git_profile;
} GidConfiguration;

GidConfiguration parseFile(char const*const restrict);

int gitProfileExists(
    GidConfiguration const*const restrict,
    char const*const restrict
);


#endif /* GID_CONFIGURATION_H */
