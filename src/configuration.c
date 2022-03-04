/* configuration.c */
#include "gid/configuration.h"

#include "gid/path.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef _WIN32
  #define HOME_ENV "USERPROFILE"
#else
  #define HOME_ENV "HOME"
#endif

int strncasecmp(
    char const *const s1,
    char const *const s2,
    size_t const strsz
) {
  unsigned char const *us1 = (unsigned char const *)s1;
  unsigned char const *us2 = (unsigned char const *)s2;
  for (size_t i = 0; i < strsz && tolower(*us1) == tolower(*us2++); ++i) {
    if (*us1++ == 0 || i + 1 == strsz) return 0;
  }
  return tolower(*us1) - tolower(*--us2);
}

bool detectFile(char *const dest, size_t destsz, char const *const exe_path) {
  FILE *configuration_file;
  if (destsz > GID_CONFIGURATION_PATH_MAX)
    destsz = GID_CONFIGURATION_PATH_MAX;

  size_t cpy_len = 0;

  // Environment variable configuration path
  char const *const env_path = getenv("GID_CONFIGURATION");
  if (
      env_path &&
      (cpy_len = strlen(env_path)) < destsz &&
      (configuration_file = fopen(env_path, "rb"))
  ) {
      fclose(configuration_file);
      strcpy(dest, env_path);
      return true;
  }

  char buffer[GID_CONFIGURATION_PATH_MAX];
  // Local configuration path
  if ((cpy_len = dirLength(exe_path, destsz) + 17) < destsz) {
    dirName(buffer, destsz, exe_path);
    strcat(buffer, "configuration.gid");
    if ((configuration_file = fopen(buffer, "rb"))) {
      fclose(configuration_file);
      strcpy(dest, buffer);
      dest[cpy_len] = 0;
      return true;
    }
  }

  // `.config` folder configuration path at HOME, or USERPROFILE on Windows
  char const*const home_path = getenv(HOME_ENV);
  if (home_path && (cpy_len = strlen(home_path) + 30) < destsz) {
    strcpy(buffer, home_path);
    fillTrailingSlash(buffer, destsz, buffer);
    strcat(buffer, ".config/gid/configuration.gid");
    if ((configuration_file = fopen(buffer, "rb"))) {
      fclose(configuration_file);
      strcpy(dest, buffer);
      return true;
    }
  }

  return false;
}

