
#ifndef AUTOCHESS_CHESSGAME_H
#define AUTOCHESS_CHESSGAME_H

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "ChessBoard.h"
#include "AI.h"

class ChessGame {
private:
    ChessBoard & board;

    AI & ai;

    bool & playerTurn;

    double evalValue = 0.0;

public:
    explicit ChessGame(ChessBoard &chessBoard, bool &turn, AI &ai);

    void setMousePos(int x, int y);

    void grabPiece(int x, int y);

    void releasePiece(int x, int y);

    void draw(sf::RenderWindow &window);

    const unsigned short *getBoardSquares();

    void setBoardSize(sf::Vector2<unsigned int> vector2);

    long moveMaker(int i, sf::RenderWindow &window, bool playerTurn, std::ofstream &ofstream);

    void makeAutomatedMove(int from, int to);
};


#endif //AUTOCHESS_CHESSGAME_H