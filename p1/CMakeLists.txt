cmake_minimum_required(VERSION 2.7)

project(ve482p1)

set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -Wextra -Werror -pedantic -Wno-unused-result -Wconversion")
file(GLOB SOURCE_FILES "*.c")

add_executable(mumsh ${SOURCE_FILES})
add_executable(mumsh_memory_check ${SOURCE_FILES})

target_compile_options(mumsh_memory_check PUBLIC -fsanitize=address,undefined,integer -fno-omit-frame-pointer)
target_link_libraries(mumsh_memory_check -fsanitize=address,undefined,integer)