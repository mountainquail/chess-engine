#include "board.h"
#include "attacks.h"
#include "movegen.h"
#include "tests.h"
#include "engine.h"
#include <iostream>
using namespace std;

string moveToString(const Move& m) {
    string s;
    s += ('a' + m.from % 8);
    s += ('1' + m.from / 8);
    s += ('a' + m.to % 8);
    s += ('1' + m.to / 8);
    return s;
}
int main() {
    generateKnightAttacks();
    generateKingAttacks();
    runTests();
    Board board = defaultPosition();
    printBoard(board);
    while(true){
        vector<Move> legal = generateLegalMoves(board, board.sideToMove);
        if(legal.empty()){
            if(isInCheck(board.sideToMove, board)) cout << "checkmate";
            else cout << "stalemate";
            break;
        }
        Move m = bestMove(board, 4);
        cout << moveToString(m) << "\n";
        makeMove(board, m);
        printBoard(board);
    }
    return 0;
}

