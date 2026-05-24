#include "tests.h"
#include <iostream>

uint64_t perft(Board& board, Color color, int depth) {
    if (depth == 0) return 1;
    auto moves = generateLegalMoves(board, color);
    if (depth == 1) return moves.size();
    uint64_t nodes = 0;
    for (auto& move : moves) {
        makeMove(board, move);
        nodes += perft(board, Color(1 - color), depth - 1);
        unmakeMove(board, move);
    }
    return nodes;
}

static void check(const char* name, uint64_t got, uint64_t expected) {
    if (got == expected)
        std::cout << "[PASS] " << name << ": " << got << "\n";
    else
        std::cout << "[FAIL] " << name << ": got " << got << ", expected " << expected << "\n";
}

void runTests() {
    std::cout << "=== Perft: starting position ===\n";
    // well-known node counts from the starting position
    Board board = defaultPosition();
    check("depth 1",  perft(board, WHITE, 1), 20);
    check("depth 2",  perft(board, WHITE, 2), 400);
    check("depth 3",  perft(board, WHITE, 3), 8902);
    check("depth 4",  perft(board, WHITE, 4), 197281);

    std::cout << "\n=== isInCheck ===\n";
    // fool's mate — black delivers checkmate on move 2
    Board fm = defaultPosition();
    Move f3{F2, F3, PAWN, WHITE, false, false, NOPIECE, NOPIECE};
    Move e5{E7, E5, PAWN, BLACK, false, false, NOPIECE, NOPIECE};
    Move g4{G2, G4, PAWN, WHITE, false, false, NOPIECE, NOPIECE};
    Move qh4{D8, H4, QUEEN, BLACK, false, false, NOPIECE, NOPIECE};
    makeMove(fm, f3); makeMove(fm, e5);
    makeMove(fm, g4); makeMove(fm, qh4);
    check("white in checkmate (0 legal moves)", generateLegalMoves(fm, WHITE).size(), 0);
    check("white is in check", isInCheck(WHITE, fm), 1);

    std::cout << "\n=== castling rights lost after king move ===\n";
    Board cr = defaultPosition();
    // clear the pieces between king and rook manually so we can move the king
    cr.pieces[WHITE][KNIGHT] = 0;
    cr.pieces[WHITE][BISHOP] = 0;
    Move ke2{E1, E2, KING, WHITE, false, false, NOPIECE, NOPIECE};
    makeMove(cr, ke2);
    check("castling rights cleared after king moves",
          (cr.castlingRights & (WHITEKING | WHITEQUEEN)), 0);
}
