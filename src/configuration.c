/* configuration.c */
#include "gid/configuration.h"

#include "gid/path.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
  #define strncasecmp _strnicmp
#endif


bool detectFile(char* dest, size_t dest_size, char const*const exe_path) {
  FILE* configuration_file;
  if (dest_size > GID_CONFIGURATION_PATH_MAX)
    dest_size = GID_CONFIGURATION_PATH_MAX;

  size_t cpy_len = 0;

  // Environment variable configuration path
  char const*const env_path = getenv("GID_CONFIGURATION");
  if (env_path) {
    cpy_len = strnlen(env_path, GID_CONFIGURATION_PATH_MAX - 1);
    if (cpy_len < dest_size) {
      configuration_file = fopen(env_path, "rb");
      if (configuration_file) {
        fclose(configuration_file);
        strncpy(dest, env_path, cpy_len);
        dest[cpy_len] = 0;
        return true;
      }
    }
  }

  char buffer[GID_CONFIGURATION_PATH_MAX];
  // Local configuration path
  cpy_len = dirLength(exe_path) + 17;
  if (cpy_len < GID_CONFIGURATION_PATH_MAX && cpy_len < dest_size) {
    dirName(buffer, exe_path);
    strcat(buffer, "configuration.gid");
    configuration_file = fopen(buffer, "rb");
    if (configuration_file) {
      fclose(configuration_file);
      strncpy(dest, buffer, cpy_len);
      dest[cpy_len] = 0;
      return true;
    }
  }

  // Home `.config` folder configuration path
  char const*const home_path = getenv("HOME");
  if (home_path) {
    cpy_len = strnlen(home_path, GID_CONFIGURATION_PATH_MAX - 31) + 30;
    if (cpy_len < dest_size) {
      strncpy(buffer, home_path, cpy_len - 30);
      buffer[cpy_len - 30] = 0;
      fillTrailingSlash(buffer);
      strcat(buffer, ".config/gid/configuration.gid");
      configuration_file = fopen(buffer, "rb");
      if (configuration_file) {
        fclose(configuration_file);
        strncpy(dest, buffer, cpy_len);
        return true;
      }
    }
  }

  // USERPROFILE as `.config` (only on Windows)
  char const*const up_path = getenv("USERPROFILE");
  if (up_path) {
    cpy_len = strnlen(up_path, GID_CONFIGURATION_PATH_MAX - 31) + 30;
    if (cpy_len < dest_size) {
      strncpy(buffer, up_path, cpy_len - 30);
      buffer[cpy_len - 30] = 0;
      fillTrailingSlash(buffer);
      strcat(buffer, ".config/gid/configuration.gid");
      configuration_file = fopen(buffer, "rb");
      if (configuration_file) {
        fclose(configuration_file);
        strncpy(dest, buffer, cpy_len);
        return true;
      }
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
  for (size_t i = 0; i < GID_CONFIGURATION_MAX_NUM_PROFILES; ++i) {
    result.git_profiles[i].name[0] = 0;
    result.git_profiles[i].user_name[0] = 0;
    result.git_profiles[i].user_email[0] = 0;
    result.git_profiles[i].commit_template[0] = 0;
    result.git_profiles[i].user_signingkey[0] = 0;
    result.git_profiles[i].ssh_key_path[0] = 0;
  }
  char line_buffer[GID_CONFIGURATION_PARSE_LINEBUF_LEN];
  size_t const line_buffer_len = GID_CONFIGURATION_PARSE_LINEBUF_LEN;
  while (fgets(line_buffer, line_buffer_len, file)) {
    bool split = false;
    int key_start = -1;
    int key_end = -1;
    int val_start = -1;
    int val_end = -1;
    int i = 0;
    while (i < line_buffer_len && line_buffer[i] > 0) {
      // Ignore empty spaces in line
      if (isspace(line_buffer[i])) {
        ++i;
        continue;
      }

      if (line_buffer[i] == ':') {
        ++i;
        split = true;
        continue;
      }

      if (!split) {
        if (key_start == -1) key_start = i;
        else key_end = i;
      }
      else {
        if (val_start == -1) val_start = i;
        else val_end = i;
      }

      ++i;
    }
    int key_len = key_end - key_start;
    int val_len = val_end - val_start;

    if (key_len <= 0) continue;
    if (val_len <= 0) continue;
    key_len++;
    val_len++;

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
        if (val_len + 2 >= GID_GITPROFILE_SSH_KEY_PATH_LEN) {
          fprintf(stderr, "parsed \"ssh_key_path\" is too long\n");
          exit(EXIT_FAILURE);
        }
        if (strchr(val, '\'')) {
          strncpy(&current_git_profile->ssh_key_path[0], val, val_len);
          current_git_profile->ssh_key_path[val_len] = 0;
        }
        else {
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
    GidConfiguration const*const gid_configuration,
    char const*const profile_name
) {
  for (size_t i = 0; i < gid_configuration->git_profiles_length; ++i) {
    if (!strcmp(&gid_configuration->git_profiles[i].name[0], profile_name))
      return i;
  }
  return -1;
}
