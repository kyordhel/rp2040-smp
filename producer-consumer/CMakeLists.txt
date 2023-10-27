cmake_minimum_required(VERSION 3.13)

# Pull in SDK (must be before project)
include(../picoSDK_import.cmake)

# Pull in FreeRTOS
include(FreeRTOS_Kernel_import.cmake)

project(producerConsumer C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(producerConsumer
	main.c
	aux.c
)

target_include_directories(producerConsumer PRIVATE
	${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(producerConsumer
	pico_stdlib
	FreeRTOS-Kernel
	FreeRTOS-Kernel-Heap1
	pico_multicore
)
pico_add_extra_outputs(producerConsumer)
pico_enable_stdio_usb(producerConsumer 1)
pico_enable_stdio_uart(producerConsumer 0)
