
#ifndef AUTOCHESS_AI_H
#define AUTOCHESS_AI_H

#include "ChessBoard.h"
#include "ChessBoardState.h"

typedef struct EvalResult {
    EvalResult(const std::array<std::string, 10> &moves, double rating);

    std::array<std::string, 10> moves;
    double rating;
} evalResult;


class AI {
private:

    const double pawnValue = 1.0;
    const double bishopValue = 3.0;
    const double knightValue = 3.0;
    const double rookValue = 5.0;
    const double queenValue = 9.0;
    const double kingValue = 2000.0;

    ChessBoard &board;

    EvalResult
    minimax(ChessBoard &chessBoard, int depth, double alpha, double beta, bool playerTurn, EvalResult evalResult,
            std::string move);

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

//    double pawnWeight[64] = {
//            0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1,
//            1, 1, 1, 1, 1, 1, 1, 1,
//            0.2, 0.2, 0.4, 0.4, 0.4, 0.4, 0.2, 0.2,
//            0.2, 0.3, 0.3, 0.4, 0.4, 0.3, 0.2, 0.2,
//            0.2, 0.3, 0.3, 0.4, 0.4, 0.3, 0.2, 0.2,
//            0.3, 0.1, 0.1, 0.3, 0.3, 0.1, 0.1, 0.3,
//            0.2, 0.3, 0.3, 0, 0, 0.3, 0.3, 0.2,
//            0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2};
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

    explicit AI(ChessBoard &board);

    double runEval(ChessBoard chessBoard, bool turn);

    double evaluateBoard(ChessBoard &chessBoard, bool playerTurn) const;

    double
    forcerKingEndgame(short friendlySquareKing, short opponentSquareKing, int materialFriend, int materialOpponent,
                      float endgameWeight) const;

    static double EndgamePhaseWeight(int materialCount, double endgameMaterialStart);

};

#endif //AUTOCHESS_AI_H