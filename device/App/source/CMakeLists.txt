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
# Main.c is our entrypoint into the FreeRTOS kernel

set(target jabbar_app)

add_executable(${target} main.cpp)

target_link_libraries(${target} PUBLIC pico_stdlib pico_cyw43_arch_none FreeRTOS-Kernel FreeRTOS-Kernel-Heap3 freertos_config gom_jabbar gj_config)

target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_LIST_DIR} ../Gom-Jabbar/include ../config )

#target_compile_options(${target} PUBLIC "-frtti")

# Enable/disable STDIO via USB and UART
pico_enable_stdio_usb(${target} 1)
pico_enable_stdio_uart(${target} 1)

pico_add_extra_outputs( ${target} )