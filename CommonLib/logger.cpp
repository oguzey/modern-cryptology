#include "logger.h"
#include "spdlog/include/spdlog/spdlog.h"


Logger::Logger() {
    try {
        this->console_logger = spdlog::stdout_logger_st("console");
    }
    catch (const spdlog::spdlog_ex& ex) {
        std::cout << "Log failed: " << ex.what() << std::endl;
        exit(-1);
    }
}

std::shared_ptr Logger::get_logger() {
    if (this->console_logger == nullptr) {
        return Logger();
    }
    return std::shared_ptr();
}
