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

namespace GJ
{
class TestManager
{
    private:
    public:
        std::map<unsigned int, ITest*> tests_;
        std::vector<unsigned int> tests_to_run;

        /*
        * Called to allow recovery from a critical state
        * Will do nothing unless overriden by the user
        */
        virtual void critical_recovery(void){};

        /*
        * Overloadable instance that allows the disable of interupts during tests
        * Leave this alone to leave interupts enabled
        */
        virtual void critical_section_start(void){};
        virtual void critical_section_end(void){};

        /*
        * Run all tests in the specifed group
        */
        void run( void )
        {
            critical_section_start();
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
                critical_recovery();
            }
            critical_section_end();
            
        }

        //Todo: restrict this to the arg types of the function under test
        template<typename ...T>
        void add_args_to_testid(unsigned int testId, T&&... args)
        {
            tests_[testId]->add_args(std::forward(args...));
        }
};
}