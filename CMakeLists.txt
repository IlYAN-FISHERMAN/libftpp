cmake_minimum_required(VERSION 3.16...3.30 FATAL_ERROR)

project(
  libftpp
  DESCRIPTION "my cpp library "
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(DEFINED CMAKE_BUILD_TYPE)
  if(UNIX
     AND NOT APPLE
     AND CMAKE_BUILD_TYPE MATCHES "Debug")
    add_link_options(-g -O0)
    message(WARNING "Linux debug compilation")
  elseif(APPLE AND CMAKE_BUILD_TYPE MATCHES "ASan")
    add_link_options(
      -g
      -O0
      -fsanitize=address,undefined
      -fsanitize=pointer-compare
      -fsanitize=pointer-subtract
      -fsanitize=bounds
      -fsanitize=nonnull-attribute
      -fno-omit-frame-pointer
      -fgpu-sanitize
      -shared-libsan
      -fsanitize-address-use-after-scope)
    message(WARNING "Macos ASan debug compilation")
  elseif(APPLE AND CMAKE_BUILD_TYPE MATCHES "Debug")
    add_link_options(-g -O0)
    message(WARNING "Macos debug compilation")
  endif()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GCC")
  add_compile_options(-Wall -Wextra -Werror)
endif()

set(SOURCES tests/testMain.cc tests/main_pool.cpp tests/main_data_buffer.cpp
            tests/main_memento.cpp)

set(SOURCES ${SOURCES} data_structures/data_buffer/data_buffer.cc)

set(SOURCES ${SOURCES} design_patterns/memento/memento.cc)

add_executable(libftpp ${SOURCES})

target_include_directories(libftpp PRIVATE data_structures)
target_include_directories(libftpp PRIVATE design_patterns)
target_include_directories(libftpp PRIVATE iostream)
target_include_directories(libftpp PRIVATE mathematics)
target_include_directories(libftpp PRIVATE networks)
target_include_directories(libftpp PRIVATE thread)
