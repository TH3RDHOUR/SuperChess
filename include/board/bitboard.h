#ifndef BITBOARD_H
#define BITBOARD_H

#include <cstdint> // uint64_t.
#include <intrin.h> // _BitScanForward64.

namespace BitBoard
{
    // Returns Unsigned Long Long shifted left by square bits.
    // This is a bitboard w/ only the bit for that square set to 1.
    // Used for set/clear/get_bit to do bitwise operations on bitboard.
    inline uint64_t mask(int square) { return 1ULL << square; }

    // True if piece is at square.
    inline bool get_bit(uint64_t bb, int square) { return bb & mask(square); }

    // OR the bitboard with the mask using square.
    inline void set_bit(uint64_t& bb, int square) { bb |= mask(square); }

    // AND the bitboard with the inverted mask using square.
    // With (~) it will flip all bits (all 1s except the target bit which is zero).
    inline void clear_bit(uint64_t& bb, int square) { bb &= ~mask(square); }

    int bit_scan_forward(uint64_t bb);
    int pop_lsb(uint64_t& bb);
}

#endif