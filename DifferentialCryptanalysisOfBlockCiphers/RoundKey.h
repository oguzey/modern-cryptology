#ifndef ROUNDKEY_H
#define ROUNDKEY_H

#include <stdint.h>
#include <array>

//#define AMOUNT_SUBKEYS			7
//#define AMOUNT_SUBKEYS_IN_BYTES		AMOUNT_SUBKEYS * sizeof(uint16_t)

//typedef struct round_key RoundKey;
//
//RoundKey* round_key_create(uint16_t *a_start_value);
//
//void round_key_destroy(RoundKey *a_rkey);
//
//void round_key_generate_new(RoundKey *a_rkey);
//
//void round_key_set_key(RoundKey *a_rkey, uint16_t *a_new_keys);
//
//void round_key_reset_using_to_start(RoundKey *a_rkey);
//
//void round_key_reset_using_to_end(RoundKey *a_rkey);
//
//uint16_t round_key_next_subkey(RoundKey *a_rkey);

namespace mc {

    enum Direction {
        UNKNOWN,
        FORWARD,
        BACKWARD
    };

    class RoundKey {
    public:
        static const int amount_subkeys = 7;
        RoundKey();
        RoundKey(Direction a_path);
        RoundKey(const std::array<uint16_t, amount_subkeys> &subkeys, Direction path);
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
}


#endif // ROUNDKEY_H
