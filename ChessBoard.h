
#ifndef AUTOCHESS_CHESSBOARD_H
#define AUTOCHESS_CHESSBOARD_H


#include <SFML/Graphics.hpp>

class ChessBoard {

private:

    sf::Vector2u boardSize;
    sf::RectangleShape boardOutline;
    sf::RectangleShape lightSquare;
    sf::RectangleShape darkSquare;

    int squares[64] = {1, 1, 1, 1, 1, 1, 0, 0,
                       0, 0, 0, 0, 0, 0, 0, 0,
                       0, 0, 0, 0, 0, 0, 0, 0,
                       0, 0, 0, 0, 0, 0, 0, 0,
                       0, 0, 0, 0, 0, 0, 0, 0,
                       0, 0, 0, 0, 0, 0, 0, 0,
                       0, 0, 0, 0, 0, 0, 0, 0,
                       1, 2, 3, 4, 5, 6, 0, 0};

public:
    ChessBoard(int i, int i1);

    void draw(sf::RenderWindow &window);

    void setBoardSize(const sf::Vector2u &boardSize);
};


#endif //AUTOCHESS_CHESSBOARD_H
