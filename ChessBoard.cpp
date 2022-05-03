#include "ChessBoard.h"
#include "ChessPiece.h"
#include <set>
#include <iostream>

enum PieceFromCode {
    Pawn = 0b1,
    Knight = 0b10,
    Bishop = 0b100,
    Rook = 0b1000,
    Queen = 0b10000,
    King = 0b100000,
};

ChessBoard::ChessBoard(int width, int height, const sf::Font &font, PlayerTurn &turn) : turn(turn) {
    this->boardSize.x = width;
    this->boardSize.y = height;
    this->font = font;

    int minSide = std::min(width, height);

    this->boardOutline.setPosition(10, 10);
    this->boardOutline.setFillColor(sf::Color::Transparent);
    this->boardOutline.setOutlineColor(sf::Color(49, 79, 105));
    this->boardOutline.setOutlineThickness(10);

    this->setBoardSize(sf::Vector2u(minSide, minSide));

    darkRect.setFillColor(sf::Color(81, 130, 171));
    lightRect.setFillColor(sf::Color(124, 163, 193));
    selectedRect.setFillColor(sf::Color(50, 200, 50));

    targetCircleShape.setOutlineThickness(4);
    targetCircleShape.setOutlineColor(sf::Color(80, 80, 80));

    lightTargetedRect.setFillColor(sf::Color(220, 100, 100));
    darkTargetedRect.setFillColor(sf::Color(170, 50, 50));

    initTextures();
}

