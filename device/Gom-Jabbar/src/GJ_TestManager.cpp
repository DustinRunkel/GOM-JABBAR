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

namespace GJ
{
    TestManager::TestManager(Config * config) : config_ (config) {};

    void TestManager::run()
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

    template<typename ...T>
    void TestManager::add_args_to_testid(unsigned int testId, T&&... args);
    {
        tests_[testId]->add_args(std::forward(args...));
    }

    void TestManager::populate_test_vector()
    {//read json msg and populate the list of tests

    }
}