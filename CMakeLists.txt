cmake_minimum_required(VERSION 3.13...3.27)

include(pico_sdk_import.cmake)
# Pull in FreeRTOS
include(FreeRTOS_Kernel_import.cmake)
include_directories(${CMAKE_SOURCE_DIR})

project(test_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

#set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/out)

pico_sdk_init()

# Add subdirectories
add_subdirectory(app)
add_subdirectory(taskMgr)

add_executable(main
main.c
)

pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)

pico_add_extra_outputs(main)

target_link_libraries(main
  pico_stdlib 
  FreeRTOS-Kernel 
  FreeRTOS-Kernel-Heap4
  app
  taskMgr
  )
