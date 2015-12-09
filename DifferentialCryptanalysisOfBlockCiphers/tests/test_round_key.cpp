#include "../RoundKey.h"
#include <array>
#include <assert.h>
#include "../../CommonLib/logger.h"

mc::Logger& logger = mc::Logger::get_logger();

int main (int argc, char **argv) {
    logger.info("Test Round key with {} subkeys\n", mc::RoundKey::get_amount_subkesy());
    std::array<uint16_t, 7> init_keys = {1, 2, 3, 4, 5, 6, 7};

    mc::RoundKey key(init_keys, mc::FORWARD);

    key.reset_position();
    for (int i = 0; i < 7; ++i) {
        assert(key.next_subkey() == i + 1);
    }

    key.set_direction(mc::BACKWARD);
    key.reset_position();

    for(int i = 7; i > 0; --i) {
        assert(key.next_subkey() == i);
    }
    return 0;
}