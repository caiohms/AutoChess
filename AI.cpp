#include "AI.h"
#include <limits>
#include <iostream>

AI::AI(PlayerTurn &turn) : turn(turn) {}

int AI::minimax(int position, int depth, double alpha, double beta, PlayerTurn playerTurn) {

//    playerTurn = (playerTurn == WHITE) ? BLACK : WHITE;

//    if (depth == 0) ||

//    if (playerTurn) // Black
    int maxEval = std::numeric_limits<double>::infinity();
    std::cout << maxEval << std::endl;


}