#ifndef NIMBLEHELPERS_H
#define NIMBLEHELPERS_H
#include <memory>
#include <string>

std::string GetTimestamp();

namespace strings {
  bool ends_with(const std::string& value, const std::string& ending);

  template<typename... Args>
  std::string formatString(std::string format, Args&&... args) {
    int size = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;	// Get Required Size
    if (size <= 0) return "";

    std::unique_ptr<char[]> buffer(new char[size]);
    std::snprintf(buffer.get(), size, format.c_str(), args...);
    return std::string(buffer.get(), buffer.get() + size - 1);
  }
}

namespace logging {
  void log(const std::string& type, const std::string& msg);
  template<typename... Args>
  void loginfo(const std::string& msg, Args&&... args) {
    log("INFO", strings::formatString(msg, args...));
  }

  template<typename... Args>
  void logwarning(const std::string& msg, Args&&... args) {
    log("WARNING", strings::formatString(msg, args...));
  }

  template<typename... Args>
  void logerror(const std::string& msg, Args&&... args) {
    log("ERROR", strings::formatString(msg, args...));
  }

  void startlogging(const std::string&path, const std::string& filename);
  void stoplogging();
  void backuplog(const std::string& path, bool crash = false);
  void deletelog(const std::string& path);
}

#endif