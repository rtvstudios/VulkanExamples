#ifndef RLogger_h
#define RLogger_h

#include <string>
#include <sstream>

namespace rvkfw {

class RLogger {
public:
    enum Type {
        Info,
        Warning,
        Error,
        Debug
    };

    std::string typeToString(Type type) const {
        switch (type) {
            case Warning:
                return  "WARNING";
            case Error:
                return  "ERROR  ";
            case Debug:
                return  "DEBUG  ";
            default:
                return  "INFO   ";
        }
    }

    RLogger(Type type, const std::string &tag);
    ~RLogger();

    std::stringstream& stream() {
        return mStream;
    }

private:
    std::stringstream mStream;
};

}

#define DO_LOG(type, tag, message) { rvkfw::RLogger(type, tag).stream() << message; }

#define LOG_INFO(tag, message) DO_LOG(rvkfw::RLogger::Type::Info, tag, message)
#define LOG_WARN(tag, message) DO_LOG(rvkfw::RLogger::Type::Warning, tag, message)
#define LOG_ERROR(tag, message) DO_LOG(rvkfw::RLogger::Type::Error, tag, message)

#ifdef DEBUG
#define LOG_DEBUG(tag, message) DO_LOG(rvkfw::RLogger::Type::Debug, tag, message)
#else
#define LOG_DEBUG(tag, message)
#endif


#endif

