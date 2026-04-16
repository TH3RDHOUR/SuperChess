#include <iostream>
#include "board/position.h"
#include "board/fen.h"
#include "utils/debug.h"
#include "board/bitboard.h"

int main()
{
    Board::Position pos;
    std::string initial_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    bool ran = Board::parse_fen(pos, initial_fen);

    if (ran)
    {
        std::cout << "Fen Parser ran successfully." << "\n";
    }

    Board::print_board(pos);

    std::string fen = Board::generate_fen(pos);
    std::cout << "Initial fen:   " << initial_fen << std::endl;
    std::cout << "Generated fen: " << fen << std::endl;
    return 0;
}