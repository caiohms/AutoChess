
#include <iostream>
#include "ChessPiece.h"

void ChessPiece::Init(const std::string &texFile) {
    if (!texture.loadFromFile(texFile)) {
        throw std::runtime_error("Failed to load texture " + texFile);
    }

    texture.setSmooth(true);
    sprite.setTexture(texture);
}


void ChessPiece::draw(float edge, float xPos, float yPos, sf::RenderWindow &renderWindow) {
    sprite.setScale(edge / 8.0f / 128.0f, edge / 8.0f / 128.0f);
    sprite.setPosition(xPos, yPos);
    renderWindow.draw(sprite);
}

ChessPiece::ChessPiece() = default;

ChessPiece::PieceColor ChessPiece::getPieceColor(unsigned short pieceCode) {
    unsigned short value = pieceCode & 0b11000000;
    if (value == 0b10000000)
        return PieceColor::BLACK;
    else if (value == 0b01000000)
        return PieceColor::WHITE;
    return PieceColor::BLANK;
}

