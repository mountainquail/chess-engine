#include "board.h"

U64 sumColor(Color color, const Board& board) {
    U64 result = 0;
    for (int piece = 0; piece < 6; piece++)
        result |= board.pieces[color][piece];
    return result;
}

U64 sumPieces(const Board& board) {
    U64 result = 0;
    for (int c = 0; c < 2; c++)
        for (int piece = 0; piece < 6; piece++)
            result |= board.pieces[c][piece];
    return result;
}

char pieceAt(const Board& board, int square) {
    U64 num = 1ULL << square;
    char pieces[2][6] = {{'p', 'r', 'n', 'b', 'q', 'k'}, {'P', 'R', 'N', 'B', 'Q', 'K'}};
    for (int color = WHITE; color <= BLACK; color++)
        for (int piece = PAWN; piece <= KING; piece++)
            if (board.pieces[color][piece] & num)
                return pieces[color][piece];
    return '.';
}

void printBoard(const Board& board) {
    for (int row = 7; row >= 0; row--) {
        std::cout << row + 1 << ' ';
        for (int col = 0; col < 8; col++)
            std::cout << pieceAt(board, row * 8 + col) << ' ';
        std::cout << '\n';
    }
    std::cout << "\n  a b c d e f g h\n";
}

Board defaultPosition() {
    Board board;
    board.pieces[WHITE][ROOK]   = (1ULL << A1) | (1ULL << H1);
    board.pieces[WHITE][KNIGHT] = (1ULL << B1) | (1ULL << G1);
    board.pieces[WHITE][BISHOP] = (1ULL << F1) | (1ULL << C1);
    board.pieces[WHITE][KING]   = (1ULL << E1);
    board.pieces[WHITE][QUEEN]  = (1ULL << D1);
    board.pieces[WHITE][PAWN]   = (1ULL << A2) | (1ULL << B2) | (1ULL << C2) | (1ULL << D2) |
                                   (1ULL << E2) | (1ULL << F2) | (1ULL << G2) | (1ULL << H2);
    board.pieces[BLACK][ROOK]   = (1ULL << A8) | (1ULL << H8);
    board.pieces[BLACK][KNIGHT] = (1ULL << B8) | (1ULL << G8);
    board.pieces[BLACK][BISHOP] = (1ULL << F8) | (1ULL << C8);
    board.pieces[BLACK][KING]   = (1ULL << E8);
    board.pieces[BLACK][QUEEN]  = (1ULL << D8);
    board.pieces[BLACK][PAWN]   = (1ULL << A7) | (1ULL << B7) | (1ULL << C7) | (1ULL << D7) |
                                   (1ULL << E7) | (1ULL << F7) | (1ULL << G7) | (1ULL << H7);
    board.castlingRights = WHITEKING | WHITEQUEEN | BLACKQUEEN | BLACKKING;
    board.sideToMove     = WHITE;
    board.enPassant      = NOCOORD;
    board.halfmoveClock  = 0;
    board.fullmoveNumber = 1;
    return board;
}
