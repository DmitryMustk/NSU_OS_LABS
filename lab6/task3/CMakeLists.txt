cmake_minimum_required(VERSION 3.0)
project(task1)

set(CMAKE_C_STANDARD 11)

# Указываем путь к исходным файлам
set(SOURCE_FILES_SERVER src/server.c)
set(SOURCE_FILES_CLIENT src/client.c)

# Компилируем сервер
add_executable(server ${SOURCE_FILES_SERVER})

# Компилируем клиент
add_executable(client ${SOURCE_FILES_CLIENT})

