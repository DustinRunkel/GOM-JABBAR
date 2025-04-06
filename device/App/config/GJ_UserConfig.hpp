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

#include "GJ_Config.hpp"
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
        std::string raw_input;
        bool INPUT_READY = false;
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
            
            if( !INPUT_READY )
            {
                return nullptr;
            }
            else 
            {
                INPUT_READY = false;
                char * retVal;
                memcpy(retVal, raw_input.c_str(), sizeof(raw_input.c_str()));
                raw_input.clear();
                return retVal;
            }
        }
        /*
        * Allows the GJ_Messaging layer to know when your input is ready
        */
        inline bool input_ready() override
        {
            return INPUT_READY;
        }
};

/*
* interupt handler to handle an incoming character from serial
* This is a helper function to the overridden receive() function
* which is used internally in the library
*/
void on_char_received(void * config)
{
    Config * _config = static_cast<Config*>(config);
    /*
    * RPI SDK wires stdin through UART or USB. We must collect each char 
    * into our buffer
    * 
    * One thing to note: this callback is called every time a char is in the buffer,
    * meaning we must read json objects one byte at a time, and assemble them in memory
    * over here.
    * 
    * This is a possible implementation of your application using the GOM-Jabbar json reader
    * The json reader only needs an incoming string from some protocol on your board
    * Eventually, this will be cofigured via an interface
    */
    char in_char = stdio_getchar_timeout_us( 10 );
    printf("%c", in_char);

    /*
    * If the json object has equal open/closed brackets,
    * the object is fully read-in. Therefore we reset the
    * reader by deserializing the json object into memory
    * and adding it to our backend message queue
    */
    if(in_char == '$')
    {
        _config->INPUT_READY = true;
    } 
    else 
    {
        _config->raw_input.push_back(in_char);
    }
}