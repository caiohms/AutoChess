
#ifndef AUTOCHESS_CHESSPIECE_H
#define AUTOCHESS_CHESSPIECE_H

#include <SFML/Graphics.hpp>
#include <set>

class ChessPiece {
public:

    explicit ChessPiece(sf::Sprite sprite);

    void Init(const std::string &texFile);

    void draw(float edge, float d, float d1, sf::RenderWindow &renderWindow);

private:
    sf::Sprite sprite;
    sf::Texture texture;
};


#endif //AUTOCHESS_CHESSPIECE_H
