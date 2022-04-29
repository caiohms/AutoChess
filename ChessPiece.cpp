
#include <iostream>
#include "ChessPiece.h"

void ChessPiece::Init(const std::string &texFile, const std::set<int> &fullMoves) {
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

ChessPiece::ChessPiece(sf::Sprite sprite) {
    this->sprite = sprite;
}

