#ifndef ROUNDKEY_H
#define ROUNDKEY_H

#include <stdint.h>
#include <array>
#include <vector>

#define AMOUNT_SUBKEYS 7
#define AMOUNT_SUBKEYS_IN_BYTES AMOUNT_SUBKEYS * 2

enum Direction {
    UNKNOWN,
    FORWARD,
    BACKWARD
};

class RoundKey {
public:
    static const int amount_subkeys = AMOUNT_SUBKEYS;
    RoundKey();
    RoundKey(Direction a_path);
    RoundKey(const std::vector<uint16_t> &a_subkeys, Direction path);
    static const int get_amount_subkesy() { return amount_subkeys; };
    void set_direction(const Direction a_path) { this->path = a_path; };
    void generate_new();
    void set_key(const std::array<uint16_t, amount_subkeys> &a_subkeys);
    void reset_position();
    uint16_t next_subkey();

private:
    std::array<uint16_t, amount_subkeys> subkeys;
    size_t pos_current_subkey;
    Direction path;
};



#endif // ROUNDKEY_H
