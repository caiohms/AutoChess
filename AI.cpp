#include "AI.h"
#include "ChessBoardState.h"
#include <limits>
#include <iostream>
#include <chrono>
#include "ChessBoard.h"
#include <utility>

AI::AI() = default;

/**
 * The minimax function
 * @param chessBoard The global chessboard is referenced across all function calls
 * @param depth The current searching depth
 * @param alpha The Alpha parameter
 * @param beta The Beta parameter
 * @param playerTurn The player turn for the current depth
 * @param evalResult The EvalResult object is copied forward at every function call. Its values are changed according to the minimax results
 * @param move The string of the movement made in the current function call (i.e. "0 -> 8" when a piece at square 0 moves to square 8)
 * @return the EvalResult object for the evaluation
 */
EvalResult
AI::minimax(ChessBoard &chessBoard, int depth, double alpha, double beta, bool playerTurn, EvalResult evalResult,
            std::string move) {

    // Each board processed increases this global variable by 1. This number is used to print out the total number of
    // boards evaluated.
    numEvals++;

    // The algorithm checks if the current state of the ChessBoard represents a CheckMate. The rating value is
    // multiplied by the depth at which the Checkmate is found.
    // This is done in order to guide the algorithm towards the fastest possible checkmate.
    // If playerTurn is true, it is White's turn to play, and if White is checkmated when his turn begins, it means
    // Black won. Thus, the evaluation is negative if it is White's turn, and positive for Black.
    if (chessBoard.checkmate) {
        if (playerTurn) {
            evalResult.rating = (-100000 * depth);
            return evalResult;
        }
        evalResult.rating = (100000 * depth);
        return evalResult;
    }

    // If the current board state represents a Tie, the evaluation is zero. If white is to move and out of two possible
    // moves one loses the game and another results in a draw, the AI will pick the draw.
    if (chessBoard.gameTied) {
        evalResult.rating = 0;
        return evalResult;
    }

    // Only boards at depth = 0 are evaluated.
    if (depth == 0) {
        evalResult.rating = evaluateBoard(chessBoard);
        return evalResult;
    }

    // White to move
    if (playerTurn) {
        // The MaxEval value is set to the minimum Double value
        double maxEval = -std::numeric_limits<double>::infinity();

        // Creating a new evalResponse object
        EvalResult evalResponse = EvalResult({}, 0);

        // Looping all board squares
        for (int i = 0; i < 64; ++i) {
            // If there is no piece at the current square or if it is a Black piece, continue to the next iteration.
            if (chessBoard.squares[i] == 0 || (chessBoard.squares[i] & 0b10000000)) continue;

            // The algorithm grabs a piece, like a user would normally do. The returned set are the squares the piece
            // may move to.
            std::unordered_set<unsigned short> set = chessBoard.grabPiece(i, playerTurn);

            // If the piece has any possible moves
            if (!set.empty())

                // Then we loop all targets and execute them one by one, changing the board state and deepening the search
                for (unsigned short target: set) {

                    // The previous board state is saved in order to restore it later when undoing the move.
                    // (Remember the same board is referenced across all function calls)
                    auto previousBoardState = ChessBoardState::fromChessBoard(&chessBoard);

                    // The piece is moved from i to target.
                    chessBoard.makeMove(i, target, false);

                    // A string for the current movement is generated. It is already formatted to more easily display
                    // it to the user afterwards.
                    std::string _move = std::to_string(i) + " to " + std::to_string(target) + " | ";

                    // The evalResult object is assigned the result of the next minimax call
                    evalResult = minimax(chessBoard, depth - 1, alpha, beta, false, evalResult, _move);

                    // After the minimax is calculated, the move is undone using the saved previous board state.
                    chessBoard.undoMove(previousBoardState);

                    // If the evaluated rating is bigger than the valued received by the current minimax call,
                    // the maxEval value is updated with the evaluated rating and the EvalResponse object receives the
                    // evalResult value returned by the last minimax evaluation.
                    if (evalResult.rating > maxEval) {
                        maxEval = evalResult.rating;
                        evalResponse = evalResult;
                    }

                    // The same is done for the alpha value.
                    // Additionally, when the alpha value is changed, the evalResponse.moves array (containing
                    // a list of moves in string format (i.e. "1 -> 18")) is updated with the current move. It is added
                    // to the list at the current Depth. This is done so that we can track the best moves calculated at
                    // each depth.
                    if (evalResult.rating > alpha) {
                        alpha = evalResult.rating;
                        evalResponse.moves[depth] = _move;
                    }

                    // If the beta value calculated is lower than or equal to the alpha value, break the loop.
                    if (beta <= alpha)
                        break;
                }
        }
        // In the end, the best evaluationResponse is returned
        return evalResponse;

    } else {
        // Black to move
        // The same steps are executed except:
        // * MaxEval is substituted for MinEval, which is set to the Maximum double value.
        // * The board is looped from 63 (lower right) to 0 (upper left). We noticed minor speed improvements at later
        // stages of the game when moving squares near the opposite side of the board first.
        // * The bitwise AND operation is substituted for the White value (0b01000000). We want to skip white pieces as
        // they have no moves available when it is Black to move.
        // * The beta value is calculated instead of alpha, using the inverse math (comparing the minimum value instead of
        // the maximum).
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

                    if (evalResult.rating < beta) {
                        beta = evalResult.rating;
                        evalResponse.moves[depth] = _move;
                    }

                    if (beta <= alpha)
                        break;
                }
        }

        return evalResponse;
    }
}

