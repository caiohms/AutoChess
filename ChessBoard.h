
#ifndef AUTOCHESS_CHESSBOARD_H
#define AUTOCHESS_CHESSBOARD_H

#include <SFML/Graphics.hpp>
#include "ChessPiece.h"

class ChessBoard {

private:
    sf::Vector2u boardSize;
    sf::RectangleShape boardOutline;
    sf::RectangleShape lightSquare;
    sf::RectangleShape darkSquare;

    ChessPiece bPawn = ChessPiece(sf::Sprite());
    ChessPiece bBishop = ChessPiece(sf::Sprite());
    ChessPiece bKnight = ChessPiece(sf::Sprite());
    ChessPiece bRook = ChessPiece(sf::Sprite());
    ChessPiece bQueen = ChessPiece(sf::Sprite());
    ChessPiece bKing = ChessPiece(sf::Sprite());
    ChessPiece wPawn = ChessPiece(sf::Sprite());
    ChessPiece wBishop = ChessPiece(sf::Sprite());
    ChessPiece wKnight = ChessPiece(sf::Sprite());
    ChessPiece wRook = ChessPiece(sf::Sprite());
    ChessPiece wQueen = ChessPiece(sf::Sprite());
    ChessPiece wKing = ChessPiece(sf::Sprite());


    int squares[64] = {4, 2, 3, 6, 5, 3, 2, 4,
                       1, 1, 1, 1, 1, 1, 1, 1,
                       0, 0, 0, 0, 0, 0, 0, 0,
                       0, 0, 0, 0, 0, 0, 0, 0,
                       0, 0, 0, 0, 0, 0, 0, 0,
                       0, 0, 0, 0, 0, 0, 0, 0,
                       7, 7, 7, 7, 7, 7, 7, 7,
                       10, 8, 9, 11, 12, 9, 8, 10,};

public:

    void initChessPieces();

    ChessBoard(int width, int height);

    void draw(sf::RenderWindow &window);

    void setBoardSize(const sf::Vector2u &boardSize);

    void grabPieces(int x, int y);

    void dropPieces();
};


#endif //AUTOCHESS_CHESSBOARD_H
