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
# Where our interface library lives
set( gj_includes "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING "GJ include path")

set(target gom_jabbar)

add_library(${target} INTERFACE )

target_link_libraries( ${target} INTERFACE yyjson gj_config)

target_include_directories( ${target} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(${target} INTERFACE $<$<COMPILE_LANGUAGE:CXX>: -frtti>)