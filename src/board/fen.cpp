#include "board/fen.h"

// TODO: Change fen parser to split the fen string first by spaces then parse it.

// Check we are on a space and not at the end of the FEN string.
static bool check_space_and_advance(int& iterator, const std::string& fen)
{
    // Verify we are not at the end of the FEN string & we are on a space.
    if (iterator >= fen.size() || fen[iterator] != ' ')
        return false;

    // Skip the space
    iterator++;

    return true;
}

// Parse the half move section.
static bool parse_halfmove(Board::Position& pos, int& iterator, const std::string& fen)
{
    // Bounds checker.
    if (iterator >= fen.size() || !std::isdigit(fen[iterator]))
        return false;

    // Loop through all halfmove ints in string.
    int value = 0;
    while (iterator < fen.size() && std::isdigit(fen[iterator]))
    {
        value = value * 10 + (fen[iterator] - '0');
        iterator++;
    }

    pos.halfmove_clock = value;
    return true;
}

// Parse the full move section.
static bool parse_fullmove(Board::Position& pos, int& iterator, const std::string& fen)
{
    // Bounds checker.
    if (iterator >= fen.size() || !std::isdigit(fen[iterator]))
        return false;

    // Loop through all fullmove ints in string.
    int value = 0;
    while (iterator < fen.size() && std::isdigit(fen[iterator]))
    {
        value = value * 10 + (fen[iterator] - '0');
        iterator++;
    }

    // Value is correct for full move.
    if (value >= 1)
    {
        pos.fullmove_number = value;
        return true;
    }
    
    return false;
}

// Parse the en Passant section.
static bool parse_en_passant(Board::Position& pos, int& iterator, const std::string& fen)
{
    // Bounds checker.
    if (iterator >= fen.size())
        return false;

    // No en Passant
    if (fen[iterator] == '-')
    {
        pos.en_passant_square = -1;
        iterator++;
    }

    // En Passant Square Determination.
    else if (fen[iterator] >= 'a' && fen[iterator] <= 'h')
    {
        // Bounds checker.
        if (iterator + 1 >= fen.size())
            return false;

        // Must be either rank 3 (black pawn) & 6 (white pawn).
        if (fen[iterator + 1] == '3' || fen[iterator + 1] == '6')
        {
            // Get file & rank of en Passant square.
            int file_pas = fen[iterator] - 'a';
            int rank_pas = fen[iterator + 1] - '1';

            // Calculate and assign en Passant square.
            pos.en_passant_square = rank_pas * 8 + file_pas;

            // Skip over en Passant square characters.
            iterator += 2;
        }
        // Invalid rank for en Passant square.
        else
        {
            return false;
        }
    }

    // Invalid en Passant section.
    else
        return false;

    // Ran successfully.
    return true;
}

static bool parse_side_to_move(Board::Position& pos, int& iterator, const std::string& fen)
{
    // Bounds checker.
    if (iterator >= fen.size())
        return false;

    // Check for side_to_move value.
    if (fen[iterator] == 'w')
    {
        pos.side_to_move = WHITE;
        iterator++;
    }
    else if (fen[iterator] == 'b')
    {
        pos.side_to_move = BLACK;
        iterator++;
    }
    else
        return false;

    return true;
}

static bool parse_castling_rights(Board::Position& pos, int& iterator, const std::string& fen)
{
    // Bounds checker.
    if (iterator >= fen.size())
        return false;

    // Castling rights loop.
    while (iterator < fen.size() && fen[iterator] != ' ')
    {
        // Bounds checker for next character.
        if (fen[iterator + 1] >= fen.size())
            return false;

        // If char is a dash.
        if (fen[iterator] == '-')
        {
            pos.castling_rights = 0;
            iterator++;

            break;
        }

        // If char is a letter.
        if (fen[iterator] == 'K')
        {
            pos.castling_rights |= 1 << 0;
        }
        else if (fen[iterator] == 'Q')
        {
            pos.castling_rights |= 1 << 1;
        }
        else if (fen[iterator] == 'k')
        {
            pos.castling_rights |= 1 << 2;
        }
        else if (fen[iterator] == 'q')
        {
            pos.castling_rights |= 1 << 3;
        }
        else
            return false;

        iterator++;
    }

    return true;
}

// Main function that parses the entire FEN string.
bool Board::parse_fen(Position& pos, const std::string& fen)
{
    Board::clear_position(pos);
    pos.castling_rights = 0;

    // Initialize rank (rows) & file (columns).
    int rank = 7; // rank 8 -> 1.
    int file = 0; // file a -> h.
    int i = 0; // Iterator.

    // Loop over the fen string board section.
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

    // Final Validation of files & ranks in board section.
    // Final board must be an 8x8.
    if (file != 8 || rank != 0)
        return false;

    // Parse the remainder of the FEN string.
    // Verify we are not at the end of the FEN string & we are on a space.
    if (!check_space_and_advance(i, fen))
        return false;

    // Parse the side to move section.
    if (!parse_side_to_move(pos, i, fen))
        return false;
    // Verify we are not at the end of the FEN string & we are on a space.
    if (!check_space_and_advance(i, fen))
        return false;

    // Parse the castling rights section.
    if (!parse_castling_rights(pos, i, fen))
        return false;
    // Verify we are not at the end of the FEN string & we are on a space.
    if (!check_space_and_advance(i, fen))
        return false;

    // Parse the en Passant section.
    if (!parse_en_passant(pos, i, fen))
        return false;
    // Verify we are not at the end of the FEN string & we are on a space.
    if (!check_space_and_advance(i, fen))
        return false;

    // Parse the halfmove section.
    if (!parse_halfmove(pos, i, fen))
        return false;
    // Verify we are not at the end of the FEN string & we are on a space.
    if (!check_space_and_advance(i, fen))
        return false;

    // Parse the full move section.
    if (!parse_fullmove(pos, i, fen))
        return false;
    // Make sure we have reached the end of the string.
    if (i != fen.size())
        return false;

    Board::update_occupancies(pos);

    return true;
}