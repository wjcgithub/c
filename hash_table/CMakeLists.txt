cmake_minimum_required(VERSION 3.7)
project(hash_table)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c hashtable.h hashtable.c)
add_executable(hash_table ${SOURCE_FILES})