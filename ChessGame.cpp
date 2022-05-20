
#include <iostream>
#include "ChessGame.h"
#include "ChessBoard.h"
#include <chrono>

ChessGame::ChessGame(ChessBoard &chessBoard, bool &turn, AI &ai) : board(chessBoard), playerTurn(turn), ai(ai) {}

void ChessGame::setMousePos(int x, int y) {
    board.setMousePos(x, y);
}

void ChessGame::grabPiece(int x, int y) {
    board.mouseGrabPiece(x, y);
}

/**
 * Called in the main thread by a mouse movement, indicating a mouse release action which may mean a selected piece is
 * being moved.
 * @param x The mouse X position
 * @param y The mouse Y position
 */
void ChessGame::releasePiece(int x, int y) {

    // The old board state is saved before calling the mouseReleasePiece() method.
    unsigned short oldSquares[64];
    std::copy_n(getBoardSquares(), 64, oldSquares);

    board.mouseReleasePiece(x, y);

    // If any piece has changed positions, it means the user moved a piece. The evaluation method is then invoked,
    // to evaluate the newest board state.
    for (int i = 0; i < 64; ++i) {
        if (getBoardSquares()[i] != oldSquares[i]) {
            std::cout << "Board changed." << std::endl;
            ai.numEvals = 0;
            ai.runEval(board, playerTurn);
            break;
        }
    }
}

void ChessGame::draw(sf::RenderWindow &window) {
    board.draw();
}

void ChessGame::setGameSize(sf::Vector2<unsigned int> vector2) {
    board.setBoardSize(vector2);
}

long ChessGame::moveMaker(int i, sf::RenderWindow &window, bool playerTurn, std::ofstream &ofstream) {
    return board.moveMaker(i, playerTurn, ofstream);
}

const unsigned short *ChessGame::getBoardSquares() {
    return board.getSquares();
}

void ChessGame::makeAutomatedMove(int from, int to) {
    board.makeMove(from, to, false);
}
