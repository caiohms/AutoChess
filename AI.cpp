#include "AI.h"
#include "ChessBoardState.h"
#include <limits>
#include <iostream>
#include <chrono>
#include "ChessBoard.h"
#include <utility>

AI::AI(ChessBoard &board) : board(board) {}

EvalResult
AI::minimax(ChessBoard &chessBoard, int depth, double alpha, double beta, bool playerTurn, EvalResult evalResult,
            std::string move) {

    numEvals++;


    if (depth == 0) {
//        evalResult.moves[0] = std::move(move);
        evalResult.rating = evaluateBoard(chessBoard, playerTurn);
        return evalResult;
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
        EvalResult evalResponse = EvalResult({}, 0);

        for (int i = 0; i < 64; ++i) {
            if (chessBoard.squares[i] == 0 || (chessBoard.squares[i] & 0b10000000)) continue;

            std::unordered_set<unsigned short> set = chessBoard.grabPiece(i, playerTurn);
            if (!set.empty())
                for (unsigned short target: set) {
                    auto previousBoardState = ChessBoardState::fromChessBoard(&chessBoard);

                    chessBoard.makeMove(i, target, false);
                    std::string _move = std::to_string(i) + " to " + std::to_string(target) + " | ";
                    evalResult = minimax(chessBoard, depth - 1, alpha, beta, false, evalResult, _move);
                    chessBoard.undoMove(previousBoardState);

                    if (evalResult.rating > maxEval){
                        maxEval = evalResult.rating;
                        evalResponse = evalResult;
                    }

                    if (evalResult.rating > alpha) {
                        alpha = evalResult.rating;
                        evalResponse.moves[depth] = _move;
                    }

//                    alpha = std::max(alpha, evalResult.rating);

                    if (beta <= alpha)
                        break;
                }
        }
        return evalResponse;
    } else {
        double minEval = std::numeric_limits<double>::infinity();
        EvalResult evalResponse = EvalResult({}, 0);

        for (int i = 63; i >= 0; --i) {
            if (chessBoard.squares[i] == 0 || (chessBoard.squares[i] & 0b01000000)) continue;

            std::unordered_set<unsigned short> set = chessBoard.grabPiece(i, playerTurn);
            if (!set.empty())
                for (unsigned short target: set) {

                    auto previousBoardState = ChessBoardState::fromChessBoard(&chessBoard);

                    chessBoard.makeMove(i, target, false);
                    std::string _move = std::to_string(i) + " to " + std::to_string(target) + " | ";
                    evalResult = minimax(chessBoard, depth - 1, alpha, beta, true, evalResult, _move);
                    chessBoard.undoMove(previousBoardState);

                    if (evalResult.rating < minEval){
                        minEval = evalResult.rating;
                        evalResponse = evalResult;
                    }
//                    minEval = std::min(minEval, result.rating);

                    if (evalResult.rating < beta) {
                        beta = evalResult.rating;
                        evalResponse.moves[depth] = _move;
                    }
//                    beta = std::min(beta, evalResult.rating);

                    if (beta <= alpha)
                        break;
                }
        }

        return evalResponse;
    }
}


void AI::runEval(ChessBoard chessBoard, bool turn) {
    std::cout << "Evaluating board" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    EvalResult result = minimax(chessBoard, 5, -std::numeric_limits<double>::infinity(),
                                std::numeric_limits<double>::infinity(),
                                turn, EvalResult({}, 0), std::string());

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Eval: " << result.rating << std::endl;
    std::cout << "Moves: ";
    std::for_each(result.moves.begin(), result.moves.end(), [&](const auto &item) {
        std::cout << item;
    });
    std::cout << std::endl;
    std::cout << "Boards evaluated: " << numEvals << std::endl;
    std::cout << "Time taken to evaluate: " << duration.count() << "ms" << std::endl << "-------" << std::endl;

}

double AI::evaluateBoard(ChessBoard &chessBoard, bool playerTurn) const {

    double whiteEval = 0;
    double blackEval = 0;
    int countB = 0;
    int countW = 0;
//    const double endgameMaterialStart = rookValue * 2 + bishopValue + knightValue;

    for (int x = 0; x < 64; ++x) {
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
                whiteEval += rookValue + rookWeight[x];
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

EvalResult::EvalResult(const std::array<std::string, 10> &moves, double rating) : moves(moves), rating(rating) {}
