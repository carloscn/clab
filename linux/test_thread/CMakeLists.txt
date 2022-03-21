cmake_minimum_required(VERSION 3.15.3)
project(test.elf C)
SET(CMAKE_BUILD_TYPE "Debug")
MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")
set(CMAKE_C_STANDARD 99)
#AUX_SOURCE_DIRECTORY(./ DIR_SRCS)
#set(MAIN_SRC_PATH test_thread.c)
#set(TEST1_SRC_PATH test_thread_sem.c)
set(TEST2_SRC_PATH test_thread_spinlock.c)
include_directories(./)
link_libraries()

set(CMAKE_C_FLAGS
    -lpthread
    )

#add_executable(${PROJECT_NAME} ${MAIN_SRC_PATH})
#add_executable("test_thread_sem.elf" ${TEST1_SRC_PATH})
add_executable("test_thread_spinlock.elf" ${TEST2_SRC_PATH})
