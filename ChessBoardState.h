
#ifndef AUTOCHESS_CHESSBOARDSTATE_H
#define AUTOCHESS_CHESSBOARDSTATE_H

class ChessBoard;

#include "ChessBoard.h"

/**
 * This class is used to represent a board state with reduced memory usage. It stores only the variables required to
 * take the board to a previous state
 */
class ChessBoardState {
public:
    ChessBoardState(bool checkmate, bool gameTied, bool gameFinished, bool bCastleKingSide, bool bCastleQueenSide,
                    bool wCastleKingSide, bool wCastleQueenSide, unsigned short wKingSquare, unsigned short bKingSquare,
                    unsigned short enPassantEnabledSquare, bool playerTurn, unsigned short *squares);

    bool gameTied;
    bool checkmate;

    bool gameFinished;
    bool playerTurn;

    unsigned short wKingSquare;
    unsigned short bKingSquare;

    bool bCastleKingSide;
    bool bCastleQueenSide;
    bool wCastleKingSide;
    bool wCastleQueenSide;

    unsigned short enPassantEnabledSquare;

    unsigned short squares[64]{};

    static void copyArray(const unsigned short *pFrom, unsigned short *pTo);

    static ChessBoardState fromChessBoard(ChessBoard *pBoard);
};


#endif //AUTOCHESS_CHESSBOARDSTATE_H
