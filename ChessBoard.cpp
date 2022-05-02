#include "ChessBoard.h"
#include "ChessPiece.h"
#include <set>
#include <iostream>


ChessBoard::ChessBoard(int width, int height, sf::Font font) {

    this->boardSize.x = width;
    this->boardSize.y = height;
    this->font = font;

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
    sf::Text t;
    t.setFont(font);

    window.draw(boardOutline);

    float boardEdge = (float) boardSize.x - 20;
    float squareEdge = boardEdge / 8.0f;

    lightSquare.setSize(sf::Vector2f(squareEdge, squareEdge));
    darkSquare.setSize(sf::Vector2f(squareEdge, squareEdge));
    selectedSquare.setSize(sf::Vector2f(squareEdge, squareEdge));

//    possibleSquare.setSize(sf::Vector2f(squareEdge, squareEdge));

    for (int i = 0; i < 64; ++i) {
        t.setString(std::to_string(i));

        float xPos = 10 + boardEdge / 8 * (float) (i % 8);
        float yPos = 10 + boardEdge / 8 * (float) (i / 8);
        lightSquare.setPosition(sf::Vector2f(xPos, yPos));
        darkSquare.setPosition(sf::Vector2f(xPos, yPos));
        selectedSquare.setPosition(sf::Vector2f(xPos, yPos));
        t.setPosition(xPos, yPos);

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
        if (attackedSquares.count(i)) {
            window.draw(targetCircleShape);
        }


        window.draw(t);
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
        selectedSquareIndex = -1;
        return;
    }

    selectedSquareIndex = getSquareUnderMousePos(mouseX, mouseY);

    selectedPieceCode = squares[selectedSquareIndex];

    possibleMoves(selectedSquareIndex);
}

bool ChessBoard::isChecked(unsigned short color, unsigned short board[64]) {
    for (int i = 0; i < 64; i++) {
        if (((board[i] & 0b11000000) != color) && board[i] != 0) {
            possibleMoves(i);
            for (int j: possibilities) {
                attackedSquares.insert(j);
            }

            for (int x: attackedSquares) {
                if ((board[x] & 0b00100000) == 0b00100000 && (board[x] & 0b11000000) == color) {
                    attackedSquares.clear();
                    return true;
                }
            }
            attackedSquares.clear();
        }
    }
    return false;
}

void ChessBoard::releasePiece(unsigned int mouseX, unsigned int mouseY) {
    if (selectedPieceCode == 0) return;
    mouseDragging = false;
    int hoveringSquare = getSquareUnderMousePos(mouseX, mouseY);
    unsigned short pieceColor = ChessPiece::getPieceColor(selectedPieceCode);
    unsigned short newBoard[64];
    for (int j = 0; j < 64; j++) {
        newBoard[j] = squares[j];
    }
    newBoard[selectedSquareIndex] = 0;
    newBoard[hoveringSquare] = selectedPieceCode;
    for (int i: possibilities) {
        if (i == hoveringSquare) {
            short hoveringSquarePieceCode = squares[hoveringSquare];
            if (!isChecked(pieceColor, newBoard)) {
                if (
                        ChessPiece::getPieceColor(selectedPieceCode) == ChessPiece::PieceColor::BLACK &&
                        ChessPiece::getPieceColor(hoveringSquarePieceCode) == ChessPiece::PieceColor::WHITE ||
                        squares[hoveringSquare] == 0) {

                    squares[selectedSquareIndex] = 0;
                    if (hoveringSquare >= 56 && hoveringSquare <= 63 && selectedPieceCode == 0b10000001) {
                        squares[hoveringSquare] = 0b10010000;
                    } else {
                        unsigned short piece = squares[hoveringSquare];
                        squares[hoveringSquare] = selectedPieceCode;
                        if (isChecked(pieceColor, squares)){
                            squares[hoveringSquare] =piece;
                            squares[selectedSquareIndex]=selectedPieceCode;
                        }
                    }
                    selectedPieceCode = 0;
                    selectedSquareIndex = -1;
                    possibilities.clear();
                } else if (
                        ChessPiece::getPieceColor(selectedPieceCode) == ChessPiece::PieceColor::WHITE &&
                        ChessPiece::getPieceColor(hoveringSquarePieceCode) == ChessPiece::PieceColor::BLACK) {
                    squares[selectedSquareIndex] = 0;
                    if (hoveringSquare >= 0 && hoveringSquare <= 7 && selectedPieceCode == 0b01000001) {
                        squares[hoveringSquare] = 0b01010000;
                    } else {
                        unsigned short piece = squares[hoveringSquare];
                        squares[hoveringSquare] = selectedPieceCode;
                        if (isChecked(pieceColor, squares)){
                            squares[hoveringSquare] =piece;
                            squares[selectedSquareIndex]=selectedPieceCode;
                        }
                    }
                    selectedPieceCode = 0;
                    selectedSquareIndex = -1;
                    possibilities.clear();
                }
            }

        }

    }

    selectedPieceCode = 0;
    possibilities.clear();
}

