#ifndef POSITION_H
#define POSITION_H

#include <cstdint>

enum Piece
{
    WhitePawn   = 0,
    WhiteKnight = 1,
    WhiteBishop = 2,
    WhiteRook   = 3,
    WhiteQueen  = 4,
    WhiteKing   = 5,
    BlackPawn   = 6,
    BlackKnight = 7,
    BlackBishop = 8,
    BlackRook   = 9,
    BlackQueen  = 10,
    BlackKing   = 11
};

enum Color
{
    WHITE = 0,
    BLACK = 1,
    BOTH  = 2
};

struct Position
{
    uint64_t bitboards[12]; // All piece types [piece]
    uint64_t occupancies[3]; // [white, black, both]

    int side_to_move;
    int castling_rights;
    int en_passant_square;

    int halfmove_clock;
    int fullmove_number;
};

#endif