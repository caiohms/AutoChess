
#include "ChessGame.h"
#include "ChessBoard.h"

ChessGame::ChessGame(ChessBoard &chessBoard, PlayerTurn &turn) : board(chessBoard), playerTurn(turn) {}

void ChessGame::setMousePos(int x, int y) {
    board.setMousePos(x, y);
}

void ChessGame::grabPiece(int x, int y) {
    board.mouseGrabPiece(x, y);
}

void ChessGame::releasePiece(int x, int y) {
    board.releasePiece(x, y);
}

void ChessGame::draw(sf::RenderWindow &window) {
    board.draw(window);
}

const unsigned short *ChessGame::getBoardSquares() {
    return board.getSquares();
}

void ChessGame::setBoardSize(sf::Vector2<unsigned int> vector2) {
    board.setBoardSize(vector2);
}

long ChessGame::moveMaker(int i, sf::RenderWindow & window) {
    return board.moveMaker(i, window);
}
