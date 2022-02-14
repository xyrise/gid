/* gitprofile.h */
#ifndef GID_GITPROFILE_H
#define GID_GITPROFILE_H

#include <stdbool.h>
#include <stddef.h>


#define GID_GITPROFILE_NAME_LEN 32
#define GID_GITPROFILE_USER_NAME_LEN 32
#define GID_GITPROFILE_USER_EMAIL_LEN 64
#define GID_GITPROFILE_USER_SIGNINGKEY_LEN 32
#define GID_GITPROFILE_COMMIT_GPGSIGN_LEN 8
#define GID_GITPROFILE_COMMIT_TEMPLATE_LEN 512
#define GID_GITPROFILE_PULL_REBASE_LEN 8
#define GID_GITPROFILE_TAG_GPGSIGN_LEN 8
#define GID_GITPROFILE_SSH_KEY_PATH_LEN 512

typedef struct {
  char name[GID_GITPROFILE_NAME_LEN];

  char user_name[GID_GITPROFILE_USER_NAME_LEN];
  char user_email[GID_GITPROFILE_USER_EMAIL_LEN];
  char user_signingkey[GID_GITPROFILE_USER_SIGNINGKEY_LEN];
  char commit_gpgsign[GID_GITPROFILE_COMMIT_GPGSIGN_LEN];
  char commit_template[GID_GITPROFILE_COMMIT_TEMPLATE_LEN];
  char pull_rebase[GID_GITPROFILE_PULL_REBASE_LEN];
  char tag_gpgsign[GID_GITPROFILE_TAG_GPGSIGN_LEN];
  char ssh_key_path[GID_GITPROFILE_SSH_KEY_PATH_LEN];
} GidGitProfile;

int concatParamStrings(
    GidGitProfile const* profile,
    char* dest,
    size_t const dest_size,
    char const* param_prefix,
    char const* param_infix,
    char const* param_suffix,
    bool system_call
);


#endif /* GID_GITPROFILE_H */
