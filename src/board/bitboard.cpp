#include "board/bitboard.h"

// Make sure the trailing zero function is called for the correct compiler & system.
int BitBoard::bit_scan_forward(uint64_t bb)
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

// Pop Least significant bit (rightmost 1 in the bitboard).
// Also known as the piece on the "lowest index" square.
// Allows for quicker iteration of moves for every piece.
int BitBoard::pop_lsb(uint64_t& bb)
{
    // Step 1. Isolate LSB using two's complement.
    uint64_t lsb = bb & -bb;

    // Step 2. Find the index of the LSB.
    // Counts how many zeros are after the LSB before the first 1.
    int square = BitBoard::bit_scan_forward(lsb);

    // Step 3. Remove the LSB from the original bitboard.
    // Flips all bits from the LSB to the right.
    bb &= bb - 1;

    // Return index of the LSB.
    return square;
}