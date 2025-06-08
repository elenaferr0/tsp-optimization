#include "utils/logger.h"

string Logger::level_to_string(Level level) const {
    switch (level) {
        case Level::DEBUG: return "DEBUG";
        case Level::INFO:  return "INFO ";
        case Level::WARN:  return "WARN ";
        case Level::ERROR: return "ERROR";
        case Level::FATAL: return "FATAL";
        default: return "UNKNOWN";
    }
}

string Logger::get_current_timestamp() const {
    auto now = chrono::system_clock::now();
    auto time_t = chrono::system_clock::to_time_t(now);
    auto ms = chrono::duration_cast<chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    stringstream ss;
    ss << put_time(localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    ss << '.' << setfill('0') << setw(3) << ms.count();
    return ss.str();
}

void Logger::write_log(Level level, const string& message) {
    if (level < min_level) {
        return; // Skip logging if below minimum level
    }

    lock_guard<mutex> lock(log_mutex);

    stringstream ss;
    ss << "[" << get_current_timestamp() << "] [" << level_to_string(level) << "]";

    if (!logger_label.empty()) {
        ss << " [" << logger_label << "]";
    }

    ss << " " << message;

    cout << ss.str() << endl;
}

// Constructor
Logger::Logger(Level min_level, const string& label)
    : min_level(min_level), logger_label(label) {
}

// Set minimum log level
void Logger::set_min_level(Level level) {
    lock_guard<mutex> lock(log_mutex);
    min_level = level;
}

// Get current minimum log level
Logger::Level Logger::get_min_level() const {
    return min_level;
}

// Set logger label
void Logger::set_label(const string& label) {
    lock_guard<mutex> lock(log_mutex);
    logger_label = label;
}

// Get logger label
string Logger::get_label() const {
    return logger_label;
}

// Logging methods
void Logger::debug(const string& message) {
    write_log(Level::DEBUG, message);
}

void Logger::info(const string& message) {
    write_log(Level::INFO, message);
}

void Logger::warn(const string& message) {
    write_log(Level::WARN, message);
}

void Logger::error(const string& message) {
    write_log(Level::ERROR, message);
}

void Logger::fatal(const string& message) {
    write_log(Level::FATAL, message);
}

void Logger::log(Level level, const string& message) {
    write_log(level, message);
}