void ChessBoard::draw(sf::RenderWindow &window) {
    sf::Text t;
    t.setFont(font);

    window.draw(boardOutline);

    float boardEdge = (float) boardSize.x - 20;
    float squareEdge = boardEdge / 8.0f;

    lightRect.setSize(sf::Vector2f(squareEdge, squareEdge));
    darkRect.setSize(sf::Vector2f(squareEdge, squareEdge));
    selectedRect.setSize(sf::Vector2f(squareEdge, squareEdge));

    lightTargetedRect.setSize(sf::Vector2f(squareEdge, squareEdge));
    darkTargetedRect.setSize(sf::Vector2f(squareEdge, squareEdge));

    for (int i = 0; i < 64; ++i) {
        t.setString(std::to_string(i));

        float xPos = 10 + boardEdge / 8 * (float) (i % 8);
        float yPos = 10 + boardEdge / 8 * (float) (i / 8);
        lightRect.setPosition(sf::Vector2f(xPos, yPos));
        darkRect.setPosition(sf::Vector2f(xPos, yPos));
        selectedRect.setPosition(sf::Vector2f(xPos, yPos));
        lightTargetedRect.setPosition(sf::Vector2f(xPos, yPos));
        darkTargetedRect.setPosition(sf::Vector2f(xPos, yPos));

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


        if ((i % 2 + (int) (i / 8)) % 2 == 0)
            window.draw(lightRect);
        else
            window.draw(darkRect);

        if (selectedSquareIndex == i && selectedPieceCode != 0)
            window.draw(selectedRect);

//        if (attackedSquaresDraw.contains(i) && selectedPieceCode != 0) {
//            if ((i % 2 + (int) (i / 8)) % 2 == 0)
//                window.draw(lightTargetedRect);
//            else
//                window.draw(darkTargetedRect);
//        }

        ChessBoard::drawPiece(squares[i], xPos, yPos, boardEdge, window);

        if (possibilities.contains(i) && selectedPieceCode != 0) {
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

    if (mouseX + 10 > boardSize.x || mouseY + 10 > boardSize.y || mouseX < 10 || mouseY < 10) {
        selectedSquareIndex = -1;
        return;
    }

    selectedSquareIndex = getSquareUnderMousePos(mouseX, mouseY);
    if ((getColorFromPieceCode(squares[selectedSquareIndex]) == ChessPiece::PieceColor::WHITE && turn)
        || (getColorFromPieceCode(squares[selectedSquareIndex]) == ChessPiece::PieceColor::BLACK && !turn)) {
        selectedPieceCode = squares[selectedSquareIndex];
        possibilities.clear();
        attackedSquaresDraw.clear();
        possibleMoves(selectedSquareIndex, false, possibilities);
    } else {
        if (turn)
            std::cout << "White's turn" << std::endl;
        else
            std::cout << "Black's turn" << std::endl;
    }
}

void ChessBoard::releasePiece(unsigned int mouseX, unsigned int mouseY) {
    if (selectedPieceCode == 0) return;

    int targetSquareIndex = getSquareUnderMousePos(mouseX, mouseY);

    if (targetSquareIndex == selectedSquareIndex) {
        selectedPieceCode = 0;
        return;
    };

    ChessPiece::PieceColor selectedPieceColor = ChessPiece::getPieceColor(selectedPieceCode);

    if (possibilities.contains(targetSquareIndex)) {
        squares[selectedSquareIndex] = 0;
        if ((targetSquareIndex / 8 == 7 || targetSquareIndex / 8 == 0) &&
            (selectedPieceCode & 0b00000001) == 0b00000001) {
            // promote
            // promotionPrompt(targetSquareIndex);
            squares[targetSquareIndex] = 0b00010000 | selectedPieceColor;
        } else {
            squares[targetSquareIndex] = selectedPieceCode;

            // send tower back if castling
            if (wCastleKingSide && selectedSquareIndex == 60 && targetSquareIndex == 62){
                //do castle white king, king side
                squares[63] = 0;
                squares[61] = ChessPiece::PieceColor::WHITE | 0b1000;
            }
            if (wCastleQueenSide && selectedSquareIndex == 60 && targetSquareIndex == 58){
                //do castle white king, queen side
                squares[56] = 0;
                squares[59] = ChessPiece::PieceColor::WHITE | 0b1000;
            }
            if (bCastleKingSide && selectedSquareIndex == 4 && targetSquareIndex == 6){
                //do castle black king, king side
                squares[7] = 0;
                squares[5] = ChessPiece::PieceColor::BLACK | 0b1000;
            }
            if (bCastleQueenSide && selectedSquareIndex == 4 && targetSquareIndex == 2){
                //do castle black king, queen side
                squares[0] = 0;
                squares[3] = ChessPiece::PieceColor::BLACK | 0b1000;
            }

            if (selectedSquareIndex == 63 || selectedSquareIndex == 60 || targetSquareIndex == 63)
                wCastleKingSide = false;
            if (selectedSquareIndex == 56 || selectedSquareIndex == 60 || targetSquareIndex == 56)
                wCastleQueenSide = false;
            if (selectedSquareIndex == 0 || selectedSquareIndex == 3 || targetSquareIndex == 0)
                bCastleKingSide = false;
            if (selectedSquareIndex == 7 || selectedSquareIndex == 3 || targetSquareIndex == 7)
                bCastleQueenSide = false;

            if (abs(selectedSquareIndex - targetSquareIndex) == 16 && (selectedPieceCode & 0b00000001) == Pawn) {
                // if pawn moved two squares
                // mark middle square as potential target
                unsigned short row = selectedSquareIndex / 8;
                enPassantEnabledSquare = (row == 1) ? selectedSquareIndex + 8 : selectedSquareIndex - 8;

            } else if (targetSquareIndex == enPassantEnabledSquare && (selectedPieceCode & 0b00000001) == Pawn) {
                // if a pawn attacks an enPassantEnabledSquare, an en passant occurred. Opponent pawn must be removed.
                unsigned short row = targetSquareIndex / 8;
                squares[targetSquareIndex + ((row == 2) ? 8 : -8)] = 0;
            }

            turn = (turn == WHITE) ? BLACK : WHITE;
        }

        selectedSquareIndex = -1;
    }

    selectedPieceCode = 0;
    possibilities.clear();
    attackedSquaresDraw.clear();

}

bool ChessBoard::isChecked(unsigned int kingSquare) {
    unsigned short selectedColor = getColorFromPieceCode(selectedPieceCode);
    if (getSquaresAttackedByOpponent().contains(kingSquare)) return true;
    return false;
}

inline unsigned int ChessBoard::getColorFromPieceCode(unsigned short selectedPieceCode) {
    unsigned int selectedColor = selectedPieceCode & 0b11000000;
    return selectedColor;
}

int ChessBoard::getSquareUnderMousePos(unsigned int mouseX, unsigned int mouseY) {
    unsigned short squareX = (mouseX) / ((boardSize.x) / 8);
    unsigned short squareY = (mouseY) / ((boardSize.y) / 8);

    return squareY * 8 + squareX;
}

void ChessBoard::possibleMoves(int currentSquare, bool checkingCheck, std::unordered_set<unsigned short> &targetSet) {

    int pieceCode = squares[currentSquare];

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

    int targetSquare;

    switch (pieceCode & 0b00111111) {
        case 0b1: {
            // pawn
            if (selectedPieceColor == ChessPiece::PieceColor::BLACK) {
                if (minBottomLeft && squares[currentSquare + 7] != 0 ||
                    currentSquare + 7 == enPassantEnabledSquare) {
                    targetSquare = currentSquare + 7;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                              targetSet);
                }
                if (minBottomRight && squares[currentSquare + 9] != 0 ||
                    currentSquare + 9 == enPassantEnabledSquare) {
                    targetSquare = currentSquare + 9;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                              targetSet);
                }
                if (spacesBelow && squares[currentSquare + 8] == 0) {
                    targetSquare = currentSquare + 8;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                              targetSet);
                }
                if (spacesBelow == 6 && squares[currentSquare + 8] == 0 && squares[currentSquare + 16] == 0) {
                    targetSquare = currentSquare + 16;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                              targetSet);
                }

            } else {
                // white pawn
                if ((minTopLeft && squares[currentSquare - 9] != 0) ||
                    currentSquare - 9 == enPassantEnabledSquare) {

                    targetSquare = currentSquare - 9;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                              targetSet);
                }
                if (minTopRight && squares[currentSquare - 7] != 0 ||
                    currentSquare - 7 == enPassantEnabledSquare) {

                    targetSquare = currentSquare - 7;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                              targetSet);
                }
                if (spacesAbove && squares[currentSquare - 8] == 0) {
                    targetSquare = currentSquare - 8;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                              targetSet);
                }
                if (spacesAbove == 6 && squares[currentSquare - 8] == 0 && squares[currentSquare - 16] == 0) {
                    targetSquare = currentSquare - 16;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                              targetSet);
                }
            }
            break;
        }
        case 0b10: {
            // knight
            if (spacesAbove > 1) {
                if (spacesLeft > 1) {
                    targetSquare = currentSquare - 10;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                              targetSet);
                }
                if (spacesRight > 1) {
                    targetSquare = currentSquare - 6;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                              targetSet);
                }
                if (spacesLeft > 0) {
                    targetSquare = currentSquare - 17;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                              targetSet);
                }
                if (spacesRight > 0) {
                    targetSquare = currentSquare - 15;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                              targetSet);
                }
            } else if (spacesAbove > 0) {
                if (spacesLeft > 1) {
                    targetSquare = currentSquare - 10;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                              targetSet);
                }
                if (spacesRight > 1) {
                    targetSquare = currentSquare - 6;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                              targetSet);
                }
            }

            if (spacesBelow > 1) {
                if (spacesLeft > 1) {
                    targetSquare = currentSquare + 6;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                              targetSet);
                }
                if (spacesRight > 1) {
                    targetSquare = currentSquare + 10;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                              targetSet);
                }
                if (spacesLeft > 0) {
                    targetSquare = currentSquare + 15;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                              targetSet);
                }
                if (spacesRight > 0) {
                    targetSquare = currentSquare + 17;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                              targetSet);
                }
            } else if (spacesBelow > 0) {
                if (spacesLeft > 1) {
                    targetSquare = currentSquare + 6;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                              targetSet);
                }
                if (spacesRight > 1) {
                    targetSquare = currentSquare + 10;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                              targetSet);
                }
            }
            break;
        }
        case 0b100: {
            // bishop
            for (int i = 1; i <= minTopLeft; ++i) {
                targetSquare = currentSquare - i * 9;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                               targetSet))
                    break;
            }
            for (int i = 1; i <= minTopRight; ++i) {
                targetSquare = currentSquare - i * 7;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                               targetSet))
                    break;
            }
            for (int i = 1; i <= minBottomLeft; ++i) {
                targetSquare = currentSquare + i * 7;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                               targetSet))
                    break;
            }
            for (int i = 1; i <= minBottomRight; ++i) {
                targetSquare = currentSquare + i * 9;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                               targetSet))
                    break;
            }
            break;
        }
        case 0b1000: {
            // rook
            for (int i = 1; i <= spacesAbove; ++i) {
                targetSquare = currentSquare - i * 8;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                               targetSet))
                    break;
            }
            for (int i = 1; i <= spacesBelow; ++i) {
                targetSquare = currentSquare + i * 8;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                               targetSet))
                    break;
            }
            for (int i = 1; i <= spacesLeft; ++i) {
                targetSquare = currentSquare - i;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                               targetSet))
                    break;
            }
            for (int i = 1; i <= spacesRight; ++i) {
                targetSquare = currentSquare + i;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                               targetSet))
                    break;
            }
            break;
        }
        case 0b10000: {
            // queen
            for (int i = 1; i <= minTopLeft; ++i) {
                targetSquare = currentSquare - i * 9;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                               targetSet))
                    break;
            }
            for (int i = 1; i <= minTopRight; ++i) {
                targetSquare = currentSquare - i * 7;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                               targetSet))
                    break;
            }
            for (int i = 1; i <= minBottomLeft; ++i) {
                targetSquare = currentSquare + i * 7;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                               targetSet))
                    break;
            }
            for (int i = 1; i <= minBottomRight; ++i) {
                targetSquare = currentSquare + i * 9;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                               targetSet))
                    break;
            }
            for (int i = 1; i <= spacesAbove; ++i) {
                targetSquare = currentSquare - i * 8;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                               targetSet))
                    break;
            }
            for (int i = 1; i <= spacesBelow; ++i) {
                targetSquare = currentSquare + i * 8;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                               targetSet))
                    break;
            }
            for (int i = 1; i <= spacesLeft; ++i) {
                targetSquare = currentSquare - i;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                               targetSet))
                    break;
            }
            for (int i = 1; i <= spacesRight; ++i) {
                targetSquare = currentSquare + i;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                               targetSet))
                    break;
            }
            break;
        }
        case 0b100000: {
            // king
            if (minBottomLeft) {
                targetSquare = currentSquare + 7;
                addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                          targetSet);

            }
            if (minBottomRight) {
                targetSquare = currentSquare + 9;
                addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                          targetSet);
            }
            if (minTopLeft) {
                targetSquare = currentSquare - 9;
                addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                          targetSet);
            }
            if (minTopRight) {
                targetSquare = currentSquare - 7;
                addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                          targetSet);
            }
            if (spacesAbove) {
                targetSquare = currentSquare - 8;
                addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                          targetSet);
            }
            if (spacesBelow) {
                targetSquare = currentSquare + 8;
                addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                          targetSet);
            }
            if (spacesLeft) {
                targetSquare = currentSquare - 1;
                addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                          targetSet);
            }
            if (spacesRight) {
                targetSquare = currentSquare + 1;
                addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                          targetSet);
            }

            bool leftCastleDisabled = selectedPieceColor == ChessPiece::WHITE ? !wCastleQueenSide : !bCastleQueenSide;
            bool rightCastleDisabled = selectedPieceColor == ChessPiece::WHITE ? !wCastleKingSide : !bCastleKingSide;

            if (spacesLeft > 1 && !leftCastleDisabled) {
                // castle left
                targetSquare = currentSquare - 2;
                addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                          targetSet);
            }
            if (spacesRight > 1 && !rightCastleDisabled) {
                // castle right
                targetSquare = currentSquare + 2;
                addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor, checkingCheck,
                          targetSet);
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

