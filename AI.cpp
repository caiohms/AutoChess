#include "AI.h"
#include "ChessBoardState.h"
#include <limits>
#include <iostream>

AI::AI(ChessBoard &board) : board(board) {}

double AI::minimax(ChessBoard chessBoard, int depth, double alpha, double beta, PlayerTurn playerTurn) {

//    if (depth == 0) ||
    double eval;

    if (playerTurn == PlayerTurn::BLACK) { // Black
        double maxEval = -std::numeric_limits<double>::infinity();
        for (int i = 0; i < 64; ++i) {
            eval = minimax(, depth - 1, alpha, beta, WHITE);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);

            if (beta <= alpha)break;

        }
        return maxEval;
    } else if (playerTurn == PlayerTurn::WHITE) { // WHITE
        double minEval = std::numeric_limits<double>::infinity();
        for (int i = 0; i < 64; ++i) {
            eval = minimax(, depth - 1, alpha, beta, BLACK);
            minEval = std::min(minEval, eval);
            alpha = std::min(alpha, eval);

            if (beta <= alpha)break;

        }
        return minEval;
    }
}

void AI::doEvaluate() {
    ChessBoardState boardState = ChessBoardState(board.isBCastleKingSide(), board.isBCastleQueenSide(),
                                                 board.isWCastleKingSide(), board.isWCastleQueenSide(),
                                                 board.getEnPassantEnabledSquare(), board.getSquares());

    std::cout << "Evaluating board" << std::endl;
    double result = minimax(board, 2, 0, 0, BLACK);


}
