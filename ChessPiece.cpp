
#include <iostream>
#include "ChessPiece.h"

/**
 * Loads a piece texture from a file in the hard disk, and converts it into a sprite which is loaded into memory.
 * @param texFile The texture location on disk
 */
void ChessPiece::Init(const std::string &texFile) {
    if (!texture.loadFromFile(texFile)) {
        throw std::runtime_error("Failed to load texture " + texFile);
    }

    texture.setSmooth(true);
    sprite.setTexture(texture);
}

/**
 * Draws a piece onto the window buffer
 * @param edge The board Edge size
 * @param xPos The X position to draw at
 * @param yPos The Y position to draw at
 * @param renderWindow The window object reference
 */
void ChessPiece::draw(float edge, float xPos, float yPos, sf::RenderWindow &renderWindow) {
    sprite.setScale(edge / 8.0f / 128.0f, edge / 8.0f / 128.0f);
    sprite.setPosition(xPos, yPos);
    renderWindow.draw(sprite);
}

ChessPiece::ChessPiece() = default;

/**
 * Gets the piece Color enum value from the unsigned short piece value
 * @param pieceCode The unsigned short piece code value (e.g. White pawn = 0b01000001)
 * @return The PieceColor enum value
 */
inline ChessPiece::PieceColor ChessPiece::getPieceColor(unsigned short pieceCode) {
    unsigned short value = pieceCode & 0b11000000;
    if (value == 0b10000000)
        return PieceColor::BLACK;
    else if (value == 0b01000000)
        return PieceColor::WHITE;
    return PieceColor::BLANK;
}

