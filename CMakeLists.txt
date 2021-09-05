cmake_minimum_required (VERSION 3.13)

project (gid
  VERSION 0.1.0
  DESCRIPTION ""
  HOMEPAGE_URL "https://github.com/xyrise/gid"
  LANGUAGES C
)

set (CMAKE_C_STANDARD 99)

add_executable (gid "")
target_include_directories (gid
  PRIVATE
    "./include"
)

add_subdirectory ("./include")
add_subdirectory ("./src")

install (
  TARGETS
    gid
  DESTINATION
    bin
)
