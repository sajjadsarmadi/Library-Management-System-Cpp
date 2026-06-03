#pragma once
#include <string>
class Logger{
public:
    static void log(const std::string& action,const std::string& details,const std::string& result);
};
