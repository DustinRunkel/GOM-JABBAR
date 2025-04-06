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
// TestManager Class
// Used to run tests and aquire results

#pragma once
#include <map>
#include "GJ_Types.hpp"
#include "GJ_Test.hpp"
#include "GJ_Config.hpp"

namespace GJ
{
class TestManager
{
    private:
        Config * config_ = nullptr;
        TestManager(){};
    public:

        TestManager(Config * config) : config_ (config){}
        std::map<unsigned int, ITest*> tests_;
        std::vector<unsigned int> tests_to_run;

        /*
        * Run all tests in the specifed group
        */
        void run( void )
        {
            config_->critical_section_start();
            for( auto t : tests_to_run )
            {
                //Will run test and ALL args sent in
                tests_[t]->run();
                /*
                * If we do not pass the gate, we halt
                */
                if( !tests_[t]->gate() )
                {
                    //TODO: add a message here that sends a failure message to the desktop
                    break;
                }
                config_->critical_recovery();
            }
            config_->critical_section_end();

        }

        //Todo: restrict this to the arg types of the function under test
        template<typename ...T>
        void add_args_to_testid(unsigned int testId, T&&... args)
        {
            tests_[testId]->add_args(std::forward(args...));
        }
};
}