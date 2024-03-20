cmake_minimum_required(VERSION 3.5.1)
project(task2)

set(SRC src/main.c src/dir_funcs.c src/file_funcs.c  src/hard_link_funcs.c src/permission_funcs.c src/sym_link_funcs.c
    src/help.c src/util.c)

add_executable(${PROJECT_NAME} ${SRC})
add_compile_options("-Wall -Wextra")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules")
