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

# Also make library compilation target here
add_library( gom-jabbar INTERFACE )

target_include_directories( gom-jabbar INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )