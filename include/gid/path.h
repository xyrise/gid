/* path.h */
#ifndef GID_PATH_H
#define GID_PATH_H

#include <stddef.h>


// Copy directory path including trailing slash to destination
void dirName(char* restrict, char const*const);

// Number of characters in directory path including trailing slash
size_t dirLength(char const*const);

void fillTrailingSlash(char*);
void stripTrailingSlash(char*);


#endif /* GID_PATH_H */
