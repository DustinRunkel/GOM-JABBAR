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
// User config

#pragma once

#include "GJ_TaskSupport.hpp"
#include <string>
#include <cstring>

#include "pico/stdlib.h"

/*
* Users must provide a class named "Config" in gj_config.so
* It must inheirit from GJ::Config and override required methods
* You can add some helper methods to the config, if you so choose,
* Or allocate some memory here.
*/
class Config : public GJ::Config
{
    public:
        std::string * raw_input;
        bool INPUT_READY = false;

        Config() : raw_input(new std::string()){}
        /*
        * Send the string over protocol of choice (Serial in our case)
        */
        void send(const char * str) override
        {
            printf("%s", str);
        }
        /*
        * Expose the string to GOM-JABBAR libraries
        */
        const char * receive() override
        {
            char * str;
            gets(str);
            return str;
        }
        /*
        * Allows the GJ_Messaging layer to know when your input is ready
        */
        inline bool input_ready() override
        {
            printf("input ready -- config\n");
            return INPUT_READY;
        }
};