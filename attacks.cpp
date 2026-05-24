#include "attacks.h"

U64 KNIGHT_ATTACK[64];
U64 KING_ATTACK[64];

U64 knightAttack(Coord square) {
    U64 bsq = 1ULL << square;
    return ((bsq << 17) & NOT_H_FILE)  |
           ((bsq << 10) & NOT_GH_FILE) |
           ((bsq >>  6) & NOT_GH_FILE) |
           ((bsq >> 15) & NOT_H_FILE)  |
           ((bsq >> 17) & NOT_A_FILE)  |
           ((bsq >> 10) & NOT_AB_FILE) |
           ((bsq <<  6) & NOT_AB_FILE) |
           ((bsq << 15) & NOT_A_FILE);
}

void generateKnightAttacks() {
    for (int i = 0; i < 64; i++)
        KNIGHT_ATTACK[i] = knightAttack(Coord(i));
}

U64 kingAttack(Coord square) {
    U64 bsq = 1ULL << square;
    U64 attacks = 0;
    attacks |= (bsq >> 8);
    attacks |= (bsq << 8);
    attacks |= ((bsq >> 1) & NOT_A_FILE);
    attacks |= ((bsq << 1) & NOT_H_FILE);
    attacks |= ((bsq >> 9) & NOT_H_FILE);
    attacks |= ((bsq << 9) & NOT_A_FILE);
    attacks |= ((bsq >> 7) & NOT_H_FILE);
    attacks |= ((bsq << 7) & NOT_A_FILE);
    return attacks;
}

void generateKingAttacks() {
    for (int i = 0; i < 64; i++)
        KING_ATTACK[i] = kingAttack(Coord(i));
}

U64 rayAttacks(U64 origin, U64 occupied, int shift, U64 mask) {
    U64 ray = origin;
    U64 attacks = 0;
    while (ray & mask) {
        ray = (shift > 0) ? (ray << shift) : (ray >> -shift);
        attacks |= ray;
        if (ray & occupied) break;
    }
    return attacks;
}

U64 rookAttack(Coord square, U64 occupied) {
    U64 bsq = 1ULL << square;
    return rayAttacks(bsq, occupied, 8,  ~0ULL)     // up
         | rayAttacks(bsq, occupied, -8, ~0ULL)     // down
         | rayAttacks(bsq, occupied, 1,  NOT_H_FILE) // right
         | rayAttacks(bsq, occupied, -1, NOT_A_FILE); // left
}

U64 bishopAttack(Coord square, U64 occupied)
{
    U64 bsq = 1ULL << square;
    U64 attacks = 0;
    attacks |= rayAttacks(bsq, occupied, 9, NOT_H_FILE);
    attacks |= rayAttacks(bsq, occupied, -9, NOT_A_FILE);
    attacks |= rayAttacks(bsq, occupied, 7, NOT_H_FILE);
    attacks |= rayAttacks(bsq, occupied, -7, NOT_A_FILE);
    return attacks;    
}

U64 queenAttack(Coord square, U64 occupied){
    return rookAttack(square, occupied)
     | bishopAttack(square, occupied);
}

//pawn pushes push all pawns of one color together
U64 pawnPushOne(Color color, U64 pawns, U64 occupied){
    return color == WHITE ? (pawns << 8) & ~occupied 
                          : (pawns >> 8) & ~occupied;
}
U64 pawnPushTwo(Color color, U64 pawns, U64 occupied){
    if(color == WHITE){
        U64 rank4 = 0x00000000FF000000ULL;
        return pawnPushOne(color, pawns, occupied) << 8 & ~occupied & rank4;
    }
    //black
    U64 rank5 = 0x000000FF00000000ULL;
    return pawnPushOne(color, pawns, occupied) >> 8 & ~occupied & rank5;
}

U64 pawnAttackLeft(Color color, U64 pawns) {
    return color == WHITE ? (pawns << 9) & NOT_A_FILE
                          : (pawns >> 7) & NOT_A_FILE;
}
U64 pawnAttackRight(Color color, U64 pawns) {
    return color == WHITE ? (pawns << 7) & NOT_H_FILE
                          : (pawns >> 9) & NOT_H_FILE;
}
U64 pawnAttack(Color color, U64 pawns) {
    return pawnAttackLeft(color, pawns) | pawnAttackRight(color, pawns);
}