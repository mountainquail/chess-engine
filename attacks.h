#pragma once
#include "types.h"

extern U64 KNIGHT_ATTACK[64];
extern U64 KING_ATTACK[64];

U64 knightAttack(Coord square);
void generateKnightAttacks();
U64 kingAttack(Coord square);
void generateKingAttacks();
U64 rookAttack(Coord square, U64 occupied);
U64 bishopAttack(Coord square, U64 occupied);
U64 queenAttack(Coord square, U64 occupied);
U64 pawnPushOne(Color color, U64 pawns, U64 occupied);
U64 pawnPushTwo(Color color, U64 pawns, U64 occupied);
U64 pawnAttackLeft(Color color, U64 pawns);
U64 pawnAttackRight(Color color, U64 pawns);
U64 pawnAttack(Color color, U64 pawns);
