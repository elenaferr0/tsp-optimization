#ifndef LOGGER_H
#define LOGGER_H

#include <chrono>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>

using namespace std;

class Logger {
public:
  enum class Level { DEBUG = 0, INFO = 1, WARN = 2, ERROR = 3, FATAL = 4 };

private:
  Level min_level;
  mutex log_mutex;
  string logger_label;

  string level_to_string(Level level) const;
  string get_current_timestamp() const;
  void write_log(Level level, const string &message);

public:
  explicit Logger(Level min_level = Level::INFO, const string &label = "");

  ~Logger() = default;

  void set_min_level(Level level);
  Level get_min_level() const;
  void set_label(const string &label);
  string get_label() const;

  void debug(const string &message);
  void info(const string &message);
  void warn(const string &message);
  void error(const string &message);
  void fatal(const string &message);

  void log(Level level, const string &message);
};

#endif
