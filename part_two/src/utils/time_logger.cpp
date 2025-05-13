#include "utils/time_logger.h"

#include <utility>

TimeLogger::TimeLogger(string label) : start_time(nullptr), last_tick_time(nullptr), label(std::move(label)) {
}

TimeLogger::TimeLogger(const TimeLogger &other) : label(other.label) {
    if (other.start_time) {
        start_time = new time_pt(*other.start_time);
    } else {
        start_time = nullptr;
    }

    if (other.last_tick_time) {
        last_tick_time = new time_pt(*other.last_tick_time);
    } else {
        last_tick_time = nullptr;
    }
}

TimeLogger &TimeLogger::operator=(const TimeLogger &other) {
    if (this == &other) {
        return *this;
    }

    delete start_time;
    delete last_tick_time;

    label = other.label;
    if (other.start_time) {
        start_time = new time_pt(*other.start_time);
    } else {
        start_time = nullptr;
    }

    if (other.last_tick_time) {
        last_tick_time = new time_pt(*other.last_tick_time);
    } else {
        last_tick_time = nullptr;
    }

    return *this;
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
            << "[" << label << "] "
            << msg
            << ": elapsed time " << elapsed.count() << "s"
            << std::endl;

    *last_tick_time = now;
}

void TimeLogger::log_total_time(const string &msg) const {
    const auto end_time = high_resolution_clock::now();
    const duration<double> total_elapsed = end_time - *start_time;
    std::cout << std::fixed << std::setprecision(6)
            << "[" << label << "] "
            << msg
            << " total elapsed time " << total_elapsed.count() << "s"
            << std::endl;
}
