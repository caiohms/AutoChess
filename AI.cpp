#include "AI.h"
#include "ChessBoardState.h"
#include <limits>
#include <iostream>
#include <chrono>
#include "ChessBoard.h"
#include <utility>

AI::AI(ChessBoard &board) : board(board) {}

double AI::minimax(ChessBoard &chessBoard, int depth, double alpha, double beta, bool playerTurn) {

    numEvals++;

    if (depth == 0) {
        auto boardState = ChessBoardState::fromChessBoard(&chessBoard);
        double e = evaluateBoard(boardState, playerTurn);
//        std::cout << e << std::endl;
        return e;
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

                    chessBoard.makeMove(i, target);
                    double eval = minimax(chessBoard, depth - 1, alpha, beta, false);
                    chessBoard.undoMove(previousBoardState);

                    maxEval = std::max(maxEval, eval);
                    alpha = std::max(alpha, eval);
                    if (beta <= alpha)
                        break;
                }
        }
        return maxEval;
    } else {
        double minEval = std::numeric_limits<double>::infinity();
        for (int i = 0; i < 64; ++i) {
            if (chessBoard.squares[i] == 0) continue;
            std::unordered_set<unsigned short> set = chessBoard.grabPiece(i, playerTurn);
            if (!set.empty())
                for (unsigned short target: set) {

                    auto previousBoardState = ChessBoardState::fromChessBoard(&chessBoard);

                    chessBoard.makeMove(i, target);
                    double eval = minimax(chessBoard, depth - 1, alpha, beta, true);
                    chessBoard.undoMove(previousBoardState);

                    minEval = std::min(minEval, eval);
                    beta = std::min(beta, eval);

                    if (beta <= alpha)
                        break;
                }
        }

        return minEval;
    }
}


void AI::runEval(ChessBoard chessBoard, bool turn) {
    std::cout << "Evaluating board" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    double result = minimax(chessBoard, 5, -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(),
                            turn);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Future evaluated to: " << result << std::endl;
    std::cout << "Boards evaluated: " << numEvals << std::endl;
    std::cout << "Time taken to evaluate: " << duration.count() << "ms" << std::endl << "-------" << std::endl;

}

double AI::evaluateBoard(ChessBoardState boardState, bool playerTurn) const {

    double whiteEval = 0;
    double blackEval = 0;
    int countB = 0;
    int countW = 0;
//    const double endgameMaterialStart = rookValue * 2 + bishopValue + knightValue;

    for (unsigned short square: boardState.squares) {
        switch (square) {
            case B_PAWN:
                blackEval += pawnValue;
                countB += 1;
                break;
            case B_KNIGHT:
                blackEval += knightValue;
                break;
            case B_BISHOP:
                blackEval += bishopValue;
                break;
            case B_ROOK:
                blackEval += rookValue;
                break;
            case B_QUEEN:
                blackEval += queenValue;
                break;
            case B_KING:
                blackEval += kingValue;
                break;
            case W_PAWN:
                whiteEval += pawnValue;
                countW += 1;
                break;
            case W_KNIGHT:
                whiteEval += knightValue;
                break;
            case W_BISHOP:
                whiteEval += bishopValue;
                break;
            case W_ROOK:
                whiteEval += rookValue;
                break;
            case W_QUEEN:
                whiteEval += queenValue;
                break;
            case W_KING:
                whiteEval += kingValue;
                break;
            default:
                break;
        }
    }
//    int whiteMaterialWithoutPawns = whiteEval - (countW * pawnValue);
//    int blackMaterialWithoutPawns = blackEval - (countB * pawnValue);
//
//    float whiteEndgamePhaseWeight = EndgamePhaseWeight(whiteMaterialWithoutPawns, endgameMaterialStart);
//    float blackEndgamePhaseWeight = EndgamePhaseWeight(blackMaterialWithoutPawns, endgameMaterialStart);
//
//    unsigned short wKingSquare = board.getWKingSquare();
//    unsigned short bKingSquare = board.getBKingSquare();
//    whiteEval += forcerKingEndgame(wKingSquare, bKingSquare, whiteEval, blackEval, blackEndgamePhaseWeight);
//    blackEval += forcerKingEndgame(bKingSquare, wKingSquare, blackEval, whiteEval, whiteEndgamePhaseWeight);
//
//
    return whiteEval - blackEval;
}
//
//double AI::forcerKingEndgame(short friendlySquareKing, short opponentSquareKing, int materialFriend, int materialOpponent,
//                      float endgameWeight) const {
//    double score = 0;
//
//    if (materialFriend > materialOpponent + pawnValue * 2 && endgameWeight > 0) {
//
//        int friendlyKingSquare = board.getSquares()[friendlySquareKing];
//        int opponentKingSquare = board.getSquares()[opponentSquareKing];
//        score += PrecomputedMoveData.centreManhattanDistance[opponentKingSquare] * 10;
//
//        // use ortho dst to promote direct opposition
//        score += (14 - PrecomputedMoveData.NumRookMovesToReachSquare(friendlyKingSquare, opponentKingSquare)) * 4;
//
//        return (int) (score * endgameWeight);
//    }
//    return 0;
//}
//
//double AI::EndgamePhaseWeight(int materialCount, double endgameMaterialStart) {
//    const double multiplier = 1 / endgameMaterialStart;
//    return 1 - std::min(1.0, materialCount * multiplier);
//}