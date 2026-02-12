cmake_minimum_required(VERSION 3.16...3.30 FATAL_ERROR)

project(
  libftpp
  DESCRIPTION "my cpp library "
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GCC")
  add_compile_options(-Wall -Wextra -Werror)
endif()

add_executable(lib tests/testMain.cc tests/main_pool.cpp)
target_include_directories(lib PRIVATE include)

# add_compile_options(-fsanitize=address) add_link_options(-fsanitize=address
# -g3)
