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
// Abstraction for task support, based on freertos tasks

#pragma once
#include "GJ_Config.hpp"
#include "GJ_Message.hpp"

namespace GJ 
{
    typedef struct TaskParams
    {
        Config * config_;
        MessageHandler * msgHandler;
    } msgTaskParams;

    void MessageHandlerTask(void * params)
    {
        TaskParams * p = static_cast<msgTaskParams*>(params);

        for(;;)
        {
            /*
            * Poll the incoming message for readiness
            * Could register a custom interupt, but that
            * requires wayy more user config than I would like
            * to configure
            */
            p->msgHandler->queue_incoming();
            //config_->msgHandler->send_all_outgoing();
        }
    }
}