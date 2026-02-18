cmake_minimum_required(VERSION 3.16...3.30 FATAL_ERROR)

project(
  libftpp
  DESCRIPTION "my cpp library "
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

add_link_options(
  -g
  -O0
  -\#\#\#
  -fsanitize=address,undefined
  -fsanitize=pointer-compare
  -fsanitize=pointer-subtract
  -fsanitize=bounds
  -fsanitize=nonnull-attribute
  -fno-omit-frame-pointer
  -fgpu-sanitize
  -shared-libsan
  -fsanitize-address-use-after-scope)

# add_link_options(-g -O0)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GCC")
  add_compile_options(-Wall -Wextra -Werror)
endif()

add_executable(lib tests/testMain.cc tests/main_pool.cpp
                   tests/main_data_buffer.cpp src/data_buffer.cc)
target_include_directories(lib PRIVATE include)
