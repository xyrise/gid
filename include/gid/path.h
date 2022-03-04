/* path.h */
#ifndef GID_PATH_H
#define GID_PATH_H

#include <stddef.h>


// Copy directory path including trailing slash to destination
void dirName(char *const, size_t const, char const*const);

// Number of characters in directory path including trailing slash
size_t dirLength(char const *const, size_t const);

void fillTrailingSlash(char *const, size_t const, char const *const);
void stripTrailingSlash(char *const, size_t const, char const *const);


#endif /* GID_PATH_H */
