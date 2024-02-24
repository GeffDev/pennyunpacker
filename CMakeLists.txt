cmake_minimum_required(VERSION 3.21)
project(pennyunpacker VERSION 1.0 LANGUAGES C)

# don't tell anyone :shhhh:
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")

set(CMAKE_C_STANDARD 17)

if (WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Wall")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-unused-parameter")
endif()

include_directories(pennyunpacker src/)
add_executable(pennyunpacker
    src/main.c
    src/extract.c
)