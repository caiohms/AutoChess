
#ifndef AUTOCHESS_CHESSGAME_H
#define AUTOCHESS_CHESSGAME_H


#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "ChessBoard.h"

class ChessGame {
private:
    enum PlayerTurn {
        WHITE, BLACK
    };

    ChessBoard board;

    PlayerTurn playerTurn = WHITE;

    bool bCastleKingSide = true;
    bool bCastleQueenSide = true;
    bool wCastleKingSide = true;
    bool wCastleQueenSide = true;

    std::vector<ChessPiece> bCapturedPieces = {};
    std::vector<ChessPiece> wCapturedPieces = {};

    double evalValue = 0.0;


public:
    explicit ChessGame(ChessBoard &chessBoard);

    void setMousePos(int x, int y);

    void grabPiece(int x, int y);

    void releasePiece(int x, int y);

    void draw(sf::RenderWindow &window);

    const unsigned short *getBoardSquares();

    void setBoardSize(sf::Vector2<unsigned int> vector2);
};


#endif //AUTOCHESS_CHESSGAME_H