GidConfiguration parseFile(char const*const file_name) {
  FILE* file = fopen(file_name, "r");
  if (!file) {
    fprintf(stderr, "error opening ");
    fprintf(stderr, "%s", file_name);
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
  }

  GidConfiguration result;
  result.git_profiles_length = 0;
  result.active_git_profile = -1;
  for (int i = 0; i < GID_CONFIGURATION_MAX_NUM_PROFILES; ++i) {
    result.git_profiles[i].name[0] = 0;
    result.git_profiles[i].user_name[0] = 0;
    result.git_profiles[i].user_email[0] = 0;
    result.git_profiles[i].commit_template[0] = 0;
    result.git_profiles[i].user_signingkey[0] = 0;
    result.git_profiles[i].ssh_key_path[0] = 0;
  }
  char line_buffer[GID_CONFIGURATION_PARSE_LINEBUF_LEN];
  int const line_buffer_len = GID_CONFIGURATION_PARSE_LINEBUF_LEN;
  while (fgets(line_buffer, line_buffer_len, file)) {
    bool split = false;
    bool started = false;
    size_t key_start = 0, key_end = 0, val_start = 0, val_end = 0;
    for (int i = 0; i < line_buffer_len && line_buffer[i]; ++i) {
      // Ignore empty spaces in line
      if (isspace(line_buffer[i])) continue;

      if (line_buffer[i] == ':') {
        split = true;
        started = false;
        continue;
      }

      if (!split) {
        if (!started && (started = true)) key_start = i;
        else key_end = i;
      }
      else {
        if (!started && (started = true)) val_start = i;
        else val_end = i;
      }
    }
    if (key_end <= key_start || val_end <= val_start) continue;
    size_t key_len = key_end - key_start + 1;
    size_t val_len = val_end - val_start + 1;
    char const*const key = &line_buffer[key_start];
    char const*const val = &line_buffer[val_start];

    if (key_len == 5 && !strncasecmp(key, "start", 5)) {
      if (val_len >= GID_GITPROFILE_NAME_LEN) {
        fprintf(stderr, "parsed Git profile name is too long\n");
        exit(EXIT_FAILURE);
      }
      strncpy(
          &result.git_profiles[result.git_profiles_length].name[0],
          val,
          val_len
      );
      result.git_profiles[result.git_profiles_length].name[val_len] = 0;
      ++result.git_profiles_length;
    }
    else if (key_len == 7 && !strncasecmp(key, "default", 7)) {
      result.active_git_profile = result.git_profiles_length;
      if (val_len >= GID_GITPROFILE_NAME_LEN) {
        fprintf(stderr, "parsed Git profile name is too long\n");
        exit(EXIT_FAILURE);
      }
      strncpy(
          &result.git_profiles[result.git_profiles_length].name[0],
          val,
          val_len
      );
      result.git_profiles[result.git_profiles_length].name[val_len] = 0;
      ++result.git_profiles_length;
    }
    else if (result.git_profiles_length > 0) {
      GidGitProfile* current_git_profile =
        &result.git_profiles[result.git_profiles_length - 1];
      if (key_len == 9 && !strncasecmp(key, "user.name", 9)) {
        if (val_len >= GID_GITPROFILE_USER_NAME_LEN) {
          fprintf(stderr, "parsed \"user.name\" is too long\n");
          exit(EXIT_FAILURE);
        }
        strncpy(&current_git_profile->user_name[0], val, val_len);
        current_git_profile->user_name[val_len] = 0;
      }
      else if (key_len == 10 && !strncasecmp(key, "user.email", 10)) {
        if (val_len >= GID_GITPROFILE_USER_EMAIL_LEN) {
          fprintf(stderr, "parsed \"user.email\" is too long\n");
          exit(EXIT_FAILURE);
        }
        strncpy(&current_git_profile->user_email[0], val, val_len);
        current_git_profile->user_email[val_len] = 0;
      }
      else if (key_len == 15 && !strncasecmp(key, "user.signingkey", 15)) {
        if (val_len >= GID_GITPROFILE_USER_SIGNINGKEY_LEN) {
          fprintf(stderr, "parsed \"user.signingkey\" is too long\n");
          exit(EXIT_FAILURE);
        }
        strncpy(&current_git_profile->user_signingkey[0], val, val_len);
        current_git_profile->user_signingkey[val_len] = 0;
      }
      else if (key_len == 14 && !strncasecmp(key, "commit.gpgsign", 14)) {
        if (val_len >= GID_GITPROFILE_COMMIT_GPGSIGN_LEN) {
          fprintf(stderr, "parsed \"commit.gpgsign\" is too long\n");
          exit(EXIT_FAILURE);
        }
        strncpy(&current_git_profile->commit_gpgsign[0], val, val_len);
        current_git_profile->commit_gpgsign[val_len] = 0;
      }
      else if (key_len == 15 && !strncasecmp(key, "commit.template", 15)) {
        if (val_len >= GID_GITPROFILE_COMMIT_TEMPLATE_LEN) {
          fprintf(stderr, "parsed \"commit.template\" is too long\n");
          exit(EXIT_FAILURE);
        }
        strncpy(&current_git_profile->commit_template[0], val, val_len);
        current_git_profile->commit_template[val_len] = 0;
      }
      else if (key_len == 11 && !strncasecmp(key, "pull.rebase", 11)) {
        if (val_len >= GID_GITPROFILE_PULL_REBASE_LEN) {
          fprintf(stderr, "parsed \"pull.rebase\" is too long\n");
          exit(EXIT_FAILURE);
        }
        strncpy(&current_git_profile->pull_rebase[0], val, val_len);
        current_git_profile->pull_rebase[val_len] = 0;
      }
      else if (key_len == 11 && !strncasecmp(key, "tag.gpgsign", 11)) {
        if (val_len >= GID_GITPROFILE_TAG_GPGSIGN_LEN) {
          fprintf(stderr, "parsed \"tag.gpgsign\" is too long\n");
          exit(EXIT_FAILURE);
        }
        strncpy(&current_git_profile->tag_gpgsign[0], val, val_len);
        current_git_profile->tag_gpgsign[val_len] = 0;
      }
      else if (key_len == 12 && !strncasecmp(key, "ssh_key_path", 12)) {
        if (strchr(val, '\'')) {
          if (val_len >= GID_GITPROFILE_SSH_KEY_PATH_LEN) {
            fprintf(stderr, "parsed \"ssh_key_path\" is too long\n");
            exit(EXIT_FAILURE);
          }
          strncpy(&current_git_profile->ssh_key_path[0], val, val_len);
          current_git_profile->ssh_key_path[val_len] = 0;
        }
        else {
          if (val_len + 2 >= GID_GITPROFILE_SSH_KEY_PATH_LEN) {
            fprintf(stderr, "parsed \"ssh_key_path\" is too long\n");
            exit(EXIT_FAILURE);
          }
          current_git_profile->ssh_key_path[0] = '\'';
          strncpy(&current_git_profile->ssh_key_path[1], val, val_len);
          current_git_profile->ssh_key_path[val_len + 1] = '\'';
          current_git_profile->ssh_key_path[val_len + 2] = 0;
        }
      }
    }
  }
  return result;
}

int gitProfileExists(
    GidConfiguration const *const gid_configuration,
    char const *const profile_name
) {
  for (int i = 0; i < gid_configuration->git_profiles_length; ++i) {
    if (!strcmp(&gid_configuration->git_profiles[i].name[0], profile_name))
      return i;
  }
  return -1;
}
