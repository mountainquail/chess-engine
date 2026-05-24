#pragma once
#include "types.h"
#include "attacks.h"
#include "board.h"
#include <vector>

bool isInCheck(Color color, const Board& board);
std::vector<Move> generateLegalMoves(Board& board, Color color);
