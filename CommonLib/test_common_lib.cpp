#include "logger.h"

static void test_logger() {
    std::string info("Test logger implementation.");
    mc::Logger& logger = mc::Logger::get_logger();
    logger.warn("{}", info);
    logger.debug("{}", info);
    logger.info("{}", info);
    logger.critical("{}", info);
}


int main(int argc, char **argv) {
    std::cout << "Start tests" << std::endl;
    test_logger();
    return 0;
}