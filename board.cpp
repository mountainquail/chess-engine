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

std::pair<Color, Piece> pieceAt(const Board& board, int square) {
    U64 num = 1ULL << square;
    for (int color = WHITE; color <= BLACK; color++)
        for (int piece = PAWN; piece <= KING; piece++)
            if (board.pieces[color][piece] & num)
                return {Color(color), Piece(piece)};
    return {NOCOLOR, NOPIECE};
}

char pieceAtChar(const Board& board, int square) {
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
            std::cout << pieceAtChar(board, row * 8 + col) << ' ';
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

void makeMove(Board& board, Move& move) {
    Color us   = move.color;
    Color them = Color(1 - us);
    U64 fromBit = 1ULL << move.from;
    U64 toBit   = 1ULL << move.to;

    move.prevEnPassant     = board.enPassant;
    move.prevCastlingRights = board.castlingRights;
    move.prevHalfmoveClock  = board.halfmoveClock;

    // move the piece
    board.pieces[us][move.piece] ^= fromBit | toBit;

    // remove captured piece
    if (move.captured != NOPIECE) {
        if (move.enPassant) {
            int epPawn = (us == WHITE) ? move.to - 8 : move.to + 8;
            board.pieces[them][PAWN] ^= 1ULL << epPawn;
        } else {
            board.pieces[them][move.captured] ^= toBit;
        }
    }

    // swap pawn for promoted piece
    if (move.promo != NOPIECE) {
        board.pieces[us][PAWN]       ^= toBit;
        board.pieces[us][move.promo] ^= toBit;
    }

    // move the rook when castling
    if (move.castle) {
        if      (move.to == G1) board.pieces[WHITE][ROOK] ^= (1ULL << H1) | (1ULL << F1);
        else if (move.to == C1) board.pieces[WHITE][ROOK] ^= (1ULL << A1) | (1ULL << D1);
        else if (move.to == G8) board.pieces[BLACK][ROOK] ^= (1ULL << H8) | (1ULL << F8);
        else if (move.to == C8) board.pieces[BLACK][ROOK] ^= (1ULL << A8) | (1ULL << D8);
    }

    // en passant square: set on double pawn push, else clear
    board.enPassant = NOCOORD;
    if (move.piece == PAWN && (move.to == move.from + 16 || move.from == move.to + 16))
        board.enPassant = (move.from + move.to) / 2;

    // castling rights
    if (move.piece == KING) {
        if (us == WHITE) board.castlingRights &= ~(WHITEKING | WHITEQUEEN);
        else             board.castlingRights &= ~(BLACKKING | BLACKQUEEN);
    }
    if (move.from == A1 || move.to == A1) board.castlingRights &= ~WHITEQUEEN;
    if (move.from == H1 || move.to == H1) board.castlingRights &= ~WHITEKING;
    if (move.from == A8 || move.to == A8) board.castlingRights &= ~BLACKQUEEN;
    if (move.from == H8 || move.to == H8) board.castlingRights &= ~BLACKKING;

    board.halfmoveClock = (move.piece == PAWN || move.captured != NOPIECE)
                          ? 0 : board.halfmoveClock + 1;
    if (us == BLACK) board.fullmoveNumber++;
    board.sideToMove = them;
}

void unmakeMove(Board& board, const Move& move) {
    Color us   = move.color;
    Color them = Color(1 - us);
    U64 fromBit = 1ULL << move.from;
    U64 toBit   = 1ULL << move.to;

    board.sideToMove      = us;
    board.enPassant       = move.prevEnPassant;
    board.castlingRights  = move.prevCastlingRights;
    board.halfmoveClock   = move.prevHalfmoveClock;
    if (us == BLACK) board.fullmoveNumber--;

    // undo promotion before moving piece back
    if (move.promo != NOPIECE) {
        board.pieces[us][move.promo] ^= toBit;
        board.pieces[us][PAWN]       ^= toBit;
    }

    // move piece back
    board.pieces[us][move.piece] ^= fromBit | toBit;

    // restore captured piece
    if (move.captured != NOPIECE) {
        if (move.enPassant) {
            int epPawn = (us == WHITE) ? move.to - 8 : move.to + 8;
            board.pieces[them][PAWN] ^= 1ULL << epPawn;
        } else {
            board.pieces[them][move.captured] ^= toBit;
        }
    }

    // move rook back when castling
    if (move.castle) {
        if      (move.to == G1) board.pieces[WHITE][ROOK] ^= (1ULL << H1) | (1ULL << F1);
        else if (move.to == C1) board.pieces[WHITE][ROOK] ^= (1ULL << A1) | (1ULL << D1);
        else if (move.to == G8) board.pieces[BLACK][ROOK] ^= (1ULL << H8) | (1ULL << F8);
        else if (move.to == C8) board.pieces[BLACK][ROOK] ^= (1ULL << A8) | (1ULL << D8);
    }
}
