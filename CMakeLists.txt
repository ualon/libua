project(test-ua)

cmake_minimum_required(VERSION 2.6)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_C_FLAGS_DEBUG "-DDEBUG -g3 -ggdb")

set(CMAKE_C_FLAGS
  "${CMAKE_C_FLAGS} -std=gnu99 -Wall -Wpointer-arith -Wno-format-y2k -Wstrict-prototypes -Wmissing-declarations -Wnested-externs -Wextra -Wundef -Wwrite-strings -Wold-style-definition -Wno-missing-field-initializers -Wredundant-decls -Wno-unused-parameter -Wno-sign-compare -Wmissing-prototypes")

add_executable(test-mem-pool test-mem-pool.c mem_pool.c)

add_executable(test-block-ring-buffer test-block-ring-buffer.c block_ring_buffer.c)

add_executable(test-state-machine test-state-machine.c state_machine.c)
target_link_libraries (test-state-machine pthread)

