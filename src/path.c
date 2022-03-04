/* path.c */
#include "gid/path.h"

#include <string.h>


/* Return provided path up to (including) last separator */
void dirName(char *const dest, size_t const destsz, char const *const path) {
  if (!dest || !path || !destsz) return;
  size_t dir_length = dirLength(path, destsz);
  if (dir_length >= destsz) return;
  if (dest != path) memcpy(dest, path, dir_length);
  dest[dir_length] = 0;
}

/* Return length of provided path up to (including) last separator */
size_t dirLength(char const *const path, size_t const strsz) {
  if (!path || !strsz) return 0;
  size_t len = 0;
  for (size_t i = 0; i < strsz && path[i]; ++i)
    if (path[i] == '/' || path[i] == '\\') len = i;
  return len + 1;
}

void fillTrailingSlash(
    char *const dest,
    size_t const destsz,
    char const *const path
) {
  if (!dest || !path || !destsz) return;
  size_t path_len = strlen(path);
  if (path_len + 1 >= destsz) return;
  if (dest != path) memcpy(dest, path, path_len);
  if (path_len && path[path_len - 1] != '/' && path[path_len - 1] != '\\') {
    dest[path_len] = '/';
    dest[path_len + 1] = 0;
  }
}

void stripTrailingSlash(
    char *const dest,
    size_t const destsz,
    char const *const path
) {
  if (!dest || !path || !destsz) return;
  size_t path_len = strlen(path);
  while (path_len && (path[path_len - 1] == '/' || path[path_len - 1] == '\\'))
    --path_len;
  if (path_len >= destsz) return;
  if (dest != path) memcpy(dest, path, path_len);
  dest[path_len] = 0;
}
