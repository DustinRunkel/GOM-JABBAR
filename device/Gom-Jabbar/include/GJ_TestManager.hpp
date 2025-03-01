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
#include <vector>
#include "GJ_Types.hpp"
#include "GJ_Test.hpp"

namespace GJ
{
    class TestManager
    {
        private:
        public:
            std::vector<Test> tests_;

            /*
            * Called to allow recovery from a critical state
            * Will do nothing unless overriden by the user
            */
            virtual void critical_recovery(void){};

            /*
            * Run all tests in the specifed group
            */
            void run( void )
            {
                for( auto t : tests_)
                {
                    t.run(/* TODO: Arguments need to be provided from GUI message*/);
                    /*
                    * If we pass the gate, we run the next test
                    */
                    if( t.gate() )
                    {
                        continue;
                    }
                    /*
                    * We should only be down here if a critical failure happened
                    */
                    critical_recovery();
                    break;
                }
            }
    };
}