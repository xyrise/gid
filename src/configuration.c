/* configuration.c */

#include "gid/configuration.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


GidConfiguration parseFile(char const*const restrict file_name) {
  FILE* file = fopen(file_name, "r");
  if (!file) {
    fprintf(stderr, "error opening ");
    fprintf(stderr, file_name);
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
  size_t const line_buffer_len = GID_CONFIGURATION_PARSE_LINEBUF_LEN;
  char line_buffer[line_buffer_len];
  while (fgets(line_buffer, line_buffer_len, file)) {
    bool split = false;
    int key_start = -1;
    int key_end = -1;
    int val_start = -1;
    int val_end = -1;
    int i = 0;
    while (i < line_buffer_len && line_buffer[i] != '\n') {
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

    if (key_len == 5 && !strncmp(key, "start", 5)) {
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
    else if (key_len == 7 && !strncmp(key, "default", 7)) {
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
      if (key_len == 9 && !strncmp(key, "user.name", 9)) {
        if (val_len >= GID_GITPROFILE_USER_NAME_LEN) {
          fprintf(stderr, "parsed \"user.name\" is too long\n");
          exit(EXIT_FAILURE);
        }
        strncpy(&current_git_profile->user_name[0], val, val_len);
        current_git_profile->user_name[val_len] = 0;
      }
      else if (key_len == 10 && !strncmp(key, "user.email", 10)) {
        if (val_len >= GID_GITPROFILE_USER_EMAIL_LEN) {
          fprintf(stderr, "parsed \"user.email\" is too long\n");
          exit(EXIT_FAILURE);
        }
        strncpy(&current_git_profile->user_email[0], val, val_len);
        current_git_profile->user_email[val_len] = 0;
      }
      else if (key_len == 15 && !strncmp(key, "commit.template", 15)) {
        if (val_len >= GID_GITPROFILE_COMMIT_TEMPLATE_LEN) {
          fprintf(stderr, "parsed \"commit.template\" is too long\n");
          exit(EXIT_FAILURE);
        }
        strncpy(&current_git_profile->commit_template[0], val, val_len);
        current_git_profile->commit_template[val_len] = 0;
      }
      else if (key_len == 15 && !strncmp(key, "user.signingkey", 15)) {
        if (val_len >= GID_GITPROFILE_USER_SIGNINGKEY_LEN) {
          fprintf(stderr, "parsed \"user.signingkey\" is too long\n");
          exit(EXIT_FAILURE);
        }
        strncpy(&current_git_profile->user_signingkey[0], val, val_len);
        current_git_profile->user_signingkey[val_len] = 0;
      }
      else if (key_len == 12 && !strncmp(key, "ssh_key_path", 12)) {
        if (val_len >= GID_GITPROFILE_SSH_KEY_PATH_LEN) {
          fprintf(stderr, "parsed \"ssh_key_path\" is too long\n");
          exit(EXIT_FAILURE);
        }
        strncpy(&current_git_profile->ssh_key_path[0], val, val_len);
        current_git_profile->ssh_key_path[val_len] = 0;
      }
    }
  }
  return result;
}

int gitProfileExists(
    GidConfiguration const*const restrict gid_configuration,
    char const*const restrict profile_name
) {
  for (size_t i = 0; i < gid_configuration->git_profiles_length; ++i) {
    if (!strcmp(&gid_configuration->git_profiles[i].name[0], profile_name))
      return i;
  }
  return -1;
}
