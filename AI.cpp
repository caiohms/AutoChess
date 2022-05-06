#include "AI.h"
#include "ChessBoardState.h"
#include <limits>
#include <iostream>

AI::AI(ChessBoard &board) : board(board) {}

double AI::minimax(ChessBoard chessBoard, int depth, double alpha, double beta, PlayerTurn playerTurn) {

//    playerTurn = (playerTurn == WHITE) ? BLACK : WHITE;

//    if (depth == 0) ||

//    if (playerTurn) // Black
    double maxEval = std::numeric_limits<double>::infinity();
    std::cout << maxEval << std::endl;

    return maxEval;
}

void AI::doEvaluate() {
    ChessBoardState boardState = ChessBoardState(board.isBCastleKingSide(), board.isBCastleQueenSide(),
                                                 board.isWCastleKingSide(), board.isWCastleQueenSide(),
                                                 board.getEnPassantEnabledSquare(), board.getSquares());

    std::cout << "Evaluating board" << std::endl;


}
