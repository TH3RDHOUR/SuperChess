#ifndef FEN_H
#define FEN_H

#include "board/bitboard.h"
#include "position.h"
#include <string>
#include <cctype>

namespace Board
{
    bool parse_fen(Board::Position& pos, const std::string& fen);   
}

#endif