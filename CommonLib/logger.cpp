#include <stdlib.h>
#include "logger.h"
#include "spdlog/include/spdlog/spdlog.h"

namespace mclogger {

    Logger::Logger() {
        std::cout<<"Called constructor of Logger" << std::endl;
        try {
            this->console_logger = spdlog::stdout_logger_st("console");
        }
        catch (const spdlog::spdlog_ex &ex) {
            std::cout << "Log failed: " << ex.what() << std::endl;
            exit(-1);
        }
    }

    Logger::~Logger() {
        std::cout<<"Called destructor of Logger" << std::endl;
    }

    static Logger& Logger::get_logger() {
        std::cout<<"Called get_logger of Logger" << std::endl;
        static Logger logger;
        return logger;
    }

    template <typename... Args> void Logger::debug(const char* fmt, const Args&... args) {
            this->console_logger->debug(fmt, args...);
    }

    template <typename... Args> void Logger::info(const char* fmt, const Args&... args) {
        this->console_logger->info(fmt, args...);
    }

    template <typename... Args> void Logger::warn(const char* fmt, const Args&... args) {
        this->console_logger->warn(fmt, args...);
    }

    template <typename... Args> void Logger::critical(const char* fmt, const Args&... args) {
        this->console_logger->critical(fmt, args...);
        exit(-1);
    }
}