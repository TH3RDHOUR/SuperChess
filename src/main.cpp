#include <iostream>
#include "board/position.h"
#include "board/fen.h"
#include "utils/debug.h"
#include "board/bitboard.h"

int main()
{
    Board::Position pos;

    bool ran = Board::parse_fen(pos, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    if (ran)
    {
        std::cout << "Fen Parser ran successfully." << "\n";
    }

    Board::print_board(pos);
    return 0;
}