#include "Logger.h"
#include <fstream>
#include <ctime>

void Logger::log(const std::string& action, const std::string& details, const std::string& result) {
    std::ofstream f("data/logs.txt", std::ios::app);
    if (!f) return; // گارد عدم دسترسی به فایل
    
    std::time_t now = std::time(nullptr);
    char* timeStr = std::ctime(&now);
    
    std::string timeSanitized(timeStr);
    if (!timeSanitized.empty() && timeSanitized.back() == '\n') {
        timeSanitized.pop_back();
    }

    f << "[" << timeSanitized << "] "
      << "ACTION: " << action << " | "
      << "DETAILS: " << details << " | "
      << "RESULT: " << result << "\n"
      << "--------------------------------------------------------\n";
}