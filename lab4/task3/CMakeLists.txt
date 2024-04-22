cmake_minimum_required(VERSION 3.10)
project(task3)

set(SOURCES 
    src/lib/my_stdlib.c
    src/main.c
)

add_library(my_stdlib SHARED src/lib/my_stdlib.c)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} my_stdlib)
