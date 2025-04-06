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
# User level config


set(target gj_config)

add_library(${target} INTERFACE)

target_link_libraries(${target} INTERFACE pico_stdlib pico_cyw43_arch_none FreeRTOS-Kernel FreeRTOS-Kernel-Heap3 freertos_config gom_jabbar)

target_include_directories(${target} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})