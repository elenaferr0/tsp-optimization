#ifndef TIME_LOGGER_H
#define TIME_LOGGER_H
#include <iostream>
#include <chrono>
#include <string>
#include <iomanip> // For std::setprecision

using std::chrono::duration;
using std::chrono::high_resolution_clock;
using namespace std;

typedef high_resolution_clock::time_point time_pt;

class TimeLogger {
    time_pt *start_time;
    time_pt *last_tick_time;
    string label;

public:
    explicit TimeLogger(string  label);
    TimeLogger(const TimeLogger&);
    TimeLogger& operator=(const TimeLogger&);

    ~TimeLogger();

    void start();

    void tick(const string &msg) const;

    void log_total_time(const string &msg) const;
};
#endif //TIME_LOGGER_H
