# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialize pico_sdk from installed location
set(ENV{PICO_SDK_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/pico-sdk")

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

# Pull in FreeRTOS
include(FreeRTOS_Kernel_import.cmake)

project(GOM-JABBAR
        LANGUAGES C CXX ASM
)

# Optional flags section: will be important later
option(BUILD_DEMO_APP ON "Builds the demo applicaiton for the RP2040W")

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()
#config values for the kernel, kernel is compiled with our application
add_subdirectory(config)

#We are working with ARM-CORTEX-M0+ architecture
set(FREERTOS_PORT "GCC_ARM_CM0" )

add_compile_definitions( configNUM_CORES=2 )

if( BUILD_DEMO_APP )
        add_subdirectory(App)
endif()

# STARTER CODE BELOW HERE, MARKED FOR REMOVAL
#===================================================================================

# Add executable. Default name is the project name, version 0.1
add_executable(GOM-JABBAR GOM-JABBAR.cpp )

# # Generate PIO header
pico_generate_pio_header(GOM-JABBAR ${CMAKE_CURRENT_LIST_DIR}/blink.pio)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(GOM-JABBAR 1)
pico_enable_stdio_usb(GOM-JABBAR 0)

# Add the standard library to the build
target_link_libraries(GOM-JABBAR pico_stdlib FreeRTOS-Kernel FreeRTOS-Kernel-Heap4 freertos_config)

# Add the standard include files to the build
target_include_directories(GOM-JABBAR PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/config
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

