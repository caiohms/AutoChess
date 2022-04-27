
#include "ChessBoard.h"

ChessBoard::ChessBoard(int w, int h) {
    this->boardSize.x = w;
    this->boardSize.y = h;

    int minSide = std::min(w, h);

    this->boardOutline.setPosition(10, 10);
    this->boardOutline.setFillColor(sf::Color::Transparent);
    this->boardOutline.setOutlineColor(sf::Color(49, 79, 105));
    this->boardOutline.setOutlineThickness(10);

    this->setBoardSize(sf::Vector2u(minSide, minSide));

    darkSquare.setFillColor(sf::Color(81, 130, 171));
    lightSquare.setFillColor(sf::Color(124, 163, 193));

}

void ChessBoard::draw(sf::RenderWindow &window) {
    window.draw(boardOutline);

    float side = (float) boardSize.x - 20;

    lightSquare.setSize(sf::Vector2f(side / 8, side / 8));
    darkSquare.setSize(sf::Vector2f(side / 8, side / 8));

    for (int i = 0; i < 64; ++i) {
        float xPos = 10 + side / 8 * (i % 8);
        float yPos = 10 + side / 8 * (i / 8);
        lightSquare.setPosition(sf::Vector2f(xPos, yPos));
        darkSquare.setPosition(sf::Vector2f(xPos, yPos));

        if ((i % 2 + (int) (i / 8)) % 2 == 0)
            window.draw(lightSquare);
        else
            window.draw(darkSquare);
    }
}


void ChessBoard::setBoardSize(const sf::Vector2u &boardSize) {
    boardOutline.setSize(sf::Vector2f((float) boardSize.x - 20, (float) boardSize.y - 20));
    ChessBoard::boardSize = boardSize;
}
