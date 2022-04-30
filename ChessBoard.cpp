
#include <iostream>
#include "ChessBoard.h"
#include "ChessPiece.h"
#include <set>
#include <iterator>

void generateMoves(std::set<int> &set);

ChessBoard::ChessBoard(int width, int height) {
    this->boardSize.x = width;
    this->boardSize.y = height;

    int minSide = std::min(width, height);

    this->boardOutline.setPosition(10, 10);
    this->boardOutline.setFillColor(sf::Color::Transparent);
    this->boardOutline.setOutlineColor(sf::Color(49, 79, 105));
    this->boardOutline.setOutlineThickness(10);

    this->setBoardSize(sf::Vector2u(minSide, minSide));

    darkSquare.setFillColor(sf::Color(81, 130, 171));
    lightSquare.setFillColor(sf::Color(124, 163, 193));
    selectedSquare.setFillColor(sf::Color(200, 50, 50));

    targetCircleShape.setOutlineThickness(4);
    targetCircleShape.setOutlineColor(sf::Color(80, 80, 80));

//    possibleSquare.setFillColor(sf::Color(2, 188, 0));

    initTextures();
}

void ChessBoard::draw(sf::RenderWindow &window) {

    window.draw(boardOutline);

    float boardEdge = (float) boardSize.x - 20;
    float squareEdge = boardEdge / 8.0f;

    lightSquare.setSize(sf::Vector2f(squareEdge, squareEdge));
    darkSquare.setSize(sf::Vector2f(squareEdge, squareEdge));
    selectedSquare.setSize(sf::Vector2f(squareEdge, squareEdge));

//    possibleSquare.setSize(sf::Vector2f(squareEdge, squareEdge));

    for (int i = 0; i < 64; ++i) {
        float xPos = 10 + boardEdge / 8 * (float) (i % 8);
        float yPos = 10 + boardEdge / 8 * (float) (i / 8);
        lightSquare.setPosition(sf::Vector2f(xPos, yPos));
        darkSquare.setPosition(sf::Vector2f(xPos, yPos));
        selectedSquare.setPosition(sf::Vector2f(xPos, yPos));

        if (squares[i] > 0) {
            // opponent piece on square
            targetCircleShape.setFillColor(sf::Color::Transparent);
            targetCircleShape.setRadius(squareEdge / 2.2f);
        } else {
            //empty square
            targetCircleShape.setFillColor(sf::Color(80, 80, 80));
            targetCircleShape.setRadius(squareEdge / 10);
        }

        targetCircleShape.setPosition(xPos, yPos);
        targetCircleShape.setOrigin(-squareEdge / 2 + targetCircleShape.getRadius(),
                                    -squareEdge / 2 + targetCircleShape.getRadius());


        //        possibleSquare.setPosition(sf::Vector2f(xPos, yPos));

        if ((i % 2 + (int) (i / 8)) % 2 == 0)
            window.draw(lightSquare);
        else
            window.draw(darkSquare);

        if (selectedSquareIndex == i)
            window.draw(selectedSquare);

        ChessBoard::drawPiece(squares[i], xPos, yPos, boardEdge, window);

        if (possibilities.count(i)) {
            window.draw(targetCircleShape);
        }
    }


    if (selectedPieceCode > 0) {
        ChessBoard::drawPiece(selectedPieceCode, mouseXpos - squareEdge / 2, mouseYpos - squareEdge / 2, boardEdge,
                              window);
    }
}

void ChessBoard::setBoardSize(const sf::Vector2u &size) {
    boardOutline.setSize(sf::Vector2f((float) size.x - 20, (float) size.y - 20));
    ChessBoard::boardSize = size;
}

