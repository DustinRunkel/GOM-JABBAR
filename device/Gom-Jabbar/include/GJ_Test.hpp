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
#pragma once
#include "GJ_Types.hpp"
#include <functional>

namespace GJ
{
    class ITest 
    {
        virtual TestResult run();
    }
    template<typename return_t, typename... arg_t >
    class Test : ITest
    {
        private:
            std::function<return_t(arg_t...)> func_;
            TestSeverity severity_;
            TestResult result_;
            return_t result_data_; 
            /*
            * force users to use public constructor, otherwise UB
            */
            Test();

        public: 
            /*
            * Must take a function and a severity level
            * Even if you don't care, about severity we need something to
            * compare against in the default version of gate()
            */
            Test(std::function<return_t(arg_t...)> func, TestSeverity severity)
            {
                func_ = func;
                severity_ = severity;
                result_ = TestResult::NONE;
            };
            /*
            * Ok to default this
            */
            virtual ~Test() = default;
            /*
            * For now, I'm keeping this simple:
            * But run() should take whatever arguments to the function
            * the user defines, and nominal return values. 
            * 
            * This function is exposed for users to use outside of the TestManager, 
            * in case they want more granular control
            * 
            * Also, this will remain virtual, incase this logic does not suffice for the user
            * 
            */
            virtual TestResult run( arg_t... args, return_t nominal_value )
            {
                try
                {
                    result_data_ = func_(std::forward<arg_t...>(args...));
                    
                    //User needs to define '=' operator for function return type
                    //If you end up here, overload "==" for your class
                    //TODO: introduce compile time assert to check for overloaded "=="
                    if( nominal_value == result_data_ )
                    { // TODO: leaving here for now, but move this comparison to the GUI
                        return TestResult::OK; 
                    }
                    else
                    {
                        return TestResult::ERROR;
                    }
                }
                catch(const std::exception& e)
                {
                    return TestResult::ERROR;
                }
                
            }
            /*
            * gate is a run control item that decides behavior if a test passes or fails
            * it is virtual because the user can decide their own criteria for test failure
            * behavior
            * 
            * But here, if the test is critical, and it fails, then we return false and halt all tests
            * 
            * true = testing progresses as normal
            * false = testing halts entirely
            */
            virtual inline constexpr bool gate()
            {
                return !(severity_ == CRITICAL && result_ == ERROR);
            }
            /*
            * result data is private, because we do not want any manipulation of the results
            */
            inline constexpr return_t * get_result()
            {
                return std::move(result_data_);
            }
    };
}