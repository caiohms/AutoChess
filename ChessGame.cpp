
#include "ChessGame.h"
#include "ChessBoard.h"

ChessGame::ChessGame(ChessBoard &chessBoard) : board(chessBoard) {}

void ChessGame::setMousePos(int x, int y) {
    board.setMousePos(x, y);
}

void ChessGame::grabPiece(int x, int y) {
    board.grabPiece(x, y);
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
