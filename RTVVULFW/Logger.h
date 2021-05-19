#ifndef Logger_h
#define Logger_h

#include <string>
#include <sstream>

namespace rtvvulfw {

class Logger {
public:
    enum Type {
        Info,
        Warning,
        Error,
        Debug
    };

    std::string typeToString(Type type) const {
        switch (type) {
            case Info:
                return  "INFO";
            case Warning:
                return  "WARNING";
            case Error:
                return  "ERROR";
            case Debug:
                return  "DEBUG";
        }
    }

    Logger(Type type, const std::string &tag);
    ~Logger();

    std::stringstream& stream() {
        return mStream;
    }

private:
    std::stringstream mStream;
};

}

#define DO_LOG(type, tag, message) { Logger(type, tag).stream() << message; }

#define LOG_INFO(tag, message) DO_LOG(Logger::Type::Info, tag, message)
#define LOG_WARN(tag, message) DO_LOG(Logger::Type::Warning, tag, message)
#define LOG_ERROR(tag, message) DO_LOG(Logger::Type::Error, tag, message)

#ifdef DEBUG
#define LOG_DEBUG(tag, message) DO_LOG(Logger::Type::Debug, tag, message)
#else
#define LOG_DEBUG(tag, message)
#endif


#endif

