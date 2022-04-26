//
// Created by Caio on 26/04/2022.
//

#include "ChessPiece.h"

ChessPiece::ChessPiece(const std::string &texFile) {

    if (!texture.loadFromFile(texFile)) {
        throw std::runtime_error("Failed to load texture " + texFile);
    }

    sprite.setTexture(texture);
}

ChessPiece::~ChessPiece() = default;

void ChessPiece::draw(sf::RenderWindow &window) {
    window.draw(sprite);
}
