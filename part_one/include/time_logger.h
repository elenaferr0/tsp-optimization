//
// Created by elena on 4/14/25.
//

#ifndef TIME_LOGGER_H
#define TIME_LOGGER_H
#include <iostream>
#include <chrono>
#include <string>
#include <iomanip> // For std::setprecision

using std::chrono::duration;
using std::chrono::high_resolution_clock;

typedef high_resolution_clock::time_point time_pt;

class time_logger
{
    time_pt start_time;
    time_pt last_tick_time;

public:
    time_logger() : start_time(high_resolution_clock::now()), last_tick_time(start_time)
    {
    }

    void tick(const std::string& msg)
    {
        const auto now = std::chrono::high_resolution_clock::now();
        const duration<double> elapsed_from_start = now - start_time;
        const duration<double> elapsed_from_last_tick = now - last_tick_time;

        std::cout << std::fixed << std::setprecision(6)
            << "[" << msg << "] "
            << "Elapsed time: " << elapsed_from_last_tick.count() << "s"
            << std::endl;

        last_tick_time = now;
    }

    void log_total_time(const std::string& msg) const
    {
        const auto end_time = std::chrono::high_resolution_clock::now();
        const duration<double> total_elapsed = end_time - start_time;
        std::cout << std::fixed << std::setprecision(6)
            << "[" << msg << "] "
            << "Total elapsed time: " << total_elapsed.count() << "s"
            << std::endl;
    }
};
#endif //TIME_LOGGER_H
