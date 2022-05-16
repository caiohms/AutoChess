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

    if (chessBoard.checkmate) {
        if (playerTurn) {
            evalResult.rating = (-100000 * depth);
            return evalResult;
        }
        evalResult.rating = (100000 * depth);
        return evalResult;
    }

    if (chessBoard.gameTied) {
        evalResult.rating = 0;
        return evalResult;
    }

    if (depth == 0) {
        evalResult.rating = evaluateBoard(chessBoard, playerTurn);
        return evalResult;
    }

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

                    if (evalResult.rating > maxEval) {
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

                    if (evalResult.rating < minEval) {
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


double AI::runEval(ChessBoard chessBoard, bool turn) {
    if (turn) return 0.0;
    std::cout << "Evaluating board" << std::endl;

    long long int duration = 0;

    int depth = 5;

    EvalResult result = EvalResult({}, 0);

    while (duration < 7000) {
        auto start = std::chrono::high_resolution_clock::now();

        result = minimax(chessBoard, depth, -std::numeric_limits<double>::infinity(),
                         std::numeric_limits<double>::infinity(),
                         turn, EvalResult({}, 0), std::string());

        auto stop = std::chrono::high_resolution_clock::now();
        duration = duration_cast<std::chrono::milliseconds>(stop - start).count();
        depth++;
        if (duration < 7000) std::cout << "Deepening search to depth: " << depth << std::endl;
    }

    depth--;

    std::cout << "Depth: " << depth << std::endl;
    std::cout << "Evaluation: " << result.rating << std::endl;
    std::cout << "Moves: ";
    for (int i = result.moves.size() - 1; i >= 0; --i) {
        std::cout << result.moves[i];
    }
    std::cout << std::endl;
    std::cout << "Boards evaluated: " << numEvals << std::endl;
    std::cout << "Time taken to evaluate: " << duration << "ms" << std::endl << "-------" << std::endl;

    return result.rating;
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
                blackEval += knightValue + knightWeight[63 - x];
                break;
            case B_BISHOP:
                blackEval += bishopValue + bishopWeight[63 - x];
                break;
            case B_ROOK:
                blackEval += rookValue + rookWeight[63 - x];
                break;
            case B_QUEEN:
                blackEval += queenValue + queenWeight[63 - x];
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
                whiteEval += kingValue + KingWeight[x];
                break;
            default:
                break;
        }
    }

    return whiteEval - blackEval;
}

EvalResult::EvalResult(const std::array<std::string, 10> &moves, double rating) : moves(moves), rating(rating) {}
