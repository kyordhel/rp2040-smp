# Top level CMakeLists.txt to build the rp2040 Demos
cmake_minimum_required(VERSION 3.13)

# Pull in SDK (must be before project)
include(picoSDK_import.cmake)
include(freeRTOS_import.cmake)

PROJECT(rp2040smp)

add_subdirectory(hello-multicore)

