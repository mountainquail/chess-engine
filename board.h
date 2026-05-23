#pragma once
#include "types.h"

U64 sumColor(Color color, const Board& board);
U64 sumPieces(const Board& board);
char pieceAt(const Board& board, int square);
void printBoard(const Board& board);
Board defaultPosition();
