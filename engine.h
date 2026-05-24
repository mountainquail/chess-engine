#pragma once
#include "board.h"
#include "movegen.h"

int evaluate(const Board& board);
int negamax(Board& board, int depth);
Move bestMove(Board& board, int depth);