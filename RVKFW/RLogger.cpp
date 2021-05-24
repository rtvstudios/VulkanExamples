
#include "RLogger.h"
#include <iostream>
#include <ctime>

namespace rvkfw {

RLogger::RLogger(Type type, const std::string &tag) {
    std::time_t time = std::time(0);
    char loc[sizeof("2021-03-01T10:44:10Z")];
    strftime(loc, sizeof(loc), "%F %T", localtime(&time));
    mStream << loc << " | " << typeToString(type) << " | " << tag << " | ";
}

RLogger::~RLogger() {
    std::cout << mStream.str() << std::endl;
}

}
