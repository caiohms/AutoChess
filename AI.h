
#ifndef AUTOCHESS_AI_H
#define AUTOCHESS_AI_H

#include "ChessBoard.h"
#include "ChessBoardState.h"

// The EvalResult struct stores the moves list as a string array and the evaluation value.
typedef struct EvalResult {
    EvalResult(const std::array<std::string, 10> &moves, double rating);

    std::array<std::string, 10> moves;
    double rating;
} evalResult;


class AI {
private:

    // The default values assigned to each piece when evaluating.
    const double pawnValue = 1.0;
    const double bishopValue = 3.0;
    const double knightValue = 3.0;
    const double rookValue = 5.0;
    const double queenValue = 9.0;
    const double kingValue = 2000.0;

    EvalResult
    minimax(ChessBoard &chessBoard, int depth, double alpha, double beta, bool playerTurn, EvalResult evalResult,
            std::string move);

    // The following double arrays represent the bonus values added to each piece depending on their board position
    double knightWeight[64] = {
            0.1, 0.2, 0.3, 0.3, 0.3, 0.3, 0.2, 0.1,
            0.2, 0.4, 0.5, 0.5, 0.5, 0.5, 0.4, 0.2,
            0.3, 0.5, 0.8, 0.6, 0.6, 0.8, 0.5, 0.2,
            0.3, 0.5, 0.6, 0.7, 0.7, 0.6, 0.5, 0.2,
            0.3, 0.5, 0.6, 0.7, 0.7, 0.6, 0.5, 0.2,
            0.3, 0.5, 0.8, 0.7, 0.7, 0.8, 0.5, 0.1,
            0.2, 0.4, 0.5, 0.5, 0.5, 0.5, 0.4, 0.2,
            0, 0.2, 0.3, 0.3, 0.3, 0.3, 0.2, 0};

    double KingWeight[64] = {
            0.1, 0.1, 0.1, 0, 0, 0.1, 0.1, 0.1,
            0.1, 0.1, 0.1, 0, 0, 0.1, 0.1, 0.1,
            0.2, 0.1, 0.1, 0, 0, 0.1, 0.1, 0.2,
            0.2, 0.1, 0.1, 0, 0, 0.1, 0.1, 0.2,
            0.2, 0.2, 0.2, 0.1, 0.1, 0.2, 0.2, 0.2,
            0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.2,
            0.4, 0.4, 0.3, 0.3, 0.2, 0.3, 0.4, 0.2,
            0.2, 1, 0.3, 0.3, 0.3, 0.3, 1, 0.2};

    double queenWeight[64] = {
            0, 0.1, 0.1, 0.2, 0.2, 0.1, 0.1, 0,
            0.1, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.1,
            0.1, 0.3, 0.7, 0.7, 0.7, 0.7, 0.3, 0.1,
            0.2, 0.3, 0.7, 0.8, 0.8, 0.7, 0.3, 0.2,
            0.3, 0.3, 0.7, 0.8, 0.8, 0.7, 0.3, 0.2,
            0.1, 0.4, 0.7, 0.7, 0.7, 0.7, 0.3, 0.1,
            0.1, 0.8, 0.6, 0.6, 0.6, 0.6, 0.3, 0.1,
            0, 0.1, 0.1, 0.2, 0.2, 0.1, 0.1, 0};

    double rookWeight[64] = {
            0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3,
            0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2,
            0, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0,
            0, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0,
            0, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0,
            0, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0,
            0, 0.2, 0.3, 0.3, 0.2, 0.3, 0.4, 0,
            0.1, 0.3, 0.4, 0.5, 0.5, 0.4, 0.3, 0.1};

    double pawnWeight[64] = {
            1, 1, 1, 1, 1, 1, 1, 1,
            0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
            0.2, 0.3, 0.4, 0.4, 0.4, 0.4, 0.3, 0.2,
            0.2, 0.3, 0.4, 0.5, 0.5, 0.4, 0.3, 0.2,
            0.2, 0.3, 0.4, 0.5, 0.5, 0.4, 0.3, 0.2,
            0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0};

    double bishopWeight[64] = {
            0, 0, 0, 0, 0, 0, 0, 0,
            0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3,
            0.3, 0.2, 0.1, 0.1, 0.1, 0.1, 0.2, 0.3,
            0.3, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.3,
            0.3, 0.1, 0.1, 0.1, 0.1, 0.2, 0.1, 0.3,
            0.3, 0.2, 0.5, 0.1, 0.1, 0.5, 0.2, 0.3,
            0.3, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.3,
            0, 0, 0, 0, 0, 0, 0, 0};

public:

    int numEvals = 0;

    explicit AI();

    double runEval(ChessBoard chessBoard, bool turn);

    double evaluateBoard(ChessBoard &chessBoard) const;

};

#endif //AUTOCHESS_AI_H