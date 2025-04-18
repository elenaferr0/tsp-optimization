#include "utils/time_logger.h"

TimeLogger::TimeLogger() : start_time(nullptr), last_tick_time(nullptr) {
}

TimeLogger::~TimeLogger() {
    delete start_time;
    delete last_tick_time;
}

void TimeLogger::start() {
    start_time = new time_pt(high_resolution_clock::now());
    last_tick_time = new time_pt(high_resolution_clock::now());
}

void TimeLogger::tick(const std::string &msg) const {
    if (!start_time || !last_tick_time) {
        std::cerr << "TimeLogger not started. Call start() before tick()." << std::endl;
        return;
    }
    const auto now = high_resolution_clock::now();

    const duration<double> elapsed = now - *last_tick_time;
    std::cout << std::fixed << std::setprecision(6)
            << "[" << msg << "] "
            << "Elapsed time: " << elapsed.count() << "s"
            << std::endl;

    *last_tick_time = now;
}

void TimeLogger::log_total_time(const string &msg) const {
    const auto end_time = high_resolution_clock::now();
    const duration<double> total_elapsed = end_time - *start_time;
    std::cout << std::fixed << std::setprecision(6)
            << "[" << msg << "] "
            << "Total elapsed time: " << total_elapsed.count() << "s"
            << std::endl;
}
