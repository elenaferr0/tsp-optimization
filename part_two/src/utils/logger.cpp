#include "utils/logger.h"

string Logger::level_to_string(Level level) const {
    switch (level) {
        case Level::TRACE:
            return "TRACE";
        case Level::DEBUG:
            return "DEBUG";
        case Level::INFO:
            return "INFO ";
        case Level::WARN:
            return "WARN ";
        case Level::ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}

string Logger::get_current_timestamp() const {
    const auto now = chrono::system_clock::now();
    const auto time_t = chrono::system_clock::to_time_t(now);
    const auto ms =
            chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()) %
            1000;

    stringstream ss;
    ss << put_time(localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    ss << '.' << setfill('0') << setw(3) << ms.count();
    return ss.str();
}

void Logger::write_log(const Level level, const string &message) const {
    if (level < min_level) {
        return; // Skip logging if below minimum level
    }

    stringstream ss;
    ss << "[" << get_current_timestamp() << "] [" << level_to_string(level)
            << "]";

    if (!logger_label.empty()) {
        ss << " [" << logger_label << "]";
    }

    ss << " " << message;

    cout << ss.str() << endl;
}

Logger::Logger(const Level min_level, const string &label)
    : min_level(min_level), logger_label(label) {
}

void Logger::set_min_level(const Level level) {
    min_level = level;
}

Logger::Level Logger::get_min_level() const { return min_level; }

void Logger::set_label(const string &label) {
    logger_label = label;
}

string Logger::get_label() const { return logger_label; }

void Logger::trace(const string &message) const { write_log(Level::TRACE, message); }

void Logger::debug(const string &message) const { write_log(Level::DEBUG, message); }

void Logger::info(const string &message) const { write_log(Level::INFO, message); }

void Logger::warn(const string &message) const { write_log(Level::WARN, message); }

void Logger::error(const string &message) const { write_log(Level::ERROR, message); }

void Logger::log(const Level level, const string &message) const {
    write_log(level, message);
}
