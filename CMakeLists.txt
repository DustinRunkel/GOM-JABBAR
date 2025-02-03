# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type") 

#We are working with ARM-CORTEX-M0+ architecture
set(FREERTOS_PORT "GCC_ARM_CM0")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(GOM-JABBAR C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

#config values for the kernel, kernel is compiled with our application
add_subdirectory(config)

#build kernel with our project
add_subdirectory(FreeRTOS-Kernel)

# Add executable. Default name is the project name, version 0.1

add_executable(GOM-JABBAR GOM-JABBAR.cpp )

pico_set_program_name(GOM-JABBAR "GOM-JABBAR")
pico_set_program_version(GOM-JABBAR "0.1")

# Generate PIO header
pico_generate_pio_header(GOM-JABBAR ${CMAKE_CURRENT_LIST_DIR}/blink.pio)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(GOM-JABBAR 1)
pico_enable_stdio_usb(GOM-JABBAR 0)

# Add the standard library to the build
target_link_libraries(GOM-JABBAR
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(GOM-JABBAR PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Link libs for demo application -- bare metal
target_link_libraries(GOM-JABBAR 
        hardware_spi
        hardware_i2c
        hardware_dma
        hardware_pio
        hardware_interp
        hardware_timer
        hardware_watchdog
        hardware_clocks
        pico_cyw43_arch_none
        )

pico_add_extra_outputs(GOM-JABBAR)

