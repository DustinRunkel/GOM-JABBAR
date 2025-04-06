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
// Test template class

#include "GJ_Test.hpp"

namespace GJ
{
    Test::Test(std::function<return_t(arg_t...)> func, TestSeverity severity)
    {
        func_ = func;
        severity_ = severity;
        result_ = TestResult::NONE;
        t_info = typeid(this);
        r_info = typeid(return_t);
    };

    Test::run()
    {
        try
        { //we assume they want to run all arguments passed in
            for( auto args : fargs )
            {
                result_data_.push_back(func_(std::forward<arg_t...>(args)));
            }
            
        }
        catch(const std::exception& e)
        {
            return TestResult::ERROR;
        }
        
    };

    Test::add_args(arg_t... args)
    {
        fargs.push_back(std::tuple(args...));
    }

    Test::clear_results()
    {
        result_data_.clear();
    }

    Test::clear_args()
    {
        fargs.clear();
    }
} //end namespace