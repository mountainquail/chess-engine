#include "board.h"
#include "attacks.h"
#include "movegen.h"
#include <iostream>

Board board;

int main() {
    generateKnightAttacks();
    generateKingAttacks();
    board = defaultPosition();

    std::cout << "=== Starting position ===\n";
    printBoard(board);

    // e2-e4
    Move e2e4{E2, E4, PAWN, WHITE, false, false, NOPIECE, NOPIECE};
    makeMove(board, e2e4);
    std::cout << "\n=== After 1. e4 ===\n";
    printBoard(board);
    std::cout << "White in check: " << isInCheck(WHITE, board) << '\n';

    // e7-e5
    Move e7e5{E7, E5, PAWN, BLACK, false, false, NOPIECE, NOPIECE};
    makeMove(board, e7e5);
    std::cout << "\n=== After 1... e5 ===\n";
    printBoard(board);

    // undo both moves
    unmakeMove(board, e7e5);
    unmakeMove(board, e2e4);
    std::cout << "\n=== After undoing both moves ===\n";
    printBoard(board);
    std::cout << "En passant square: " << board.enPassant << " (should be 64 = NOCOORD)\n";

    return 0;
}
