#include "board/bitboard.h"

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

    return square;
}