void ChessBoard::initTextures() {
    bPawn.Init("resources\\sprites\\black-pawn.png");
    bBishop.Init("resources\\sprites\\black-bishop.png");
    bKnight.Init("resources\\sprites\\black-knight.png");
    bRook.Init("resources\\sprites\\black-rook.png");
    bQueen.Init("resources\\sprites\\black-queen.png");
    bKing.Init("resources\\sprites\\black-king.png");

    wPawn.Init("resources\\sprites\\white-pawn.png");
    wBishop.Init("resources\\sprites\\white-bishop.png");
    wKnight.Init("resources\\sprites\\white-knight.png");
    wRook.Init("resources\\sprites\\white-rook.png");
    wQueen.Init("resources\\sprites\\white-queen.png");
    wKing.Init("resources\\sprites\\white-king.png");
}

void ChessBoard::grabPiece(unsigned int mouseX, unsigned int mouseY) {
    mouseDragging = true;

    if (mouseX + 10 > boardSize.x || mouseY + 10 > boardSize.y || mouseX < 10 || mouseY < 10) {
        selectedSquareIndex = 0;
        return;
    }

    selectedSquareIndex = getSquareUnderMousePos(mouseX, mouseY);

    selectedPieceCode = squares[selectedSquareIndex];

    possibleMoves(selectedSquareIndex, selectedPieceCode);
}

void ChessBoard::releasePiece(unsigned int mouseX, unsigned int mouseY) {
    if (selectedPieceCode == 0) return;

    mouseDragging = false;

    int hoveringSquare = getSquareUnderMousePos(mouseX, mouseY);

//    if (hoveringSquare)

    squares[selectedSquareIndex] = 0;
    squares[hoveringSquare] = selectedPieceCode;
    selectedPieceCode = 0;
    selectedSquareIndex = -1;
    possibilities = {};
}

int ChessBoard::getSquareUnderMousePos(unsigned int mouseX, unsigned int mouseY) {
    unsigned short squareX = (mouseX) / ((boardSize.x) / 8);
    unsigned short squareY = (mouseY) / ((boardSize.y) / 8);

    return squareY * 8 + squareX;
}

