
#ifndef AUTOCHESS_AI_H
#define AUTOCHESS_AI_H

#include "ChessBoard.h"
#include "ChessBoardState.h"

class AI {
private:

    const double pawnValue = 1.0;
    const double bishopValue = 3.0;
    const double knightValue = 3.0;
    const double rookValue = 5.0;
    const double queenValue = 9.0;
    const double kingValue = 20.0;


    ChessBoard &board;

    double minimax(ChessBoard &chessBoard, int depth, double alpha, double beta, bool playerTurn);

public:

    int numEvals = 0;

    explicit AI(ChessBoard &board);

    void runEval(ChessBoard chessBoard, bool turn);

    double evaluateBoard(ChessBoardState boardState, bool playerTurn) const;
};


#endif //AUTOCHESS_AI_H
