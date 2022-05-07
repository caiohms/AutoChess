
#ifndef AUTOCHESS_AI_H
#define AUTOCHESS_AI_H

#include "ChessBoard.h"
#include "ChessBoardState.h"

class AI {
private:

    double pawnValue = 1.0;
    double bishopValue = 3.0;
    double knightValue = 3.0;
    double rookValue = 5.0;
    double queenValue = 9.0;
    double kingValue = 20.0;


    ChessBoard &board;

    double minimax(ChessBoard chessBoard, int depth, double alpha, double beta, PlayerTurn playerTurn);

public:

    int numEvals = 0;

    explicit AI(ChessBoard &board);

    void runEval();

    double evaluateBoard(ChessBoardState boardState, PlayerTurn playerTurn);
};


#endif //AUTOCHESS_AI_H
