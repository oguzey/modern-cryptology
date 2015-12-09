#ifndef MODERN_CRYPTOLOGY_LOGGER_H
#define MODERN_CRYPTOLOGY_LOGGER_H

#include "spdlog/include/spdlog/spdlog.h"
namespace mc {
    class Logger {
    private:
        std::shared_ptr<spdlog::logger> console_logger;

        Logger() {
            std::cout << "Called constructor of Logger" << std::endl;
            try {
                this->console_logger = spdlog::stdout_logger_st("console");
                spdlog::set_level(spdlog::level::debug);
            }
            catch (const spdlog::spdlog_ex &ex) {
                std::cout << "Log failed: " << ex.what() << std::endl;
                exit(-1);
            }
        }

        Logger(Logger const &) = delete;
        void operator=(Logger const &) = delete;

    public:
        ~Logger() {
            std::cout << "Called destructor of Logger" << std::endl;
        }

        static Logger &get_logger() {
            static Logger new_loger;
            return new_loger;
        }

        void log(std::string a_str) {
            this->console_logger->debug("{}", a_str);
        }

        template<typename... Args>
        void debug(const char *fmt, const Args &... args) {
            this->console_logger->debug(fmt, args...);
        }

        template<typename... Args>
        void info(const char *fmt, const Args &... args) {
            this->console_logger->info(fmt, args...);
        }

        template<typename... Args>
        void warn(const char *fmt, const Args &... args) {
            this->console_logger->warn(fmt, args...);
        }

        template<typename... Args>
        void critical(const char *fmt, const Args &... args) {
            this->console_logger->critical(fmt, args...);
            exit(-1);
        }
    };
}


extern mc::Logger& logger;

#endif //MODERN_CRYPTOLOGY_LOGGER_H