int ChessBoard::getSquareUnderMousePos(unsigned int mouseX, unsigned int mouseY) {
    unsigned short squareX = (mouseX) / ((boardSize.x) / 8);
    unsigned short squareY = (mouseY) / ((boardSize.y) / 8);

    return squareY * 8 + squareX;
}

void ChessBoard::possibleMoves(int currentSquare) {

    int pieceCode = squares[currentSquare];

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

    ChessPiece::PieceColor selectedPieceColor = ChessPiece::getPieceColor(pieceCode);
    ChessPiece::PieceColor oppositePieceColor = ChessPiece::getPieceColor(pieceCode ^ 0b11000000 << 6 >> 6);

    int target;

    switch (pieceCode & 0b00111111) {
        case 0b1: {
            // pawn
            if ((pieceCode & 0b11000000) == 0b10000000) {
                if (minBottomLeft && squares[currentSquare + 7] != 0) {
                    target = currentSquare + 7;
                    addTarget(target, selectedPieceColor, oppositePieceColor);
//                    if (spacesBelow==0){
//                        squares[currentSquare] = 0b01010000;
//                    }
                }
                if (minBottomRight && squares[currentSquare + 9] != 0) {
                    target = currentSquare + 9;
                    addTarget(target, selectedPieceColor, oppositePieceColor);
                }
                if (spacesBelow && squares[currentSquare + 8] == 0) {
                    target = currentSquare + 8;
                    addTarget(target, selectedPieceColor, oppositePieceColor);
                }
                if (spacesBelow == 6 && squares[currentSquare + 8] == 0 && squares[currentSquare + 16] == 0) {
                    target = currentSquare + 16;
                    addTarget(target, selectedPieceColor, oppositePieceColor);
                }

            } else {
                if (minTopLeft && squares[currentSquare - 9] != 0) {
                    target = currentSquare - 9;
                    addTarget(target, selectedPieceColor, oppositePieceColor);
                }
                if (minTopRight && squares[currentSquare - 7] != 0) {
                    target = currentSquare - 7;
                    addTarget(target, selectedPieceColor, oppositePieceColor);
                }
                if (spacesAbove && squares[currentSquare - 8] == 0) {
                    target = currentSquare - 8;
                    addTarget(target, selectedPieceColor, oppositePieceColor);
                }
                if (spacesAbove == 6 && squares[currentSquare - 8] == 0 && squares[currentSquare - 16] == 0) {
                    target = currentSquare - 16;
                    addTarget(target, selectedPieceColor, oppositePieceColor);
                }
            }
            break;
        }
        case 0b10: {
            // knight
            if (spacesAbove > 1) {
                if (spacesLeft > 1) {
                    target = currentSquare - 10;
                    addTarget(target, selectedPieceColor, oppositePieceColor);
                }
                if (spacesRight > 1) {
                    target = currentSquare - 6;
                    addTarget(target, selectedPieceColor, oppositePieceColor);
                }
                if (spacesLeft > 0) {
                    target = currentSquare - 17;
                    addTarget(target, selectedPieceColor, oppositePieceColor);
                }
                if (spacesRight > 0) {
                    target = currentSquare - 15;
                    addTarget(target, selectedPieceColor, oppositePieceColor);
                }
            } else if (spacesAbove > 0) {
                if (spacesLeft > 1) {
                    target = currentSquare - 10;
                    addTarget(target, selectedPieceColor, oppositePieceColor);
                }
                if (spacesRight > 1) {
                    target = currentSquare - 6;
                    addTarget(target, selectedPieceColor, oppositePieceColor);
                }
            }

            if (spacesBelow > 1) {
                if (spacesLeft > 1) {
                    target = currentSquare + 6;
                    addTarget(target, selectedPieceColor, oppositePieceColor);
                }
                if (spacesRight > 1) {
                    target = currentSquare + 10;
                    addTarget(target, selectedPieceColor, oppositePieceColor);
                }
                if (spacesLeft > 0) {
                    target = currentSquare + 15;
                    addTarget(target, selectedPieceColor, oppositePieceColor);
                }
                if (spacesRight > 0) {
                    target = currentSquare + 17;
                    addTarget(target, selectedPieceColor, oppositePieceColor);
                }
            } else if (spacesBelow > 0) {
                if (spacesLeft > 1) {
                    target = currentSquare + 6;
                    addTarget(target, selectedPieceColor, oppositePieceColor);
                }
                if (spacesRight > 1) {
                    target = currentSquare + 10;
                    addTarget(target, selectedPieceColor, oppositePieceColor);
                }
            }
            break;
        }
        case 0b100: {
            // bishop
            for (int i = 1; i <= minTopLeft; ++i) {
                target = currentSquare - i * 9;
                if (!addTarget(target, selectedPieceColor, oppositePieceColor))
                    break;
            }
            for (int i = 1; i <= minTopRight; ++i) {
                target = currentSquare - i * 7;
                if (!addTarget(target, selectedPieceColor, oppositePieceColor))
                    break;
            }
            for (int i = 1; i <= minBottomLeft; ++i) {
                target = currentSquare + i * 7;
                if (!addTarget(target, selectedPieceColor, oppositePieceColor))
                    break;
            }
            for (int i = 1; i <= minBottomRight; ++i) {
                target = currentSquare + i * 9;
                if (!addTarget(target, selectedPieceColor, oppositePieceColor))
                    break;
            }
            break;
        }
        case 0b1000: {
            // rook
            for (int i = 1; i <= spacesAbove; ++i) {
                target = currentSquare - i * 8;
                if (!addTarget(target, selectedPieceColor, oppositePieceColor))
                    break;
            }
            for (int i = 1; i <= spacesBelow; ++i) {
                target = currentSquare + i * 8;
                if (!addTarget(target, selectedPieceColor, oppositePieceColor))
                    break;
            }
            for (int i = 1; i <= spacesLeft; ++i) {
                target = currentSquare - i;
                if (!addTarget(target, selectedPieceColor, oppositePieceColor))
                    break;
            }
            for (int i = 1; i <= spacesRight; ++i) {
                target = currentSquare + i;
                if (!addTarget(target, selectedPieceColor, oppositePieceColor))
                    break;
            }
            break;
        }
        case 0b10000: {
            // queen
            for (int i = 1; i <= minTopLeft; ++i) {
                target = currentSquare - i * 9;
                if (!addTarget(target, selectedPieceColor, oppositePieceColor))
                    break;
            }
            for (int i = 1; i <= minTopRight; ++i) {
                target = currentSquare - i * 7;
                if (!addTarget(target, selectedPieceColor, oppositePieceColor))
                    break;
            }
            for (int i = 1; i <= minBottomLeft; ++i) {
                target = currentSquare + i * 7;
                if (!addTarget(target, selectedPieceColor, oppositePieceColor))
                    break;
            }
            for (int i = 1; i <= minBottomRight; ++i) {
                target = currentSquare + i * 9;
                if (!addTarget(target, selectedPieceColor, oppositePieceColor))
                    break;
            }
            for (int i = 1; i <= spacesAbove; ++i) {
                target = currentSquare - i * 8;
                if (!addTarget(target, selectedPieceColor, oppositePieceColor))
                    break;
            }
            for (int i = 1; i <= spacesBelow; ++i) {
                target = currentSquare + i * 8;
                if (!addTarget(target, selectedPieceColor, oppositePieceColor))
                    break;
            }
            for (int i = 1; i <= spacesLeft; ++i) {
                target = currentSquare - i;
                if (!addTarget(target, selectedPieceColor, oppositePieceColor))
                    break;
            }
            for (int i = 1; i <= spacesRight; ++i) {
                target = currentSquare + i;
                if (!addTarget(target, selectedPieceColor, oppositePieceColor))
                    break;
            }
            break;
        }
        case 0b100000: {
            // king
            if (minBottomLeft) {
                target = currentSquare + 7;
                addTarget(target, selectedPieceColor, oppositePieceColor);

            }
            if (minBottomRight) {
                target = currentSquare + 9;
                addTarget(target, selectedPieceColor, oppositePieceColor);
            }
            if (minTopLeft) {
                target = currentSquare - 9;
                addTarget(target, selectedPieceColor, oppositePieceColor);
            }
            if (minTopRight) {
                target = currentSquare - 7;
                addTarget(target, selectedPieceColor, oppositePieceColor);
            }
            if (spacesAbove) {
                target = currentSquare - 8;
                addTarget(target, selectedPieceColor, oppositePieceColor);
            }
            if (spacesBelow) {
                target = currentSquare + 8;
                addTarget(target, selectedPieceColor, oppositePieceColor);
            }
            if (spacesLeft) {
                target = currentSquare - 1;
                addTarget(target, selectedPieceColor, oppositePieceColor);
            }
            if (spacesRight) {
                target = currentSquare + 1;
                addTarget(target, selectedPieceColor, oppositePieceColor);
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

bool ChessBoard::addTarget(unsigned short target, ChessPiece::PieceColor selectedPieceColor,
                           ChessPiece::PieceColor oppositePieceColor) {
    ChessPiece::PieceColor targetPieceColor = ChessPiece::getPieceColor(squares[target]);
    if (targetPieceColor == selectedPieceColor)
        return false;
    else if (targetPieceColor == oppositePieceColor) {
        possibilities.insert(target);
        return false;
    } else
        possibilities.insert(target);
    return true;
}