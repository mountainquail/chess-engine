#include "movegen.h"
#include <vector>

static bool isAttacked(int sq, Color them, const Board& board) {
    U64 occupied = sumPieces(board);
    return (KNIGHT_ATTACK[sq] & board.pieces[them][KNIGHT])
        || (KING_ATTACK[sq]   & board.pieces[them][KING])
        || (rookAttack(Coord(sq), occupied)   & (board.pieces[them][ROOK]   | board.pieces[them][QUEEN]))
        || (bishopAttack(Coord(sq), occupied) & (board.pieces[them][BISHOP] | board.pieces[them][QUEEN]))
        || (pawnAttack(Color(1 - them), 1ULL << sq) & board.pieces[them][PAWN]);
}

bool isInCheck(Color color, const Board& board) {
    int kingSq = __builtin_ctzll(board.pieces[color][KING]);
    return isAttacked(kingSq, Color(1 - color), board);
}
std::vector<Move> generateLegalMoves(Board& board, Color color)
{


    Color enemy = (color == WHITE) ? BLACK : WHITE;
    std::vector<Move> total;
    std::vector<Move> legal;
    const U64 teamPieces = sumColor(color, board);
    const U64 enemyPieces = sumColor(enemy, board);
    const U64 occupied = teamPieces | enemyPieces;


    //knight
    U64 knights = board.pieces[color][KNIGHT];
    while(knights > 0){
        //square index of first knight (ctzll - count trailing zeroes long long)
        int from = __builtin_ctzll(knights);
        //remove that knight
        knights &= knights - 1;
        //all possible moves on the board
        U64 locations = KNIGHT_ATTACK[from] & ~teamPieces;
        while (locations > 0){
            //same logic
            int to = __builtin_ctzll(locations);
            locations &= locations - 1;
            Piece captured = pieceAt(board, to).second;
            total.push_back(Move(
            Coord(from), Coord(to), KNIGHT, color, false, false, captured, NOPIECE));
        }
    }

    //king
    U64 kings = board.pieces[color][KING];
    //same logic
    while(kings > 0){
        int from = __builtin_ctzll(kings);
        kings &= kings - 1;
        U64 locations = KING_ATTACK[from] & ~teamPieces;
        while (locations > 0){
            int to = __builtin_ctzll(locations);
            locations &= locations - 1;
            Piece captured = pieceAt(board, to).second;
            total.push_back(Move(
            Coord(from), Coord(to), KING, color, false, false, captured, NOPIECE));
        }
    }

    //rook
    U64 rooks = board.pieces[color][ROOK];
    while(rooks > 0){
        int from = __builtin_ctzll(rooks);
        rooks &= rooks - 1;
        U64 rook_attack = rookAttack(Coord(from), occupied);
        U64 locations = rook_attack & ~teamPieces;
        while (locations > 0){
            int to = __builtin_ctzll(locations);
            locations &= locations - 1;
            Piece captured = pieceAt(board, to).second;
            total.push_back(Move(
            Coord(from), Coord(to), ROOK, color, false, false, captured, NOPIECE));
        }
    }


    //queen
    U64 queens = board.pieces[color][QUEEN];
    while(queens > 0){
        int from = __builtin_ctzll(queens);
        queens &= queens - 1;
        U64 queen_attack = queenAttack(Coord(from), occupied);
        U64 locations = queen_attack & ~teamPieces;
        while (locations > 0){
            int to = __builtin_ctzll(locations);
            locations &= locations - 1;
            Piece captured = pieceAt(board, to).second;
            total.push_back(Move(
            Coord(from), Coord(to), QUEEN, color, false, false, captured, NOPIECE));
        }
    }

    //bishop
    U64 bishops = board.pieces[color][BISHOP];
    while(bishops > 0){
        int from = __builtin_ctzll(bishops);
        bishops &= bishops - 1;
        U64 bishop_attack = bishopAttack(Coord(from), occupied);
        U64 locations = bishop_attack & ~teamPieces;
        while (locations > 0){
            int to = __builtin_ctzll(locations);
            locations &= locations - 1;
            Piece captured = pieceAt(board, to).second;
            total.push_back(Move(
            Coord(from), Coord(to), BISHOP, color, false, false, captured, NOPIECE));
        }
    }
    
    //pawn
    //ohgod
    U64 pawns = board.pieces[color][PAWN];
    //8 for white, 1 for black
    U64 promoRank = (color == WHITE) ? 0xFF00000000000000ULL : 0x00000000000000FFULL;
    //pawn moves are calculated globally
    U64 pushOneTotal = pawnPushOne(color, pawns, occupied);
    U64 pushOnePromo = pushOneTotal & promoRank;
    U64 pushOne = pushOneTotal & ~promoRank;
    //which way to go?
    int dir = (color == WHITE) ? 8 : -8; 
    while(pushOne){
        int to = __builtin_ctzll(pushOne);
        pushOne &= pushOne - 1;
        total.push_back(Move(Coord(to - dir) /*this is from*/, Coord(to),
    PAWN, color, false, false, NOPIECE, NOPIECE));
    }
    while(pushOnePromo){
        int to = __builtin_ctzll(pushOnePromo);
        pushOnePromo &= pushOnePromo - 1;
        for(int piece = ROOK; piece <= QUEEN; piece++){
            total.push_back(Move(Coord(to - dir), Coord(to),
    PAWN, color, false, false, NOPIECE, Piece(piece)));
        }
    }
    U64 pushTwo = pawnPushTwo(color, pawns, occupied);
    while(pushTwo){
        int to = __builtin_ctzll(pushTwo);
        pushTwo &= pushTwo - 1;
        total.push_back(Move(Coord(to - dir - dir), Coord(to),
    PAWN, color, false, false, NOPIECE, NOPIECE));
   
    }
    U64 leftAttacks  = pawnAttackLeft(color, pawns) & enemyPieces;
    U64 rightAttacks = pawnAttackRight(color, pawns) & enemyPieces;
    U64 leftPromo  = leftAttacks  & promoRank;
    U64 rightPromo = rightAttacks & promoRank;
    leftAttacks  &= ~promoRank;
    rightAttacks &= ~promoRank;
    int leftDir  = (color == WHITE) ? 9 : -7;
    int rightDir = (color == WHITE) ? 7 : -9;
    while (leftAttacks) 
    {
        int to = __builtin_ctzll(leftAttacks);
        leftAttacks &= leftAttacks - 1;
        Piece captured = pieceAt(board, to).second;
        total.push_back(Move(Coord(to - leftDir), Coord(to), PAWN, color, false, false, captured, NOPIECE));
    }
    while (rightAttacks) 
    {
        int to = __builtin_ctzll(rightAttacks);
        rightAttacks &= rightAttacks - 1;
        Piece captured = pieceAt(board, to).second;
        total.push_back(Move(Coord(to - rightDir), Coord(to), PAWN, color, false, false, captured, NOPIECE));
    }
    while (leftPromo) 
    {
        int to = __builtin_ctzll(leftPromo);
        leftPromo &= leftPromo - 1;
        Piece captured = pieceAt(board, to).second;
        for (int piece = ROOK; piece <= QUEEN; piece++)
            total.push_back(Move(Coord(to - leftDir), Coord(to), PAWN, color, false, false, captured, Piece(piece)));
    }
    while (rightPromo) 
    {
        int to = __builtin_ctzll(rightPromo);
        rightPromo &= rightPromo - 1;
        Piece captured = pieceAt(board, to).second;
        for (int piece = ROOK; piece <= QUEEN; piece++)
            total.push_back(Move(Coord(to - rightDir), Coord(to), PAWN, color, false, false, captured, Piece(piece)));
    }
    if (board.enPassant!=NOCOORD)
    {
        U64 ep = 1ULL << board.enPassant;
        //which way little boy
        U64 epLeft = pawnAttackLeft(color, pawns) & ep;
        U64 epRight = pawnAttackRight(color, pawns) & ep;
        if(epLeft){
            total.push_back(Move(
                Coord(board.enPassant - leftDir), Coord(board.enPassant), PAWN, color, false, true, PAWN, NOPIECE));
        }
        if(epRight){
            total.push_back(Move(
                Coord(board.enPassant - rightDir), Coord(board.enPassant), PAWN, color, false, true, PAWN, NOPIECE));
        }
    }
    // castling — king must not be in check, transit square must not be attacked,
    // landing square legality is handled by the filter below
    if (!isInCheck(color, board)) {
        if (color == WHITE) {
            if ((board.castlingRights & WHITEKING) &&
                !(occupied & ((1ULL << F1) | (1ULL << G1))) &&
                !isAttacked(F1, enemy, board))
                total.push_back(Move(E1, G1, KING, WHITE, true, false, NOPIECE, NOPIECE));

            if ((board.castlingRights & WHITEQUEEN) &&
                !(occupied & ((1ULL << B1) | (1ULL << C1) | (1ULL << D1))) &&
                !isAttacked(D1, enemy, board))
                total.push_back(Move(E1, C1, KING, WHITE, true, false, NOPIECE, NOPIECE));
        } else {
            if ((board.castlingRights & BLACKKING) &&
                !(occupied & ((1ULL << F8) | (1ULL << G8))) &&
                !isAttacked(F8, enemy, board))
                total.push_back(Move(E8, G8, KING, BLACK, true, false, NOPIECE, NOPIECE));

            if ((board.castlingRights & BLACKQUEEN) &&
                !(occupied & ((1ULL << B8) | (1ULL << C8) | (1ULL << D8))) &&
                !isAttacked(D8, enemy, board))
                total.push_back(Move(E8, C8, KING, BLACK, true, false, NOPIECE, NOPIECE));
        }
    }

    //check legality (ie is in check)
    for(Move& i : total){
        makeMove(board, i);
        if (!isInCheck(color, board))
            legal.push_back(i);
        unmakeMove(board, i);
    }
    return legal;
}
