
#ifndef AUTOCHESS_CHESSPIECE_H
#define AUTOCHESS_CHESSPIECE_H

#include <SFML/Graphics.hpp>
#include <set>

class ChessPiece {
public:

    explicit ChessPiece(sf::Sprite sprite);

    void Init(const std::string &texFile);

    void draw(float edge, float d, float d1, sf::RenderWindow &renderWindow);

    enum PieceColor {BLACK = 0b10000000, WHITE = 0b01000000, BLANK};

    static PieceColor getPieceColor(unsigned short pieceCode);

private:
    sf::Sprite sprite;
    sf::Texture texture;
};


#endif //AUTOCHESS_CHESSPIECE_H
