
#ifndef AUTOCHESS_AI_H
#define AUTOCHESS_AI_H

#include "ChessBoard.h"

class AI {
private:

    ChessBoard &board;

    double minimax(ChessBoard chessBoard, int depth, double alpha, double beta, PlayerTurn playerTurn);

public:
    explicit AI(ChessBoard &board);

    void doEvaluate();

};


#endif //AUTOCHESS_AI_H
