cmake_minimum_required(VERSION 3.7)
project(pthread)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c)
set(SOURCE_FILES mystrerror.c)
set(SOURCE_FILES pthread_attr_sample.c)
set(SOURCE_FILES specific_data.c)
set(SOURCE_FILES thread_cancel.c)
set(SOURCE_FILES thread_sync.c)
set(SOURCE_FILES thread_sync_mutex.c)
set(SOURCE_FILES thread_sync_cond.c)

find_package (Threads)

add_executable(pthread ${SOURCE_FILES})

target_link_libraries (pthread ${CMAKE_THREAD_LIBS_INIT})