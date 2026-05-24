#include "engine.h"
const int INF = 1e9;

static const int PIECE_VALUE[6] = { 100, 500, 320, 330, 900, 20000 };

int evaluate(const Board& board) {
    int score = 0;
    for (int piece = 0; piece < 6; piece++) {
        score += __builtin_popcountll(board.pieces[WHITE][piece]) * PIECE_VALUE[piece];
        score -= __builtin_popcountll(board.pieces[BLACK][piece]) * PIECE_VALUE[piece];
    }
    return board.sideToMove == WHITE ? score : -score;
}
int negamax(Board& board, int depth){
    if(depth == 0) return evaluate(board);
    std::vector<Move> moves = generateLegalMoves(board, board.sideToMove);
    if (moves.empty()) return isInCheck(board.sideToMove, board) ? -INF : 0;
    int best = -INF;
    for (Move &m : moves){
        makeMove(board, m);
        int eval = -negamax(board, depth - 1);
        unmakeMove(board, m);
        best = std::max(best, eval);
    }
    return best;
}
Move bestMove(Board& board, int depth){
    std::vector<Move> moves = generateLegalMoves(board, board.sideToMove);
    Move bmove = moves[0];
    int best = -INF;
    for (Move &m : moves){
        makeMove(board, m);
        int eval = -negamax(board, depth - 1);
        unmakeMove(board, m);
        if(eval > best){
            best = eval; bmove = m;
        }
    }
    return bmove;
}