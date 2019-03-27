#ifndef RUN_PROCESS_UTILITY_HPP
#define RUN_PROCESS_UTILITY_HPP

#include "headers.hpp"
#include "time_utility.hpp"

namespace run_process
{
    inline void with_fixed_time(std::function<bool(void)> run_function, std::chrono::milliseconds fixed_interval) 
    {
        auto start_time = Clock::now();
        bool is_OK = run_function();
        auto process_time = Clock::now() - start_time;
        if(process_time < fixed_interval)
            std::this_thread::sleep_for(fixed_interval - process_time);

        while(is_OK) 
        {
            start_time = Clock::now();
            is_OK = run_function();
            process_time = Clock::now() - start_time;
            if(process_time < fixed_interval)
                std::this_thread::sleep_for(fixed_interval - process_time);
        }
    }
}


#endif