cmake_minimum_required(VERSION 3.16...3.30 FATAL_ERROR)

project(
  libftpp
  DESCRIPTION "my cpp library "
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(UNIX
   AND NOT APPLE
   AND CMAKE_BUILD_TYPE MATCHES "Debug")
  add_link_options(-g -O0)
  message(WARNING "Linux debug Compiling")
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
  message(WARNING "Compiling for Macos with ASan options")
elseif(APPLE AND CMAKE_BUILD_TYPE MATCHES "TSan")
  add_link_options(-g -O0 -fsanitize=thread -fno-omit-frame-pointer)
  message(WARNING "Compiling for Macos with TSan options")
elseif(APPLE AND CMAKE_BUILD_TYPE MATCHES "Debug")
  add_link_options(-g -O0)
  message(WARNING "Compiling for Macos with debug options")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GCC")
  add_compile_options(-Wall -Wextra -Werror)
endif()

set(SOURCES
    tests/testMain.cc
    tests/main_pool.cpp
    tests/main_data_buffer.cpp
    tests/main_memento.cpp
    tests/main_observer.cpp
    tests/main_singleton.cpp
    tests/main_state_machine.cpp
    tests/main_thread_safe_iostream.cpp)

set(SOURCES ${SOURCES} data_structures/data_buffer/data_buffer.cc)

set(SOURCES ${SOURCES} design_patterns/memento/memento.cc)

set(SOURCES ${SOURCES} iostream/thread_safe_iostream.cc)

add_executable(libftpp ${SOURCES})

target_include_directories(libftpp PRIVATE data_structures)
target_include_directories(libftpp PRIVATE iostream)
target_include_directories(libftpp PRIVATE mathematics)
target_include_directories(libftpp PRIVATE networks)
target_include_directories(libftpp PRIVATE threading)
target_include_directories(libftpp PRIVATE design_patterns)
