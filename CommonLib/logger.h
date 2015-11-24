#ifndef MODERN_CRYPTOLOGY_LOGGER_H
#define MODERN_CRYPTOLOGY_LOGGER_H

#include <bits/shared_ptr.h>

class Logger {
public:
    std::shared_ptr get_logger();
private:
    std::shared_ptr console_logger;
    Logger();


};


#endif //MODERN_CRYPTOLOGY_LOGGER_H
