cmake_minimum_required(VERSION 3.16)

project(ex2)

set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -Wextra -Werror -pedantic -Wno-unused-result -Wconversion -Wvla")
file(GLOB SOURCE_FILES "*.c")

add_executable(l5 ${SOURCE_FILES})
add_executable(l5_memory_check ${SOURCE_FILES})

target_compile_options(l5_memory_check PUBLIC -fsanitize=address,undefined,integer -fno-omit-frame-pointer)
target_link_libraries(l5_memory_check -fsanitize=address,undefined,integer)