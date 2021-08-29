cmake_minimum_required (VERSION 3.13)

project (gid
  VERSION 0.0.1
  DESCRIPTION ""
  HOMEPAGE_URL "https://github.com/xyrise/gid"
  LANGUAGES CXX
)

set (CMAKE_CXX_STANDARD 17)

add_executable (gid "")
target_include_directories (gid
  PRIVATE
    "./include"
)

add_subdirectory ("./include")
add_subdirectory ("./src")
