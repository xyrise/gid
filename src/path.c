/* path.c */
#include "gid/path.h"

#include <string.h>


/* Return provided path up to (including) last separator */
void dirName(char* dest, char const*const path) {
  size_t dir_length = dirLength(path);
  strncpy(dest, path, dir_length);
}

/* Return length of provided path up to (including) last separator */
size_t dirLength(char const*const path) {
  size_t i = 0;
  size_t last_slash = 0;
  while (path[i]) {
    switch (path[i]) {
      case '/':
      case '\\':
        last_slash = i;
        break;
      default: break;
    }
    ++i;
  }
  return last_slash + 1;
}

void fillTrailingSlash(char* path) {
  size_t i = 0;
  while (path[i]) ++i;
  if (i && path[i - 1] != '/' && path[i - 1] != '\\') {
    path[i] = '/';
    path[i + 1] = 0;
  }
}

void stripTrailingSlash(char* path) {
  size_t i = 0;
  while (path[i]) ++i;
  while (i && (path[i - 1] == '/' || path[i - 1] == '\\')) path[--i] = 0;
}
