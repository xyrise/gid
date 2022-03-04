cmake_minimum_required (VERSION 3.13)

project (gid
  VERSION 0.1.1
  DESCRIPTION "A terminal utility for managing multiple Git profiles."
  HOMEPAGE_URL "https://github.com/xyrise/gid"
  LANGUAGES C
)

set (CMAKE_C_STANDARD 11)
set (CMAKE_C_STANDARD_REQUIRED 11)

if (MSVC)
  add_compile_options(
    /Wall
    /WX
    /wd4710
    /wd4711
    /wd5045
    /wd4820
    /wd4706
    /Qspectre
  )
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
  add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

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
