cmake_minimum_required(VERSION 3.8)
project(default)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        deqll.c
        deqll.h
        main.c
        receiver.c
        receiver.h car2.c car2.h)

add_executable(default ${SOURCE_FILES} car2.c)