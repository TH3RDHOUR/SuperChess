#ifndef BITBOARD_H
#define BITBOARD_H

#include <cstdint> // uint64_t.
#include <intrin.h> // _BitScanForward64.

namespace BitBoard
{
    // Returns Unsigned Long Long shifted left by square bits.
    // This is a bitboard w/ only the bit for that square set to 1.
    // Used for set/clear/get_bit.
    inline uint64_t mask(int square) { return 1ULL << square; }

    // True if piece is at square.
    inline bool get_bit(uint64_t bb, int square) { return bb & mask(square); }

    // OR the bit at position square.
    inline void set_bit(uint64_t& bb, int square) { bb |= mask(square); }

    // AND the bit at position square.
    inline void clear_bit(uint64_t& bb, int square) { bb &= ~mask(square); }

    // Make sure the trailing zero function is called for the correct compiler & system.
    inline int bit_scan_forward(uint64_t bb)
    {
    // MSVC specific (__builtin_ctzll doesn't exist).
    #if defined(_MSC_VER)
        unsigned long index;
        _BitScanForward64(&index, bb);
        return static_cast<int>(index);
    // GCC/Clang specific.
    #else
        return __builtin_ctzll(bb);
    #endif
    }

    int pop_lsb(uint64_t& bb);
}

#endif