bool ChessBoard::addTarget(unsigned short originSquare, unsigned short targetSquare,
                           ChessPiece::PieceColor selectedPieceColor,
                           ChessPiece::PieceColor oppositePieceColor, bool checkingCheck,
                           std::unordered_set<unsigned short> &set) {

    if (targetSquare == 62 && selectedSquareIndex == 60) {
        // white wants to castle king side
        if (squares[61] != 0 || squares[62] != 0 || isChecked(60) || isChecked(61) || isChecked(62))
            return false;
    }
    if (targetSquare == 58 && selectedSquareIndex == 60) {
        // white wants to castle queen side
        if (squares[57] != 0 || squares[58] != 0 || squares[59] != 0 || isChecked(60) || isChecked(58) || isChecked(59))
            return false;
    }
    if (targetSquare == 6 && selectedSquareIndex == 4) {
        // black wants to castle king side
        if (squares[5] != 0 || squares[6] != 0 || isChecked(4) || isChecked(5) || isChecked(6))
            return false;
    }
    if (targetSquare == 2 && selectedSquareIndex == 4) {
        // black wants to castle queen side
        if (squares[1] != 0 || squares[2] != 0 || squares[3] != 0 || isChecked(4) || isChecked(2) || isChecked(3))
            return false;
    }

    ChessPiece::PieceColor targetPieceColor = ChessPiece::getPieceColor(squares[targetSquare]);
    if (targetPieceColor == selectedPieceColor)
        return false;

    if (!checkingCheck) {
        bool wCastleKingSideOld = wCastleKingSide;
        bool wCastleQueenSideOld = wCastleQueenSide;
        bool bCastleKingSideOld = bCastleKingSide;
        bool bCastleQueenSideOld = bCastleQueenSide;

        unsigned short originalPieceCode = makeMove(originSquare, targetSquare);
        if (!isChecked((selectedPieceCode & 0b00100000) != 0 ? targetSquare : (selectedPieceColor == ChessPiece::WHITE ? wKingSquare : bKingSquare)))
            set.insert(targetSquare);
        undoMove(originSquare, targetSquare, originalPieceCode, wCastleKingSideOld, wCastleQueenSideOld,
                 bCastleKingSideOld, bCastleQueenSideOld);
    } else {
        set.insert(targetSquare);
    }

    if (targetPieceColor == oppositePieceColor) return false;
    return true;
}

const unsigned short *ChessBoard::getSquares() const {
    return squares;
}

unsigned short ChessBoard::makeMove(unsigned short origin, unsigned short targetSquare) {
    unsigned short targetPiece = squares[targetSquare];
    squares[targetSquare] = selectedPieceCode;
    squares[origin] = 0;

    return targetPiece;
}

void
ChessBoard::undoMove(unsigned short originSquare, unsigned short targetSquare, unsigned short originalPieceCode,
                     bool wCastleKingSide, bool wCastleQueenSide, bool bCastleKingSide, bool bCastleQueenSide) {
    squares[originSquare] = squares[targetSquare];
    squares[targetSquare] = originalPieceCode;
    for (unsigned short sq: attackedSquares) {
        attackedSquaresDraw.insert(sq);
    }
    attackedSquares.clear();
}

std::unordered_set<unsigned short> ChessBoard::getSquaresAttackedByOpponent() {
    attackedSquares.clear();

    for (int i = 0; i < 64; ++i) {
        if (squares[i] != 0 && getColorFromPieceCode(squares[i]) != getColorFromPieceCode(selectedPieceCode)) {
            possibleMoves(i, true, attackedSquares);
        }
    }
    return attackedSquares;
}

