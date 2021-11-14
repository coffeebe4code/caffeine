cmake_minimum_required(VERSION 3.16..3.16)

project(caffeine VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra -Wno-unused-command-line-argument")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY target)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)

link_directories(lib)

enable_testing()

find_package(Threads REQUIRED)

add_library(utils STATIC "")
add_library(server STATIC "")
add_library(barista STATIC "")
add_library(requester STATIC "")
add_library(responder STATIC "")

add_subdirectory(src/utils)
add_subdirectory(src/server)
add_subdirectory(src/barista)
add_subdirectory(src/requester)
add_subdirectory(src/responder)
add_subdirectory(tests)

