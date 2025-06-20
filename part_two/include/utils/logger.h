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
  enum class Level { TRACE = 0, DEBUG = 1, INFO = 2, WARN = 3, ERROR = 4 };

private:
  Level min_level;
  string logger_label;

  string level_to_string(Level level) const;
  string get_current_timestamp() const;
  void write_log(Level level, const string &message) const;

public:
  explicit Logger(Level min_level = Level::INFO, const string &label = "");

  ~Logger() = default;

  void set_min_level(Level level);
  Level get_min_level() const;
  void set_label(const string &label);
  string get_label() const;

  void trace(const string &message) const;
  void debug(const string &message) const;
  void info(const string &message) const;
  void warn(const string &message) const;
  void error(const string &message) const;

  void log(Level level, const string &message) const;
};

#endif
