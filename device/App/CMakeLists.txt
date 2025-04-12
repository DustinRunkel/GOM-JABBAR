# ________  ________  _____ ______                             
# |\   ____\|\   __  \|\   _ \  _   \                           
# \ \  \___|\ \  \|\  \ \  \\\__\ \  \                          
#  \ \  \  __\ \  \\\  \ \  \\|__| \  \                         
#   \ \  \|\  \ \  \\\  \ \  \    \ \  \                        
#    \ \_______\ \_______\ \__\    \ \__\                       
#     \|_______|\|_______|\|__|_____\|__|________  ________     
#    |\  \|\   __  \|\   __  \|\   __  \|\   __  \|\   __  \    
#    \ \  \ \  \|\  \ \  \|\ /\ \  \|\ /\ \  \|\  \ \  \|\  \   
#  __ \ \  \ \   __  \ \   __  \ \   __  \ \   __  \ \   _  _\  
# |\  \\_\  \ \  \ \  \ \  \|\  \ \  \|\  \ \  \ \  \ \  \\  \| 
# \ \________\ \__\ \__\ \_______\ \_______\ \__\ \__\ \__\\ _\ 
#  \|________|\|__|\|__|\|_______|\|_______|\|__|\|__|\|__|\|__|
#
# Directory where our demonstration app lives

# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")

# # Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

# Pull in FreeRTOS
include(FreeRTOS_Kernel_import.cmake)

project(jabbar_app
        LANGUAGES C CXX ASM
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

#We are working with ARM-CORTEX-M0+ architecture
#set(FREERTOS_PORT "GCC_ARM_CM0" )

add_subdirectory(Gom-Jabbar)
add_subdirectory(source)
add_subdirectory(config)