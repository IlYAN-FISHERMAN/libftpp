cmake_minimum_required(VERSION 3.16...3.30 FATAL_ERROR)

project(
  libftpp
  VERSION 1.0
  DESCRIPTION "my cpp library "
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(BUILD_CLIENT "Build clientGUI" OFF)
option(BUILD_BINARY "Build main binary" OFF)
option(BUILD_LIB "Build lib" OFF)

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
    tests/main_perlin_2D.cpp
    tests/main_arena_allocator.cc
    tests/main_timer.cc
    tests/main_responsability_chain.cc
    tests/main_non_copyable.cc
    tests/main_unique_memento.cc)

set(SOURCES ${SOURCES} data_structures/data_buffer/data_buffer.cc)

set(SOURCES
    ${SOURCES} design_patterns/memento/memento.cc
    design_patterns/responsability_chain/responsability_chain.cc
    design_patterns/arena_allocator/arena_allocator.cc)

set(SOURCES ${SOURCES} iostream/thread_safe_iostream.cc)

set(SOURCES
    ${SOURCES} threading/thread/thread.cc threading/worker_pool/worker_pool.cc
    threading/persistent_worker/persistent_worker.cc)

set(SOURCES
    ${SOURCES}
    network/message/message.cc
    network/client/client.cc
    network/logger/logger.cc
    network/server/server.cc
    network/prompt/prompt.cc
    network/matt_daemon/matt_daemon.cc)

set(SOURCES
    ${SOURCES}
    mathematics/random_2D_coordinate_generator/random_2D_coordinate_generator.cc
    mathematics/perlin_noise_2D/perlin_noise_2D.cc)

set(SOURCES ${SOURCES} time/chronometer/chronometer.cc time/timer/timer.cc)

add_library(libftpp STATIC ${SOURCES})
target_compile_features(libftpp PUBLIC cxx_std_23)

if(BUILD_CLIENT OR BUILD_BINARY)
  target_include_directories(libftpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

  if(BUILD_CLIENT)
    find_package(SDL3 CONFIG)

    set(CLIENT_GUI
        network/client/clientGUI.cc
        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_demo.cpp
        imgui/backends/imgui_impl_sdl3.cpp
        imgui/backends/imgui_impl_sdlrenderer3.cpp
        imgui/imgui_widgets.cpp
        imgui/misc/cpp/imgui_stdlib.cpp)

    add_executable(Ben_AFK ${CLIENT_GUI})

    target_link_libraries(Ben_AFK PUBLIC SDL3::SDL3)
    target_link_libraries(Ben_AFK PRIVATE libftpp)

    target_include_directories(Ben_AFK PRIVATE imgui)
    target_include_directories(Ben_AFK PRIVATE imgui/backends)
    target_include_directories(Ben_AFK PRIVATE imgui/misc/cpp/)
  endif()
  if(BUILD_BINARY)
    find_package(CURL REQUIRED)
    add_executable(matt-daemon tests/testMain.cc)
    target_link_libraries(matt-daemon PRIVATE libftpp)
    target_link_libraries(matt-daemon PRIVATE CURL::libcurl)
  endif()
elseif(BUILD_LIB)
  target_include_directories(
    libftpp
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/data_structures>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/design_patterns>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/iostream>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/mathematics>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/network>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/threading>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/time>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/utils>)
  install(
    TARGETS libftpp
    EXPORT libftppTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)

  install(FILES libftpp.hh DESTINATION include)
  install(DIRECTORY data_structures DESTINATION include)
  install(DIRECTORY design_patterns DESTINATION include)
  install(DIRECTORY iostream DESTINATION include)
  install(DIRECTORY mathematics DESTINATION include)
  install(DIRECTORY network DESTINATION include)
  install(DIRECTORY threading DESTINATION include)
  install(DIRECTORY time DESTINATION include)
  install(DIRECTORY utils DESTINATION include)

  install(
    EXPORT libftppTargets
    FILE libftppTargets.cmake
    NAMESPACE libftpp::
    DESTINATION lib/cmake/libftpp)

  install(FILES ftppConfig.cmake DESTINATION lib/cmake/libftpp)
  add_library(libftpp::libftpp ALIAS libftpp)
endif()
