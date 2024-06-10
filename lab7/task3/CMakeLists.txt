cmake_minimum_required(VERSION 3.0)
project(task3)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES_SERVER src/server.c)
set(SOURCE_FILES_CLIENT src/client.c)

add_executable(server ${SOURCE_FILES_SERVER})

add_executable(client ${SOURCE_FILES_CLIENT})

