// ________  ________  _____ ______                             
// |\   ____\|\   __  \|\   _ \  _   \                           
// \ \  \___|\ \  \|\  \ \  \\\__\ \  \                          
//  \ \  \  __\ \  \\\  \ \  \\|__| \  \                         
//   \ \  \|\  \ \  \\\  \ \  \    \ \  \                        
//    \ \_______\ \_______\ \__\    \ \__\                       
//     \|_______|\|_______|\|__|_____\|__|________  ________     
//    |\  \|\   __  \|\   __  \|\   __  \|\   __  \|\   __  \    
//    \ \  \ \  \|\  \ \  \|\ /\ \  \|\ /\ \  \|\  \ \  \|\  \   
//  __ \ \  \ \   __  \ \   __  \ \   __  \ \   __  \ \   _  _\  
// |\  \\_\  \ \  \ \  \ \  \|\  \ \  \|\  \ \  \ \  \ \  \\  \| 
// \ \________\ \__\ \__\ \_______\ \_______\ \__\ \__\ \__\\ _\ 
//  \|________|\|__|\|__|\|_______|\|_______|\|__|\|__|\|__|\|__|
// Library user configuration object
#pragma once

namespace GJ
{
    class Config 
    {
        public:
            Config(){};
            /*
            * User defined string sending protocol
            */
            virtual void send(const char * str) = 0;
            /*
            * User defined string reception protocol
            */
            virtual const char * receive() = 0;
            /*
            * Overloadable instance that allows the disable of interupts during tests
            * Leave this alone to leave interupts enabled
            */
            virtual void critical_section_start(void){};
            virtual void critical_section_end(void){};
            /*
            * Called to allow recovery from a critical state
            * Will do nothing unless overriden by the user
            */
            virtual void critical_recovery(void){};

            virtual bool input_ready() = 0;
    };
}