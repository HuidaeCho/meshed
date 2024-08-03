cmake_minimum_required (VERSION 3.9)
cmake_policy(SET CMP0077 NEW)

project(meshed
  VERSION 1.0.0
  LANGUAGES C
)

option(USE_ASAN "Enable or disable address sanitizer" OFF)
option(USE_USAN "Enable or disable undefined sanitizer" OFF)

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
set_property(TARGET meshed.exe PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
  target_compile_options(meshed.exe PRIVATE -foptimize-sibling-calls)
endif()
if(USE_ASAN)
  target_compile_options(meshed.exe PRIVATE -fsanitize=address)
  target_link_options(meshed.exe PRIVATE -fsanitize=address)
endif()

if(USE_USAN)
  target_compile_options(meshed.exe PRIVATE -fsanitize=undefined)
  target_link_options(meshed.exe PRIVATE -fsanitize=undefined)
endif()
