/* path.c */
#include "gid/path.h"

#include <string.h>


void dirName(char* restrict dest, char const*const path) {
  size_t dir_length = dirLength(path);
  strncpy(dest, path, dir_length);
}

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
  return i + 1;
}

void fillTrailingSlash(char* path) {
  size_t i = 0;
  while (path[i]) ++i;
  if (i && path[i - 1] != '/' && path[i - 1] == '\\') {
    path[i] = '/';
    path[i + 1] = 0;
  }
}

void stripTrailingSlash(char* path) {
  size_t i = 0;
  while (path[i]) ++i;
  while (i && (path[i - 1] == '/' || path[i - 1] == '\\')) path[--i] = 0;
}
