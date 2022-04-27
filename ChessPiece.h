
#ifndef AUTOCHESS_CHESSPIECE_H
#define AUTOCHESS_CHESSPIECE_H

#include <SFML/Graphics.hpp>

class ChessPiece {
public:
    explicit ChessPiece(const std::string &texFile);

    ~ChessPiece();

    sf::Sprite sprite;

    void draw(sf::RenderWindow &window);

private:
    sf::Texture texture;
};


#endif //AUTOCHESS_CHESSPIECE_H
