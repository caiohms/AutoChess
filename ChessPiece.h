
#ifndef AUTOCHESS_CHESSPIECE_H
#define AUTOCHESS_CHESSPIECE_H

#include <SFML/Graphics.hpp>
#include <set>

class ChessPiece {
public:
    explicit ChessPiece(const std::string &texFile, std::set<int> moves);

    std::set<int> moves;

    ~ChessPiece();

    sf::Sprite sprite;

    void draw(sf::RenderWindow &window);



private:
    sf::Texture texture;
};


#endif //AUTOCHESS_CHESSPIECE_H
