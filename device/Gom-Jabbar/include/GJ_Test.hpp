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
#include <typeinfo>
#include <deque>
#include <tuple>
#include <vector>

namespace GJ
{
class ITest 
{
    public:
        /*
        * Generic functions that our test class overrides
        * To make working with our tests easier
        * We are able to make a container of ITest
        * But not Test, as it is an incomplete type
        */
        virtual TestResult  run()           = 0;
        virtual inline bool gate()          = 0;
        virtual void *      get_result()    = 0;
        virtual void        add_args(...)   = 0;
        virtual void        clear_args()    = 0;
        virtual void        clear_results() = 0;
        //Type information for test and return type
        std::type_info *    t_info;
        std::type_info *    r_info;
};

template<typename return_t, typename... arg_t >
class Test : ITest
{
    private:
        std::function<return_t(arg_t...)> func_;
        TestSeverity severity_;
        TestResult result_;
        std::vector<return_t> result_data_;
        /*
        * Store all user arguments in a deque of tuples for later use
        */
        std::deque<std::tuple<arg_t...>> fargs;
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
            t_info = typeid(this);
            r_info = typeid(return_t);
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
        TestResult run( ) override
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
        inline bool gate() override
        {
            return !(severity_ == CRITICAL && result_ == ERROR);
        };
        /*
        * result data is private, because we do not want any manipulation of the results
        */
        inline constexpr return_t * get_result() override
        {
            return std::move(result_data_);
        };
        /*
        * Allow users to set arguments at runtime
        * TestManager should be able to call this and pack
        * with message data
        */
        void add_args(arg_t... args) override
        {
            fargs.push_back(std::tuple(args...));
        };
        /*
        * Clears result data
        */
        void clear_results() override
        {
            result_data_.clear();
        }
        /*
        * Clears args
        */
        void clear_args() override
        {
            fargs.clear();
        }
};
}