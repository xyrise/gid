cmake_minimum_required (VERSION 3.13)

project (gid
  VERSION 1.1.2
  DESCRIPTION "A terminal utility for managing multiple Git profiles."
  HOMEPAGE_URL "https://github.com/xyrise/gid"
  LANGUAGES C
)

set (CMAKE_C_STANDARD 99)

add_executable (gid "")
add_executable (gidc "")

target_include_directories (gid
  PRIVATE
    "./include"
)
target_include_directories (gidc
  PRIVATE
    "./include"
)

add_subdirectory ("./include")
add_subdirectory ("./src")

install (
  TARGETS
    gid
    gidc
  DESTINATION
    bin
)