void ChessBoard::possibleMoves(int currentSquare, int pieceCode) {
    possibilities.clear();
    if (pieceCode == 0) return;

    int spacesAbove = currentSquare / 8;
    int spacesBelow = 7 - currentSquare / 8;
    int spacesLeft = (currentSquare % 8);
    int spacesRight = 7 - (currentSquare % 8);

    int minTopLeft = std::min(spacesAbove, spacesLeft);
    int minTopRight = std::min(spacesAbove, spacesRight);
    int minBottomLeft = std::min(spacesBelow, spacesLeft);
    int minBottomRight = std::min(spacesBelow, spacesRight);

    switch (pieceCode & 0b00111111) {
        case 0b1: {
            // pawn
            if ((pieceCode & 0b11000000) == 0b10000000) {
                if (minBottomLeft) {
                    possibilities.insert(currentSquare + 1 * 7);
                }
                if (minBottomRight) {
                    possibilities.insert(currentSquare + 1 * 9);
                }
                if (spacesBelow) {
                    possibilities.insert(currentSquare + 1 * 8);
                }
                if (spacesBelow) {
                    possibilities.insert(currentSquare + 2 * 8);
                }
            } else {
                if (minTopLeft) {
                    possibilities.insert(currentSquare - 1 * 9);
                }
                if (minTopRight) {
                    possibilities.insert(currentSquare - 1 * 7);
                }
                if (spacesAbove) {
                    possibilities.insert(currentSquare - 1 * 8);
                }
                if (spacesAbove) {
                    possibilities.insert(currentSquare - 2 * 8);
                }
            }
            break;
        }
        case 0b10: {
            // knight
            possibilities.insert({});
            break;
        }
        case 0b100: {
            // bishop
            for (int i = 1; i <= minTopLeft; ++i) {
                possibilities.insert(currentSquare - i * 9);
            }
            for (int i = 1; i <= minTopRight; ++i) {
                possibilities.insert(currentSquare - i * 7);
            }
            for (int i = 1; i <= minBottomLeft; ++i) {
                possibilities.insert(currentSquare + i * 7);
            }
            for (int i = 1; i <= minBottomRight; ++i) {
                possibilities.insert(currentSquare + i * 9);
            }
            break;
        }
        case 0b1000: {
            // rook
            for (int i = 1; i <= spacesAbove; ++i) {
                possibilities.insert(currentSquare - i * 8);
            }
            for (int i = 1; i <= spacesBelow; ++i) {
                possibilities.insert(currentSquare + i * 8);
            }
            for (int i = 1; i <= spacesLeft; ++i) {
                possibilities.insert(currentSquare - i);
            }
            for (int i = 1; i <= spacesRight; ++i) {
                possibilities.insert(currentSquare + i);
            }
            break;
        }
        case 0b10000: {
            // queen
            for (int i = 1; i <= minTopLeft; ++i) {
                possibilities.insert(currentSquare - i * 9);
            }
            for (int i = 1; i <= minTopRight; ++i) {
                possibilities.insert(currentSquare - i * 7);
            }
            for (int i = 1; i <= minBottomLeft; ++i) {
                possibilities.insert(currentSquare + i * 7);
            }
            for (int i = 1; i <= minBottomRight; ++i) {
                possibilities.insert(currentSquare + i * 9);
            }
            for (int i = 1; i <= spacesAbove; ++i) {
                possibilities.insert(currentSquare - i * 8);
            }
            for (int i = 1; i <= spacesBelow; ++i) {
                possibilities.insert(currentSquare + i * 8);
            }
            for (int i = 1; i <= spacesLeft; ++i) {
                possibilities.insert(currentSquare - i);
            }
            for (int i = 1; i <= spacesRight; ++i) {
                possibilities.insert(currentSquare + i);
            }
            break;
        }
        case 0b100000: {
            // king
            if (minBottomLeft) {
                possibilities.insert(currentSquare + 1 * 7);
            }
            if (minBottomRight) {
                possibilities.insert(currentSquare + 1 * 9);
            }
            if (spacesBelow) {
                possibilities.insert(currentSquare + 1 * 8);
            }
            if (minTopLeft) {
                possibilities.insert(currentSquare - 1 * 9);
            }
            if (minTopRight) {
                possibilities.insert(currentSquare - 1 * 7);
            }
            if (spacesAbove) {
                possibilities.insert(currentSquare - 1 * 8);
            }
            if (spacesLeft) {
                possibilities.insert(currentSquare - 1);
            }
            if (spacesRight) {
                possibilities.insert(currentSquare + 1);
            }
            break;
        }
    }
}

void
ChessBoard::drawPiece(unsigned short pieceCode, float xPos, float yPos, float boardEdge, sf::RenderWindow &window) {
    switch (pieceCode) {
        case 0b10000001:
            bPawn.draw(boardEdge, xPos, yPos, window);
            break;
        case 0b10000010:
            bKnight.draw(boardEdge, xPos, yPos, window);
            break;
        case 0b10000100:
            bBishop.draw(boardEdge, xPos, yPos, window);
            break;
        case 0b10001000:
            bRook.draw(boardEdge, xPos, yPos, window);
            break;
        case 0b10010000:
            bQueen.draw(boardEdge, xPos, yPos, window);
            break;
        case 0b10100000:
            bKing.draw(boardEdge, xPos, yPos, window);
            break;
        case 0b01000001:
            wPawn.draw(boardEdge, xPos, yPos, window);
            break;
        case 0b01000010:
            wKnight.draw(boardEdge, xPos, yPos, window);
            break;
        case 0b01000100:
            wBishop.draw(boardEdge, xPos, yPos, window);
            break;
        case 0b01001000:
            wRook.draw(boardEdge, xPos, yPos, window);
            break;
        case 0b01010000:
            wQueen.draw(boardEdge, xPos, yPos, window);
            break;
        case 0b01100000:
            wKing.draw(boardEdge, xPos, yPos, window);
            break;
        default:
            break;
    }
}

void ChessBoard::setMousePos(int mouseX, int mouseY) {
    this->mouseXpos = mouseX;
    this->mouseYpos = mouseY;
}
