#include "board/position.h"

void Board::clear_position(Position& pos)
{
    // Reset the bitboards.
    for (int i = 0; i < 12; i++)
        pos.bitboards[i] = 0ULL;

    // Reset the occupancies.
    for (int i = 0; i < 3; i++)
        pos.occupancies[i] = 0ULL;

    // Reset all other parameters.
    pos.side_to_move = WHITE;
    pos.castling_rights = 0;
    pos.en_passant_square = -1;

    pos.halfmove_clock = 0;
    pos.fullmove_number = 1;
}

void Board::update_occupancies(Position& pos)
{
    pos.occupancies[WHITE] =
        pos.bitboards[WhitePawn] |
        pos.bitboards[WhiteKnight] |
        pos.bitboards[WhiteBishop] |
        pos.bitboards[WhiteRook] |
        pos.bitboards[WhiteQueen] |
        pos.bitboards[WhiteKing];

    pos.occupancies[BLACK] =
        pos.bitboards[BlackPawn] |
        pos.bitboards[BlackKnight] |
        pos.bitboards[BlackBishop] |
        pos.bitboards[BlackRook] |
        pos.bitboards[BlackQueen] |
        pos.bitboards[BlackKing];

    pos.occupancies[BOTH] =
        pos.occupancies[WHITE] |
        pos.occupancies[BLACK];
}