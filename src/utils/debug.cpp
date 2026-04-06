#include "utils/debug.h"
#include <iostream>

void Board::print_board(const Position& pos)
{
    for (int rank = 7; rank >= 0; rank--)
    {
        // Print each rank label.
        std::cout << rank + 1 << " ";

        for (int file = 0; file <= 7; file++)
        {
            int square = rank * 8 + file;
            bool found = false;

            for (int i = 0; i < 12; i++)
            {
                if (BitBoard::get_bit(pos.bitboards[i], square))
                {
                    found = true;
                    switch (i)
                    {
                        case 0:
                            std::cout << " P";
                            break;
                        case 1:
                            std::cout << " N";
                            break;
                        case 2:
                            std::cout << " B";
                            break;
                        case 3:
                            std::cout << " R";
                            break;
                        case 4:
                            std::cout << " Q";
                            break;
                        case 5:
                            std::cout << " K";
                            break;
                        case 6:
                            std::cout << " p";
                            break;
                        case 7:
                            std::cout << " n";
                            break;
                        case 8:
                            std::cout << " b";
                            break;
                        case 9:
                            std::cout << " r";
                            break;
                        case 10:
                            std::cout << " q";
                            break;
                        case 11:
                            std::cout << " k";
                            break;
                    }
                    break;
                }
            }

            if (!found)
            {
                std::cout << " .";
            }
        }

        // Print Newline.
        std::cout << "\n";
    }

    // Print File labels.
    std::cout << "   a b c d e f g h";
}