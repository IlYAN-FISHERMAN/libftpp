cmake_minimum_required(VERSION 3.16...3.30 FATAL_ERROR)

project(
  ftpp
  VERSION 1.0
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
    -g3
    -O0
    -fsanitize=address
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
  add_link_options(-g -O0 -fsanitize=thread -fno-omit-frame-pointer -lpthread)
  message(WARNING "Compiling for Macos with TSan options")
elseif(APPLE AND CMAKE_BUILD_TYPE MATCHES "Debug")
  add_link_options(-g -O0)
  message(WARNING "Compiling for Macos with debug options")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GCC")
  add_compile_options(-Wall -Wextra -Werror)
endif()

set(SOURCES
    tests/IoStat.cc
    tests/testMain.cc
    tests/main_pool.cpp
    tests/main_data_buffer.cpp
    tests/main_memento.cpp
    tests/main_observer.cpp
    tests/main_singleton.cpp
    tests/main_state_machine.cpp
    tests/main_thread_safe_iostream.cpp
    tests/main_thread_safe_queue.cpp
    tests/main_thread.cpp
    tests/main_worker_pool.cpp
    tests/main_persistent_worker.cpp
    tests/main_server.cpp
    tests/main_client.cpp
	tests/main_vector_2.cpp
	tests/main_vector_3.cpp
	tests/main_random_2D_coordinate_generator.cpp
	tests/main_perlin_2D.cpp)

set(SOURCES ${SOURCES} data_structures/data_buffer/data_buffer.cc)

set(SOURCES ${SOURCES} design_patterns/memento/memento.cc)

set(SOURCES ${SOURCES} iostream/thread_safe_iostream.cc)

set(SOURCES
    ${SOURCES} threading/thread/thread.cc threading/worker_pool/worker_pool.cc
    threading/persistent_worker/persistent_worker.cc)

set(SOURCES ${SOURCES} network/message/message.cc network/client/client.cc
            network/server/server.cc)

set(SOURCES ${SOURCES} mathematics/random_2D_coordinate_generator/random_2D_coordinate_generator.cc
mathematics/perlin_noise_2D/perlin_noise_2D.cc)

set(SOURCES ${SOURCES} chronometer/chronometer.cc)

add_executable(lib ${SOURCES})
add_library(ftpp STATIC ${SOURCES})

target_include_directories(ftpp PRIVATE data_structures)
target_include_directories(ftpp PRIVATE iostream)
target_include_directories(ftpp PRIVATE mathematics)
target_include_directories(ftpp PRIVATE networks)
target_include_directories(ftpp PRIVATE threading)
target_include_directories(ftpp PRIVATE design_patterns)
target_include_directories(ftpp PRIVATE chronometer)
