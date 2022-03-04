/* gitprofile.c */
#include "gid/gitprofile.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


size_t concatParamString(
    char* dest,
    size_t const orig_dest_offset,
    char const* name,
    size_t const name_len,
    char const* value,
    size_t const value_len,
    char const* prefix,
    size_t const prefix_len,
    char const* infix,
    size_t const infix_len,
    char const* suffix,
    size_t const suffix_len,
    bool system_call
) {
  size_t dest_offset = orig_dest_offset;
  if (prefix) {
    memcpy(dest + dest_offset, prefix, prefix_len);
    dest_offset += prefix_len;
  }
  memcpy(dest + dest_offset, name, name_len);
  dest_offset += name_len;
  if (infix) {
    memcpy(dest + dest_offset, infix, infix_len);
    dest_offset += infix_len;
  }
  memcpy(dest + dest_offset, value, value_len);
  dest_offset += value_len;
  if (suffix) {
    memcpy(dest + dest_offset, suffix, suffix_len);
    dest_offset += suffix_len;
  }
  if (system_call) {
    dest[dest_offset] = 0;
    dest_offset = orig_dest_offset;
    if (system(dest)) {
      fprintf(stderr, "Git command returned with non-zero exit code\n");
    }
  }
  return dest_offset;
}

int concatParamStrings(
    GidGitProfile const* profile,
    char* dest,
    size_t const dest_size,
    char const* param_prefix,
    char const* param_infix,
    char const* param_suffix,
    bool system_call
) {
  int num_params = 0;
  size_t dest_offset = strlen(dest);

  size_t prefix_len = 0;
  if (param_prefix) prefix_len += strlen(param_prefix);

  size_t infix_len = 0;
  if (param_infix) infix_len += strlen(param_infix);

  size_t suffix_len = 0;
  if (param_suffix) suffix_len += strlen(param_suffix);

  size_t comb_len = prefix_len + infix_len + suffix_len;

  if (profile->user_name[0]) {
    size_t user_name_len = strlen(profile->user_name);
    size_t total_len = dest_offset + comb_len + user_name_len + 9;
    if (total_len + 1 < dest_size) {
      dest_offset = concatParamString(
          dest,
          dest_offset,
          "user.name",
          9,
          &(profile->user_name[0]),
          user_name_len,
          param_prefix,
          prefix_len,
          param_infix,
          infix_len,
          param_suffix,
          suffix_len,
          system_call
      );
      num_params += 1;
    }
  }
  if (profile->user_email[0]) {
    size_t user_email_len = strlen(profile->user_email);
    size_t total_len = dest_offset + comb_len + user_email_len + 10;
    if (total_len + 1 < dest_size) {
      dest_offset = concatParamString(
          dest,
          dest_offset,
          "user.email",
          10,
          &(profile->user_email[0]),
          user_email_len,
          param_prefix,
          prefix_len,
          param_infix,
          infix_len,
          param_suffix,
          suffix_len,
          system_call
      );
      num_params += 1;
    }
  }
  if (profile->user_signingkey[0]) {
    size_t user_signingkey_len = strlen(profile->user_signingkey);
    size_t total_len = dest_offset + comb_len + user_signingkey_len + 15;
    if (total_len + 1 < dest_size) {
      dest_offset = concatParamString(
          dest,
          dest_offset,
          "user.signingkey",
          15,
          &(profile->user_signingkey[0]),
          user_signingkey_len,
          param_prefix,
          prefix_len,
          param_infix,
          infix_len,
          param_suffix,
          suffix_len,
          system_call
      );
      num_params += 1;
    }
  }
  if (profile->commit_gpgsign[0]) {
    size_t commit_gpgsign_len = strlen(profile->commit_gpgsign);
    size_t total_len = dest_offset + comb_len + commit_gpgsign_len + 14;
    if (total_len + 1 < dest_size) {
      dest_offset = concatParamString(
          dest,
          dest_offset,
          "commit.gpgsign",
          14,
          &(profile->commit_gpgsign[0]),
          commit_gpgsign_len,
          param_prefix,
          prefix_len,
          param_infix,
          infix_len,
          param_suffix,
          suffix_len,
          system_call
      );
      num_params += 1;
    }
  }
  if (profile->commit_template[0]) {
    size_t commit_template_len = strlen(profile->commit_template);
    size_t total_len = dest_offset + comb_len + commit_template_len + 14;
    if (total_len + 1 < dest_size) {
      dest_offset = concatParamString(
          dest,
          dest_offset,
          "commit.template",
          15,
          &(profile->commit_template[0]),
          commit_template_len,
          param_prefix,
          prefix_len,
          param_infix,
          infix_len,
          param_suffix,
          suffix_len,
          system_call
      );
      num_params += 1;
    }
  }
  if (profile->pull_rebase[0]) {
    size_t pull_rebase_len = strlen(profile->pull_rebase);
    size_t total_len = dest_offset + comb_len + pull_rebase_len + 11;
    if (total_len + 1 < dest_size) {
      dest_offset = concatParamString(
          dest,
          dest_offset,
          "pull.rebase",
          11,
          &(profile->pull_rebase[0]),
          pull_rebase_len,
          param_prefix,
          prefix_len,
          param_infix,
          infix_len,
          param_suffix,
          suffix_len,
          system_call
      );
      num_params += 1;
    }
  }
  if (profile->tag_gpgsign[0]) {
    size_t tag_gpgsign_len = strlen(profile->tag_gpgsign);
    size_t total_len = dest_offset + comb_len + tag_gpgsign_len + 11;
    if (total_len + 1 < dest_size) {
      dest_offset = concatParamString(
          dest,
          dest_offset,
          "tag.gpgsign",
          11,
          profile->tag_gpgsign,
          tag_gpgsign_len,
          param_prefix,
          prefix_len,
          param_infix,
          infix_len,
          param_suffix,
          suffix_len,
          system_call
      );
      num_params += 1;
    }
  }
  if (profile->ssh_key_path[0]) {
    size_t ssh_key_path_len = strlen(profile->ssh_key_path);
    char new_value[GID_GITPROFILE_SSH_KEY_PATH_LEN + 7];
    sprintf(new_value, "ssh -i ");
    memcpy(new_value + 7, profile->ssh_key_path, ssh_key_path_len);
    ssh_key_path_len += 7;
    new_value[ssh_key_path_len] = 0;
    size_t total_len = dest_offset + comb_len + ssh_key_path_len + 15;
    if (total_len + 1 < dest_size) {
      dest_offset = concatParamString(
          dest,
          dest_offset,
          "core.sshCommand",
          15,
          new_value,
          ssh_key_path_len,
          param_prefix,
          prefix_len,
          param_infix,
          infix_len,
          param_suffix,
          suffix_len,
          system_call
      );
      num_params += 1;
    }
  }
  if (num_params > 0) dest[dest_offset] = 0;
  return num_params;
}
