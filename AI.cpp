#include "AI.h"
#include "ChessBoardState.h"
#include <limits>
#include <iostream>
#include <chrono>
#include "ChessBoard.h"
#include <utility>

AI::AI(ChessBoard &board) : board(board) {}

evalResult AI::minimax(ChessBoard &chessBoard, int depth, double alpha, double beta, bool playerTurn) {

    numEvals++;

    if (depth == 0) {
        return {"", evaluateBoard(chessBoard, playerTurn)};
    }

//    if (gameover){
//        if blacklost return 10000*depth;
//        else return -10000*depth;
//        TODO:game over

// TODO: stalemate
// if stalemate
// return 0

    if (playerTurn) {

        double maxEval = -std::numeric_limits<double>::infinity();

        for (int i = 0; i < 64; ++i) {
            if (chessBoard.squares[i] == 0) continue;
            std::unordered_set<unsigned short> set = chessBoard.grabPiece(i, playerTurn);
            if (!set.empty())
                for (unsigned short target: set) {
                    auto previousBoardState = ChessBoardState::fromChessBoard(&chessBoard);

                    chessBoard.makeMove(i, target, false);
                    evalResult result = minimax(chessBoard, depth - 1, alpha, beta, false);
                    chessBoard.undoMove(previousBoardState);

                    maxEval = std::max(maxEval, result.rating);
                    alpha = std::max(alpha, result.rating);
                    if (beta <= alpha)
                        break;
                }
        }
        evalResult maxEvalResult("", maxEval);
        return maxEvalResult;
    } else {
        double minEval = std::numeric_limits<double>::infinity();
        for (int i = 63; i >= 0; --i) {
            if (chessBoard.squares[i] == 0) continue;
            std::unordered_set<unsigned short> set = chessBoard.grabPiece(i, playerTurn);
            if (!set.empty())
                for (unsigned short target: set) {

                    auto previousBoardState = ChessBoardState::fromChessBoard(&chessBoard);

                    chessBoard.makeMove(i, target, false);
                    evalResult result = minimax(chessBoard, depth - 1, alpha, beta, true);
                    chessBoard.undoMove(previousBoardState);

                    minEval = std::min(minEval, result.rating);
                    beta = std::min(beta, result.rating);

                    if (beta <= alpha)
                        break;
                }
        }

        evalResult minEvalResult("", minEval);
        return minEvalResult;
    }
}


void AI::runEval(ChessBoard chessBoard, bool turn) {
    std::cout << "Evaluating board" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    evalResult result = minimax(chessBoard, 5, -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(),
                            turn);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Eval: " << result.rating << std::endl;
    std::cout << "Moves: " << result.moves << std::endl;
    std::cout << "Boards evaluated: " << numEvals << std::endl;
    std::cout << "Time taken to evaluate: " << duration.count() << "ms" << std::endl << "-------" << std::endl;

}

double AI::evaluateBoard(ChessBoard &chessBoard, bool playerTurn) const {

    double whiteEval = 0;
    double blackEval = 0;
    int countB = 0;
    int countW = 0;
//    const double endgameMaterialStart = rookValue * 2 + bishopValue + knightValue;

    for (int x = 0; x < 64; ++x){
        switch (chessBoard.squares[x]) {
            case B_PAWN:
                blackEval += pawnValue + pawnWeight[63 - x];
                break;
            case B_KNIGHT:
                blackEval += knightValue + knightWeight[x];
                break;
            case B_BISHOP:
                blackEval += bishopValue + bishopWeight[x];
                break;
            case B_ROOK:
                blackEval += rookValue + rookWeight[x];
                break;
            case B_QUEEN:
                blackEval += queenValue + queenWeight[x];
                break;
            case B_KING:
                blackEval += kingValue + KingWeight[63 - x];
                break;
            case W_PAWN:
                whiteEval += pawnValue + pawnWeight[x];
                break;
            case W_KNIGHT:
                whiteEval += knightValue + knightWeight[x];
                break;
            case W_BISHOP:
                whiteEval += bishopValue + bishopWeight[x];
                break;
            case W_ROOK:
                whiteEval += rookValue +  rookWeight[x];
                break;
            case W_QUEEN:
                whiteEval += queenValue + queenWeight[x];
                break;
            case W_KING:
                whiteEval += kingValue + KingWeight[63];
                break;
            default:
                break;
        }
    }

    return whiteEval - blackEval;
}

evalResult::evalResult(std::string commands, double rating) : moves(std::move(commands)), rating(rating) {}
