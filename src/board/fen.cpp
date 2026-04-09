#include "board/fen.h"

// Parse a fen string to update all bitboards with pieces on the game board.
bool Board::parse_fen(Position& pos, const std::string& fen)
{
    Board::clear_position(pos);
    pos.castling_rights = 0;

    // Initialize rank (rows) & file (columns).
    int rank = 7; // rank 8 -> 1.
    int file = 0; // file a -> h.
    int i = 0; // Iterator.

    // Loop over the fen string.
    for (; i < fen.size(); i++)
    {
        // Out of Bounds.
        if (file > 8 || rank < 0)
            return false;

        char c = fen[i];

        // End of placement.
        if (c == ' ')
        {
            break;
        }

        // Move to next rank
        if (c == '/')
        {
            // Invalid FEN, incorrect number of squares.
            if (file != 8)
                return false;

            // Move down a rank, increment the number of ranks, & reset file.
            rank--;
            file = 0;
        }

        // Empty squares (amount of c)
        else if (std::isdigit(c))
        {
            // Type cast to int.
            int offset = (c - '0');
            
            // Digit is too large, incorrect FEN.
            if (file + offset > 8)
                return false;

            file += offset;
        }

        else
        {
            // Placement of piece, Square index.
            int square = rank * 8 + file;

            int piece = -1; // In case of invalid character.

            // Determine the piece to place.
            switch (c)
            {
                case 'P': piece = WhitePawn; break;
                case 'N': piece = WhiteKnight; break;
                case 'B': piece = WhiteBishop; break;
                case 'R': piece = WhiteRook; break;
                case 'Q': piece = WhiteQueen; break;
                case 'K': piece = WhiteKing; break;

                case 'p': piece = BlackPawn; break;
                case 'n': piece = BlackKnight; break;
                case 'b': piece = BlackBishop; break;
                case 'r': piece = BlackRook; break;
                case 'q': piece = BlackQueen; break;
                case 'k': piece = BlackKing; break;
            }

            // Could not find character.
            if (piece == -1)
                return false;

            // Set the piece at position square.
            BitBoard::set_bit(pos.bitboards[piece], square);

            // Move to the next file.
            file++;
        }
    }

    // Final Validation of files & ranks.
    // Final board must be an 8x8.
    if (file != 8 || rank != 0)
        return false;

    // Parse the remainder of the FEN string.
    // Verify we are not at the end of the FEN string & we are on a space.
    if (i >= fen.size() || fen[i] != ' ')
        return false;

    // Skip the space.
    i++;

    // Bounds checker.
    if (i >= fen.size())
        return false;

    // Check for side_to_move value.
    if (fen[i] == 'w')
    {
        pos.side_to_move = WHITE;
        i++;
    }
    else if (fen[i] == 'b')
    {
        pos.side_to_move = BLACK;
        i++;
    }
    else
        return false;

    // Verify we are not at the end of the FEN string & we are on a space.
    if (i >= fen.size() || fen[i] != ' ')
        return false;

    // Skip the space
    i++;

    // Bounds checker.
    if (i >= fen.size())
        return false;

    // Castling rights loop.
    while (i < fen.size() && fen[i] != ' ')
    {
        // If char is a dash.
        if (fen[i] == '-')
        {
            pos.castling_rights = 0;
            i++;

            // Make sure next char is space for valid FEN.
            if (i >= fen.size() || fen[i] != ' ')
                return false;

            break;
        }
        // If char is a letter.
        if (fen[i] == 'K')
            pos.castling_rights |= 1 << 0;
        else if (fen[i] == 'Q')
            pos.castling_rights |= 1 << 1;
        else if (fen[i] == 'k')
            pos.castling_rights |= 1 << 2;
        else if (fen[i] == 'q')
            pos.castling_rights |= 1 << 3;
        else
            return false;

        i++;
    }

    // Verify we are not at the end of the FEN string & we are on a space.
    if (i >= fen.size() || fen[i] != ' ')
        return false;

    // Skip the space
    i++;

    Board::update_occupancies(pos);

    return true;
}