cmake_minimum_required (VERSION 3.9)

project(meshed
  VERSION 1.0.0
  LANGUAGES C
)

cmake_policy(SET CMP0077 NEW)

find_package(GDAL REQUIRED)
find_package(OpenMP REQUIRED)

set(CMAKE_C_STANDARD 99)

set(WARNING_FLAGS
  -Waddress
  -Wall
  -Wdeprecated
  -Wextra
  -Wimplicit-fallthrough
  -Wmisleading-indentation
  -Wmissing-prototypes
  -Wshadow
  -Wshift-sign-overflow
  -Wswitch
  -Wuninitialized
  -Wunreachable-code
  -Wunused
  -Wunused-but-set-variable
  -Wunused-function
  # -Werror
)

add_executable(meshed.exe
  delineate.c
  delineate_lessmem.c
  delineate_moremem.c
  gettimeofday.c
  hierarchy.c
  main.c
  outlet_list.c
  outlets.c
  raster.c
  timeval_diff.c
)

target_link_libraries(meshed.exe PRIVATE GDAL::GDAL OpenMP::OpenMP_C m)
target_compile_options(meshed.exe PRIVATE ${WARNING_FLAGS})
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
  target_compile_options(meshed.exe PRIVATE -foptimize-sibling-calls)
endif()
