# Create a library target
add_library(taskMgr)

# Add source files to the library
target_sources(taskMgr
    PRIVATE
        src/health_monitor_task.c
        src/taskMgr.c
)

message(${CMAKE_CURRENT_SOURCE_DIR})

# Set include directories
target_include_directories(taskMgr
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include 
)

target_link_libraries(taskMgr
  pico_stdlib 
  FreeRTOS-Kernel
  app
  )