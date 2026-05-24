#pragma once
#include <iostream>
typedef unsigned long long U64;

enum Piece {
    PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING, NOPIECE
};
enum Color {
    WHITE, BLACK, NOCOLOR
};
enum Coord {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8, NOCOORD
};
enum Castle {
    WHITEKING  = 1 << 0,
    WHITEQUEEN = 1 << 1,
    BLACKKING  = 1 << 2,
    BLACKQUEEN = 1 << 3
};

struct Board {
    U64 pieces[2][6] = {};
    Color sideToMove = WHITE;
    int castlingRights = 0;
    int enPassant = NOCOORD;
    int halfmoveClock = 0;
    int fullmoveNumber = 1;
    U64 pieceSum[3] = {};
};

struct Move {
    Coord from;
    Coord to;
    Piece piece;
    Color color;
    bool castle;
    bool enPassant;
    Piece captured;
    Piece promo;
    int prevEnPassant      = 0;
    int prevCastlingRights = 0;
    int prevHalfmoveClock  = 0;

    Move() = default;
    Move(Coord from, Coord to, Piece piece, Color color,
         bool castle, bool enPassant, Piece captured, Piece promo)
        : from(from), to(to), piece(piece), color(color),
          castle(castle), enPassant(enPassant), captured(captured), promo(promo) {}
};

struct MoveList {
    Move moves[256];
    int count = 0;
    void add(const Move& m) { moves[count++] = m; }
};

extern const U64 NOT_A_FILE;
extern const U64 NOT_H_FILE;
extern const U64 NOT_AB_FILE;
extern const U64 NOT_GH_FILE;

