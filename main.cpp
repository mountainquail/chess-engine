#include "board.h"
#include "attacks.h"

Board board;

int main() {
    generateKnightAttacks();
    generateKingAttacks();
    board = defaultPosition();
    printBoard(board);
    return 0;
}
