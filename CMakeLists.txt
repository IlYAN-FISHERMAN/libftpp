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
option(BUILD_NMAP "Build main binary" OFF)
option(BUILD_MATT_DAEMON "Build matt_daemon binary" OFF)

option(BUILD_LIB "Build lib" OFF)

find_package(CURL REQUIRED)
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
    tests/test_pool.cc
    tests/test_data_buffer.cc
    tests/test_memento.cc
    tests/test_observer.cc
    tests/test_singleton.cc
    tests/test_state_machine.cc
    tests/test_thread_safe_iostream.cc
    tests/test_thread_safe_queue.cc
    tests/test_thread.cc
    tests/test_worker_pool.cc
    tests/test_persistent_worker.cc
    tests/test_server.cc
    tests/test_client.cc
    tests/test_vector_2.cc
    tests/test_vector_3.cc
    tests/test_random_2D_coordinate_generator.cc
    tests/test_perlin_2D.cc
    tests/test_arena_allocator.cc
    tests/test_timer.cc
    tests/test_responsability_chain.cc
    tests/test_non_copyable.cc
    tests/test_system.cc
    tests/test_unique_memento.cc)

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
    network/matt_daemon/matt_daemon.cc
	network/vuln/attack/ip.cc
	network/vuln/nmap/nmap.cc)

set(SOURCES
    ${SOURCES}
    mathematics/random_2D_coordinate_generator/random_2D_coordinate_generator.cc
    mathematics/perlin_noise_2D/perlin_noise_2D.cc)

set(SOURCES ${SOURCES} time/chronometer/chronometer.cc time/timer/timer.cc)

set(SOURCES ${SOURCES} system/system_shell/system_shell.cc)

add_library(libftpp STATIC ${SOURCES})
target_compile_features(libftpp PUBLIC cxx_std_23)
target_link_libraries(libftpp PRIVATE CURL::libcurl)

if(BUILD_CLIENT OR BUILD_BINARY OR BUILD_NMAP OR BUILD_MATT_DAEMON)
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

    add_executable(server_client_gui ${CLIENT_GUI})

    target_link_libraries(server_client_gui PUBLIC SDL3::SDL3)
    target_link_libraries(server_client_gui PRIVATE libftpp)

    target_include_directories(server_client_gui PRIVATE imgui)
    target_include_directories(server_client_gui PRIVATE imgui/backends)
    target_include_directories(server_client_gui PRIVATE imgui/misc/cpp/)
  endif()
  if(BUILD_MATT_DAEMON)
	  add_executable(matt-daemon network/matt_daemon/matt_daemon.cc)
	  target_link_libraries(matt-daemon PRIVATE libftpp)
  endif()
  if(BUILD_NMAP)
	  add_executable(lppnmap network/vuln/attack/website_tracking.cc)
	  target_link_libraries(lppnmap PRIVATE libftpp)
	  target_link_libraries(lppnmap PRIVATE CURL::libcurl)
  endif()
  if(BUILD_BINARY)
	  add_executable(lib tests/main_tests.cc)
	  target_link_libraries(lib PRIVATE libftpp)
	  target_link_libraries(lib PRIVATE CURL::libcurl)
  endif()
elseif(BUILD_LIB)

	include(GNUInstallDirs)
	target_include_directories(
		libftpp PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

	install(
		TARGETS libftpp
		EXPORT libftppTargets
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

  install(FILES libftpp.hh DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

  install(DIRECTORY data_structures DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  install(DIRECTORY design_patterns DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  install(DIRECTORY iostream DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  install(DIRECTORY mathematics DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  install(DIRECTORY network DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  install(DIRECTORY threading DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  install(DIRECTORY time DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  install(DIRECTORY utils DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

  install(
    EXPORT libftppTargets
    FILE libftppTargets.cmake
    NAMESPACE libftpp::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libftpp)

  install(FILES ftppConfig.cmake
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libftpp)

  add_library(libftpp::libftpp ALIAS libftpp)

endif()
