#include "board/fen.h"

// Parse the half move section.
static bool parse_halfmove(Board::Position& pos, const std::string& half)
{
    // Verify length.
    if (half.length() < 1)
        return false;

    // Loop through all halfmove ints in string.
    int value = 0;
    for (int i = 0; i < half.size(); i++)
    {
        if (std::isdigit(half[i]))
            value = value * 10 + (half[i] - '0');
        else
            return false;
    }

    pos.halfmove_clock = value;
    return true;
}

// Parse the full move section.
static bool parse_fullmove(Board::Position& pos, const std::string& full)
{
    // Verify length.
    if (full.length() < 1)
        return false;

    // Loop through all fullmove ints in string.
    int value = 0;
    for (int i = 0; i < full.size(); i++)
    {
        if (std::isdigit(full[i]))
            value = value * 10 + (full[i] - '0');
        else
            return false;
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
static bool parse_en_passant(Board::Position& pos, const std::string& enp)
{
    // Verify length.
    if (enp.size() < 1 || enp.size() > 2)
        return false;

    // No en Passant
    if (enp[0] == '-' && enp.size() == 1)
    {
        pos.en_passant_square = -1;
    }

    // En Passant Square Determination.
    else if (enp.size() == 2 && enp[0] >= 'a' && enp[0] <= 'h')
    {
        // Must be either rank 3 (black pawn) & 6 (white pawn).
        if (enp[1] == '3' || enp[1] == '6')
        {
            // Get file & rank of en Passant square.
            int file_pas = enp[0] - 'a';
            int rank_pas = enp[1] - '1';

            // Calculate and assign en Passant square.
            pos.en_passant_square = rank_pas * 8 + file_pas;
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

static bool parse_side_to_move(Board::Position& pos, const std::string& stm)
{
    // Verify length.
    if (stm.size() != 1)
        return false;

    // Check for side_to_move value.
    if (stm[0] == 'w')
    {
        pos.side_to_move = WHITE;
    }
    else if (stm[0] == 'b')
    {
        pos.side_to_move = BLACK;
    }
    else
        return false;

    return true;
}

static bool parse_castling_rights(Board::Position& pos, const std::string& cast)
{
    // Initialize castling rights.
    pos.castling_rights = 0;

    // Verify length.
    if (cast.length() < 1)
        return false;

    // If char is a dash.
    if (cast[0] == '-')
    {
        // MUST be length of 1 if a dash.
        if (cast.length() == 1)
            pos.castling_rights = 0;
        else
            return false;
    }

    else
    {
        // Loop over castling rights section.
        for (int i = 0; i < cast.size(); i++)
        {
            // If char is a letter.
            if (cast[i] == 'K')
            {
                pos.castling_rights |= 1 << 0;
            }
            else if (cast[i] == 'Q')
            {
                pos.castling_rights |= 1 << 1;
            }
            else if (cast[i] == 'k')
            {
                pos.castling_rights |= 1 << 2;
            }
            else if (cast[i] == 'q')
            {
                pos.castling_rights |= 1 << 3;
            }
            else
                return false;
        }
    }

    return true;
}

static bool parse_board(Board::Position& pos, const std::string& board)
{
    // Initialize rank (rows) & file (columns).
    int rank = 7; // rank 8 -> 1.
    int file = 0; // file a -> h.

    // Loop over the fen string board section.
    for (int i = 0; i < board.size(); i++)
    {
        // Out of Bounds.
        if (file > 8 || rank < 0)
            return false;

        char c = board[i];

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
            if (offset < 1 || offset > 8)
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

    return true;
}

// Split a FEN string by spaces and return a vector of strings.
static std::vector<std::string> split(const std::string& fen)
{
    // Instantiate vector to hold the split fen.
    std::vector<std::string> tokens;
    std::istringstream iss(fen);
    std::string token;

    // Loop over fen separating by spaces.
    while (iss >> token)
    {
        tokens.push_back(token);
    }

    return tokens;
}

// Main function that parses the entire FEN string.
bool Board::parse_fen(Position& pos, const std::string& fen)
{
    Board::clear_position(pos);

    // Split the FEN string.
    auto tokens = split(fen);

    // Make sure there is the correct number of sections.
    if (tokens.size() != 6)
    {
        return false;
    }

    // Parse the board section of the FEN string.
    if (!parse_board(pos, tokens[0]))
        return false;

    // Parse the side to move section.
    if (!parse_side_to_move(pos, tokens[1]))
        return false;

    // Parse the castling rights section.
    if (!parse_castling_rights(pos, tokens[2]))
        return false;

    // Parse the en Passant section.
    if (!parse_en_passant(pos, tokens[3]))
        return false;

    // Parse the halfmove section.
    if (!parse_halfmove(pos, tokens[4]))
        return false;

    // Parse the full move section.
    if (!parse_fullmove(pos, tokens[5]))
        return false;

    Board::update_occupancies(pos);

    return true;
}