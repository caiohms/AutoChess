
#ifndef AUTOCHESS_CHESSBOARDSTATE_H
#define AUTOCHESS_CHESSBOARDSTATE_H

#include "ChessBoard.h"

class ChessBoardState {
public:
    ChessBoardState(bool bCastleKingSide, bool bCastleQueenSide, bool wCastleKingSide, bool wCastleQueenSide,
                    unsigned short enPassantEnabledSquare, const unsigned short *squares);

    explicit ChessBoardState(const ChessBoard& board);

    bool bCastleKingSide;
    bool bCastleQueenSide;
    bool wCastleKingSide;
    bool wCastleQueenSide;

    unsigned short enPassantEnabledSquare;

    unsigned short squares[64]{};

    static void copyArray(const unsigned short *pFrom, unsigned short *pTo);
};


#endif //AUTOCHESS_CHESSBOARDSTATE_H
