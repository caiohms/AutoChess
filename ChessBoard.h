
#ifndef AUTOCHESS_CHESSBOARD_H
#define AUTOCHESS_CHESSBOARD_H

#include <SFML/Graphics.hpp>
#include "ChessPiece.h"

class ChessBoard {

private:
    short selectedSquareIndex = -1;
    short selectedPieceCode = -1;

    int mouseXpos = -1;
    int mouseYpos = -1;

    bool mouseDragging = false;

    std::set<int> possibilty = {};


    sf::Vector2u boardSize;
    sf::RectangleShape boardOutline;
    sf::RectangleShape lightSquare;
    sf::RectangleShape darkSquare;
    sf::RectangleShape selectedSquare;
    sf::RectangleShape possibiltyPiece;

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
                       1, 0, 0, 7, 0, 0, 0, 0,
                       0, 0, 0, 0, 0, 0, 0, 0,
                       0, 0, 0, 2, 0, 0, 0, 0,
                       0, 0, 1, 0, 0, 0, 0, 0,
                       7, 7, 7, 7, 7, 7, 7, 7,
                       10, 8, 9, 12, 11, 9, 8, 10,};

public:

    void initChessPieces();

    ChessBoard(int width, int height);

    void draw(sf::RenderWindow &window);

    void setBoardSize(const sf::Vector2u &size);

    void grabPiece(unsigned int mouseX, unsigned int mouseY);

    void releasePiece(unsigned int mouseX, unsigned int mouseY);

    void drawPiece(int pieceCode, float xPos, float yPos, float boardEdge, sf::RenderWindow &window);

    void setMousePos(int mouseX, int mouseY);

    void possibleMoves(int indexSquare, int codePiece);
};


#endif //AUTOCHESS_CHESSBOARD_H
