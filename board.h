#pragma once
#include "types.h"

U64 sumColor(Color color, const Board& board);
U64 sumPieces(const Board& board);
std::pair<Color, Piece> pieceAt(const Board& board, int square);
char pieceAtChar(const Board& board, int square);
void printBoard(const Board& board);
Board defaultPosition();
void makeMove(Board& board, Move& move);
void unmakeMove(Board& board, const Move& move);
