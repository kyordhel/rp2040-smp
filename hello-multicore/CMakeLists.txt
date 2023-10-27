cmake_minimum_required(VERSION 3.13)

# Pull in SDK (must be before project)
include(../picoSDK_import.cmake)

# Pull in FreeRTOS
include(FreeRTOS_Kernel_import.cmake)

project(helloMulticore C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(helloMulticore
	main.c
	aux.c
)

target_include_directories(helloMulticore PRIVATE
	${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(helloMulticore
	pico_stdlib
	FreeRTOS-Kernel
	FreeRTOS-Kernel-Heap1
	pico_multicore
)
pico_add_extra_outputs(helloMulticore)
pico_enable_stdio_usb(helloMulticore 1)
pico_enable_stdio_uart(helloMulticore 0)
