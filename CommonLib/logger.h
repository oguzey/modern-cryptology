#ifndef MODERN_CRYPTOLOGY_LOGGER_H
#define MODERN_CRYPTOLOGY_LOGGER_H

#include <bits/shared_ptr.h>
#include "spdlog/include/spdlog/logger.h"

namespace mclogger {
    class Logger {
    public:
        static Logger& get_logger();
        template <typename... Args> void debug(const char* fmt, const Args&... args);
        template <typename... Args> void info(const char* fmt, const Args&... args);
        template <typename... Args> void warn(const char* fmt, const Args&... args);
        template <typename... Args> void critical(const char* fmt, const Args&... args);
    private:
        std::shared_ptr<spdlog::logger> console_logger;
        Logger();
        ~Logger();
        Logger(Logger const&)           = delete;
        void operator=(Logger const&)   = delete;
    };
}


#endif //MODERN_CRYPTOLOGY_LOGGER_H
