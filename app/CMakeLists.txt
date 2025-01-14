# Create a library target
add_library(app)

# Add source files to the library
target_sources(app
    PRIVATE
        src/led_out.c
        src/serial_in.c
)

# Set include directories
target_include_directories(app
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include 
)

target_link_libraries(app
  pico_stdlib 
  FreeRTOS-Kernel
  )