/**
 * Runs the evaluation for the current board
 * @param chessBoard The chessBoard to be evaluated
 * @param turn The current turn to move
 * @return The evaluation value as a double. (It was planned it would be used in an user interface but there was not
 * enough time to add that).
 */
double AI::runEval(ChessBoard chessBoard, bool turn) {
    // If it is White to move, there is no need to spend time evaluating the board. Thus, this implementation only plays
    // as Black.
    if (turn) return 0.0;

    std::cout << "Evaluating board" << std::endl;

    long long int duration = 0;

    // The minimum depth to start at
    int depth = 5;

    EvalResult result = EvalResult({}, 0);

    while (duration < 7000) {
        // If the board is evaluated in less than 7 seconds, we spend more time evaluating to a deeper depth.
        auto start = std::chrono::high_resolution_clock::now();

        result = minimax(chessBoard, depth, -std::numeric_limits<double>::infinity(),
                         std::numeric_limits<double>::infinity(),
                         turn, EvalResult({}, 0), std::string());

        auto stop = std::chrono::high_resolution_clock::now();
        duration = duration_cast<std::chrono::milliseconds>(stop - start).count();
        depth++;
        if (duration < 7000) std::cout << "Deepening search to depth: " << depth << std::endl;
    }

    // The depth is increased at the last iteration of the previous while loop. This corrects the value.
    depth--;

    std::cout << "Depth: " << depth << std::endl;
    std::cout << "Evaluation: " << result.rating << std::endl;
    std::cout << "Moves: ";
    // The resulting branch of the best moves tree is printed out.
    for (int i = result.moves.size() - 1; i >= 0; --i) {
        std::cout << result.moves[i];
    }
    std::cout << std::endl;
    std::cout << "Boards evaluated: " << numEvals << std::endl;
    std::cout << "Time taken to evaluate: " << duration << "ms" << std::endl << "-------" << std::endl;

    return result.rating;
}

/**
 * The board itself is evaluated.
 * All pieces have a default value but a bonus is added depending on their position on the board.
 * @param chessBoard The chessBoard reference to be evaluated
 * @return The evaluated value. A positive value indicates White has an advantage while negative values indicate Black
 * should be ahead.
 */
double AI::evaluateBoard(ChessBoard &chessBoard) const {

    // The total points for each player. This value is positive for both.
    // The final evaluation is equal to the White evaluation minus the Black evaluation.
    double whiteEval = 0;
    double blackEval = 0;

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
