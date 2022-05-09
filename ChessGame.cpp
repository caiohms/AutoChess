
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

void ChessGame::releasePiece(int x, int y) {

    unsigned short oldSquares[64];
    std::copy_n(getBoardSquares(), 64, oldSquares);

    board.mouseReleasePiece(x, y);

    for (int i = 0; i < 64; ++i) {
        if (getBoardSquares()[i] != oldSquares[i]) {
            std::cout << "Board changed!" << std::endl;
            ai.runEval(board);
            break;
        }
    }
}

void ChessGame::draw(sf::RenderWindow &window) {
    board.draw();
}

void ChessGame::setBoardSize(sf::Vector2<unsigned int> vector2) {
    board.setBoardSize(vector2);
}

long ChessGame::moveMaker(int i, sf::RenderWindow &window, bool playerTurn) {
    return board.moveMaker(i, playerTurn);
}

const unsigned short *ChessGame::getBoardSquares() {
    return board.getSquares();
}

void ChessGame::makeAutomatedMove(int from, int to) {
    board.makeMove(from, to);
}
