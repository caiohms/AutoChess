#include "ChessBoard.h"
#include "ChessPiece.h"
#include "ChessBoardState.h"
#include <set>
#include <iostream>

ChessBoard::ChessBoard(int width, int height, const sf::Font &font, bool &turn, sf::RenderWindow &window)
        : turn(turn), window(window) {
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

void ChessBoard::draw() {
    sf::Text text;
    text.setFont(font);

    window.draw(boardOutline);

    float boardEdge = (float) boardSize.x - 20;
    float squareEdge = boardEdge / 8.0f;

    lightRect.setSize(sf::Vector2f(squareEdge, squareEdge));
    darkRect.setSize(sf::Vector2f(squareEdge, squareEdge));
    selectedRect.setSize(sf::Vector2f(squareEdge, squareEdge));

    lightTargetedRect.setSize(sf::Vector2f(squareEdge, squareEdge));
    darkTargetedRect.setSize(sf::Vector2f(squareEdge, squareEdge));

    for (int i = 0; i < 64; ++i) {
        text.setString(std::to_string(i));

        float xPos = 10 + boardEdge / 8 * (float) (i % 8);
        float yPos = 10 + boardEdge / 8 * (float) (i / 8);
        lightRect.setPosition(sf::Vector2f(xPos, yPos));
        darkRect.setPosition(sf::Vector2f(xPos, yPos));
        selectedRect.setPosition(sf::Vector2f(xPos, yPos));
        lightTargetedRect.setPosition(sf::Vector2f(xPos, yPos));
        darkTargetedRect.setPosition(sf::Vector2f(xPos, yPos));

        text.setPosition(xPos, yPos);

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

        //draw under selected square
        if (mouseSelectedSquare == i)
            window.draw(selectedRect);

//        if (attackedSquaresDraw.contains(i) && selectedPieceCode != 0) {
//            if ((i % 2 + (int) (i / 8)) % 2 == 0)
//                window.draw(lightTargetedRect);
//            else
//                window.draw(darkTargetedRect);
//        }

        ChessBoard::drawPiece(squares[i], xPos, yPos, boardEdge);

        if (possibilities.contains(i)) {
            window.draw(targetCircleShape);
        }

        window.draw(text);
    }

    if (mouseSelectedSquare >= 0 && mouseSelectedSquare < 64) {
        unsigned short mouseFollowingPieceCode = squares[mouseSelectedSquare];

        if (mouseFollowingPieceCode > 0) {
            ChessBoard::drawPiece(mouseFollowingPieceCode, mouseXpos - squareEdge / 2, mouseYpos - squareEdge / 2,
                                  boardEdge);
        }
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

long ChessBoard::moveMaker(int depth, bool playerTurn) {
    if (debugging) {
        redrawWindow();
    }

    playerTurn = !playerTurn;

    if (depth == 0) return 1;

    long a = 0;

    for (int i = 0; i < 64; ++i) {

        if (squares[i] == 0) continue;
        std::unordered_set<unsigned short> set = grabPiece(i, playerTurn);
        if (!set.empty())
            for (unsigned short target: set) {

                auto previousState = ChessBoardState::fromChessBoard(this);
                makeMove(i, target);

                a += moveMaker(depth - 1, playerTurn);

                undoMove(previousState);
            }
    }

    return a;
}

void ChessBoard::mouseGrabPiece(unsigned int mouseX, unsigned int mouseY) {
    if (mouseX + 10 > boardSize.x || mouseY + 10 > boardSize.y || mouseX < 10 || mouseY < 10) {
        mouseSelectedSquare = -1;
        return;
    }

    int selectedSquare = getSquareUnderMousePos(mouseX, mouseY);
    mouseSelectedSquare = selectedSquare;
    unsigned short selectedPieceCode = squares[selectedSquare];

    grabPiece(selectedSquare);
}

std::unordered_set<unsigned short> ChessBoard::grabPiece(unsigned int squareIdx, bool playerTurn) {
    mouseSelectedSquare = squareIdx;

    if ((getColorCodeFromPieceCode(squares[squareIdx]) == ChessPiece::PieceColor::WHITE && playerTurn)
        || (getColorCodeFromPieceCode(squares[squareIdx]) == ChessPiece::PieceColor::BLACK && !playerTurn)) {
        possibilities.clear();
//        attackedSquaresDraw.clear();
        return possibleMoves(squareIdx);
//    } else {
//        if (playerTurn)
//            std::cout << "It is white's turn" << std::endl;
//        else
//            std::cout << "It is black's turn" << std::endl;
    }

    return {};
}

std::unordered_set<unsigned short> ChessBoard::grabPiece(unsigned short squareIdx) {

    if ((getColorCodeFromPieceCode(squares[squareIdx]) == ChessPiece::PieceColor::WHITE && turn)
        || (getColorCodeFromPieceCode(squares[squareIdx]) == ChessPiece::PieceColor::BLACK && !turn)) {
        possibilities.clear();
//        attackedSquaresDraw.clear();
        return possibleMoves(squareIdx);
//    } else {
//        if (playerTurn)
//            std::cout << "It is white's turn" << std::endl;
//        else
//            std::cout << "It is black's turn" << std::endl;
    }

    return {};
}

void ChessBoard::mouseReleasePiece(unsigned int mouseX, unsigned int mouseY) {
    if (mouseSelectedSquare == -1) return;

    int targetSquareIndex = getSquareUnderMousePos(mouseX, mouseY);

    if (targetSquareIndex == mouseSelectedSquare) {
        mouseSelectedSquare = -1;
        return;
    }

    if (possibilities.contains(targetSquareIndex)) {
        makeMove(mouseSelectedSquare, targetSquareIndex);
    }

    mouseSelectedSquare = -1;
    possibilities.clear();
}

bool ChessBoard::isChecked(unsigned short square) {
    int pieceCode = squares[square];

    ChessPiece::PieceColor squareColor;

    ChessPiece::PieceColor teamColor = (turn ? ChessPiece::WHITE : ChessPiece::BLACK);
    ChessPiece::PieceColor oppositePieceColor = (!turn ? ChessPiece::WHITE : ChessPiece::BLACK);

    int spacesAbove = square / 8;
    int spacesBelow = 7 - square / 8;
    int spacesLeft = (square % 8);
    int spacesRight = 7 - (square % 8);

    int minTopLeft = std::min(spacesAbove, spacesLeft);
    int minTopRight = std::min(spacesAbove, spacesRight);
    int minBottomLeft = std::min(spacesBelow, spacesLeft);
    int minBottomRight = std::min(spacesBelow, spacesRight);

    int squareValue;
    unsigned short code;

    // check all directions
    for (int i = 1; i <= minTopLeft; ++i) {
        squareValue = squares[square - i * 9];
        if (squareValue == 0)
            continue;
        squareColor = getPieceColorFromPieceCode(squareValue);
        if (squareColor == teamColor) break;
        if (squareColor == oppositePieceColor) {
            code = getPieceCode(squareValue);
            if (code == BISHOP_CODE || code == QUEEN_CODE)
                return true;
        }
    }
    for (int i = 1; i <= minTopRight; ++i) {
        squareValue = squares[square - i * 7];
        if (squareValue == 0)
            continue;
        squareColor = getPieceColorFromPieceCode(squareValue);
        if (squareColor == teamColor) break;
        if (squareColor == oppositePieceColor) {
            code = getPieceCode(squareValue);
            if (code == BISHOP_CODE || code == QUEEN_CODE)
                return true;
        }
    }
    for (int i = 1; i <= minBottomLeft; ++i) {
        squareValue = squares[square + i * 7];
        if (squareValue == 0)
            continue;
        squareColor = getPieceColorFromPieceCode(squareValue);
        if (squareColor == teamColor) break;
        if (squareColor == oppositePieceColor) {
            code = getPieceCode(squareValue);
            if (code == BISHOP_CODE || code == QUEEN_CODE)
                return true;
        }
    }
    for (int i = 1; i <= minBottomRight; ++i) {
        squareValue = squares[square + i * 9];
        if (squareValue == 0)
            continue;
        squareColor = getPieceColorFromPieceCode(squareValue);
        if (squareColor == teamColor) break;
        if (squareColor == oppositePieceColor) {
            code = getPieceCode(squareValue);
            if (code == BISHOP_CODE || code == QUEEN_CODE)
                return true;
        }
    }
    for (int i = 1; i <= spacesAbove; ++i) {
        squareValue = squares[square - i * 8];

        if (squareValue == 0)
            continue;
        squareColor = getPieceColorFromPieceCode(squareValue);
        if (squareColor == teamColor) break;
        if (squareColor == oppositePieceColor) {
            code = getPieceCode(squareValue);
            if (code == ROOK_CODE || code == QUEEN_CODE)
                return true;
        }
    }
    for (int i = 1; i <= spacesBelow; ++i) {
        squareValue = squares[square + i * 8];
        if (squareValue == 0)
            continue;
        squareColor = getPieceColorFromPieceCode(squareValue);
        if (squareColor == teamColor) break;
        if (squareColor == oppositePieceColor) {
            code = getPieceCode(squareValue);
            if (code == ROOK_CODE || code == QUEEN_CODE)
                return true;
        }
    }
    for (int i = 1; i <= spacesLeft; ++i) { ;
        squareValue = squares[square - i];
        if (squareValue == 0)
            continue;
        squareColor = getPieceColorFromPieceCode(squareValue);
        if (squareColor == teamColor) break;
        if (squareColor == oppositePieceColor) {
            code = getPieceCode(squareValue);
            if (code == ROOK_CODE || code == QUEEN_CODE)
                return true;
        }
    }
    for (int i = 1; i <= spacesRight; ++i) {
        squareValue = squares[square + i];
        if (squareValue == 0)
            continue;
        squareColor = getPieceColorFromPieceCode(squareValue);
        if (squareColor == teamColor) break;
        if (squareColor == oppositePieceColor) {
            code = getPieceCode(squareValue);
            if (code == ROOK_CODE || code == QUEEN_CODE)
                return true;
        }
    }

    //knight movement
    if (spacesAbove > 1) {
        if (spacesLeft > 1) {
            squareValue = squares[square - 10];
            if (getColorCodeFromPieceCode(squareValue) == oppositePieceColor) {
                code = getPieceCode(squareValue);
                if (code == KNIGHT_CODE)
                    return true;
            }
        }
        if (spacesRight > 1) {
            squareValue = squares[square - 6];
            if (getColorCodeFromPieceCode(squareValue) == oppositePieceColor) {
                code = getPieceCode(squareValue);
                if (code == KNIGHT_CODE)
                    return true;
            }
        }
        if (spacesLeft > 0) {
            squareValue = squares[square - 17];
            if (getColorCodeFromPieceCode(squareValue) == oppositePieceColor) {
                code = getPieceCode(squareValue);
                if (code == KNIGHT_CODE)
                    return true;
            }
        }
        if (spacesRight > 0) {
            squareValue = squares[square - 15];
            if (getColorCodeFromPieceCode(squareValue) == oppositePieceColor) {
                code = getPieceCode(squareValue);
                if (code == KNIGHT_CODE)
                    return true;
            }
        }
    } else if (spacesAbove > 0) {
        if (spacesLeft > 1) {
            squareValue = squares[square - 10];
            if (getColorCodeFromPieceCode(squareValue) != teamColor) {
                code = getPieceCode(squareValue);
                if (code == KNIGHT_CODE)
                    return true;
            }
        }
        if (spacesRight > 1) {
            squareValue = squares[square - 6];
            if (getColorCodeFromPieceCode(squareValue) != teamColor) {
                code = getPieceCode(squareValue);
                if (code == KNIGHT_CODE)
                    return true;
            }
        }
    }
    if (spacesBelow > 1) {
        if (spacesLeft > 1) {
            squareValue = squares[square + 6];
            if (getColorCodeFromPieceCode(squareValue) == oppositePieceColor) {
                code = getPieceCode(squareValue);
                if (code == KNIGHT_CODE)
                    return true;
            }
        }
        if (spacesRight > 1) {
            squareValue = squares[square + 10];
            if (getColorCodeFromPieceCode(squareValue) == oppositePieceColor) {
                code = getPieceCode(squareValue);
                if (code == KNIGHT_CODE)
                    return true;
            }
        }
        if (spacesLeft > 0) {
            squareValue = squares[square + 5];
            if (getColorCodeFromPieceCode(squareValue) == oppositePieceColor) {
                code = getPieceCode(squareValue);
                if (code == KNIGHT_CODE)
                    return true;
            }
        }
        if (spacesRight > 0) {
            squareValue = squares[square + 17];
            if (getColorCodeFromPieceCode(squareValue) == oppositePieceColor) {
                code = getPieceCode(squareValue);
                if (code == KNIGHT_CODE)
                    return true;
            }
        }
    } else if (spacesBelow > 0) {
        if (spacesLeft > 1) {
            squareValue = squares[square + 6];
            if (getColorCodeFromPieceCode(squareValue) == oppositePieceColor) {
                code = getPieceCode(squareValue);
                if (code == KNIGHT_CODE)
                    return true;
            }
        }
        if (spacesRight > 1) {
            squareValue = squares[square + 10];
            if (getColorCodeFromPieceCode(squareValue) == oppositePieceColor) {
                code = getPieceCode(squareValue);
                if (code == KNIGHT_CODE)
                    return true;
            }
        }
    }

    if (minBottomLeft) {
        squareValue = squares[square + 7];
        if (getColorCodeFromPieceCode(squareValue) == oppositePieceColor) {
            code = getPieceCode(squareValue);
            if (code == KING_CODE || code == PAWN_CODE)
                return true;
        }
    }
    if (minBottomRight) {
        squareValue = squares[square + 9];
        if (getColorCodeFromPieceCode(squareValue) == oppositePieceColor) {
            code = getPieceCode(squareValue);
            if (code == KING_CODE || code == PAWN_CODE)
                return true;
        }
    }
    if (minTopLeft) {
        squareValue = squares[square - 9];
        if (getColorCodeFromPieceCode(squareValue) == oppositePieceColor) {
            code = getPieceCode(squareValue);
            if (code == KING_CODE || code == PAWN_CODE)
                return true;
        }
    }
    if (minTopRight) {
        squareValue = squares[square - 7];
        if (getColorCodeFromPieceCode(squareValue) == oppositePieceColor) {
            code = getPieceCode(squareValue);
            if (code == KING_CODE || code == PAWN_CODE)
                return true;
        }
    }
    if (spacesAbove) {
        squareValue = squares[square - 8];
        if (getColorCodeFromPieceCode(squareValue) == oppositePieceColor) {
            code = getPieceCode(squareValue);
            if (code == KING_CODE)
                return true;
        }
    }
    if (spacesBelow) {
        squareValue = squares[square + 8];
        if (getColorCodeFromPieceCode(squareValue) == oppositePieceColor) {
            code = getPieceCode(squareValue);
            if (code == KING_CODE)
                return true;
        }
    }

    if (spacesLeft) {
        squareValue = squares[square - 1];
        if (getColorCodeFromPieceCode(squareValue) == oppositePieceColor) {
            code = getPieceCode(squareValue);
            if (code == KING_CODE)
                return true;
        }
    }
    if (spacesRight) {
        squareValue = squares[square + 1];
        if (getColorCodeFromPieceCode(squareValue) == oppositePieceColor) {
            code = getPieceCode(squareValue);
            if (code == KING_CODE)
                return true;
        }
    }

    return false;
}

inline unsigned int ChessBoard::getColorCodeFromPieceCode(unsigned short selectedPieceCode) {
    unsigned int selectedColor = selectedPieceCode & 0b11000000;
    return selectedColor;
}

inline ChessPiece::PieceColor ChessBoard::getPieceColorFromPieceCode(unsigned short pieceCode) {
    if (pieceCode == 0b11000000) return ChessPiece::BLANK;
    else if (pieceCode & 0b01000000) return ChessPiece::WHITE;
    else if (pieceCode & 0b10000000) return ChessPiece::BLACK;
    return ChessPiece::BLANK;
}

int ChessBoard::getSquareUnderMousePos(unsigned int mouseX, unsigned int mouseY) {
    unsigned short squareX = (mouseX) / ((boardSize.x) / 8);
    unsigned short squareY = (mouseY) / ((boardSize.y) / 8);

    return squareY * 8 + squareX;
}

std::unordered_set<unsigned short>
ChessBoard::possibleMoves(int currentSquare) {

    int pieceCode = squares[currentSquare];

    if (pieceCode == 0) return {};

    int spacesAbove = currentSquare / 8;
    int spacesBelow = 7 - currentSquare / 8;
    int spacesLeft = (currentSquare % 8);
    int spacesRight = 7 - (currentSquare % 8);

    int minTopLeft = std::min(spacesAbove, spacesLeft);
    int minTopRight = std::min(spacesAbove, spacesRight);
    int minBottomLeft = std::min(spacesBelow, spacesLeft);
    int minBottomRight = std::min(spacesBelow, spacesRight);

    ChessPiece::PieceColor selectedPieceColor = getPieceColorFromPieceCode(pieceCode);
    ChessPiece::PieceColor oppositePieceColor = getPieceColorFromPieceCode(pieceCode ^ 0b11000000 << 6 >> 6);

    int targetSquare;

    switch (pieceCode & 0b00111111) {
        case 0b1: {
            // pawn
            if (selectedPieceColor == ChessPiece::PieceColor::BLACK) {
                if (minBottomLeft && (squares[currentSquare + 7] != 0 ||
                                      currentSquare + 7 == enPassantEnabledSquare)) {
                    targetSquare = currentSquare + 7;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
                }
                if (minBottomRight && (squares[currentSquare + 9] != 0 ||
                                       currentSquare + 9 == enPassantEnabledSquare)) {
                    targetSquare = currentSquare + 9;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
                }
                if (spacesBelow && squares[currentSquare + 8] == 0) {
                    targetSquare = currentSquare + 8;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
                }
                if (spacesBelow == 6 && squares[currentSquare + 8] == 0 && squares[currentSquare + 16] == 0) {
                    targetSquare = currentSquare + 16;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
                }

            } else {
                // white pawn
                if (minTopLeft && (squares[currentSquare - 9] != 0 || currentSquare - 9 == enPassantEnabledSquare)) {

                    targetSquare = currentSquare - 9;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
                }
                if (minTopRight && (squares[currentSquare - 7] != 0 ||
                                    currentSquare - 7 == enPassantEnabledSquare)) {

                    targetSquare = currentSquare - 7;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
                }
                if (spacesAbove && squares[currentSquare - 8] == 0) {
                    targetSquare = currentSquare - 8;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
                }
                if (spacesAbove == 6 && squares[currentSquare - 8] == 0 && squares[currentSquare - 16] == 0) {
                    targetSquare = currentSquare - 16;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
                }
            }
            break;
        }
        case 0b10: {
            // knight
            if (spacesAbove > 1) {
                if (spacesLeft > 1) {
                    targetSquare = currentSquare - 10;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
                }
                if (spacesRight > 1) {
                    targetSquare = currentSquare - 6;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
                }
                if (spacesLeft > 0) {
                    targetSquare = currentSquare - 17;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
                }
                if (spacesRight > 0) {
                    targetSquare = currentSquare - 15;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
                }
            } else if (spacesAbove > 0) {
                if (spacesLeft > 1) {
                    targetSquare = currentSquare - 10;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
                }
                if (spacesRight > 1) {
                    targetSquare = currentSquare - 6;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
                }
            }

            if (spacesBelow > 1) {
                if (spacesLeft > 1) {
                    targetSquare = currentSquare + 6;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
                }
                if (spacesRight > 1) {
                    targetSquare = currentSquare + 10;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
                }
                if (spacesLeft > 0) {
                    targetSquare = currentSquare + 15;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
                }
                if (spacesRight > 0) {
                    targetSquare = currentSquare + 17;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
                }
            } else if (spacesBelow > 0) {
                if (spacesLeft > 1) {
                    targetSquare = currentSquare + 6;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
                }
                if (spacesRight > 1) {
                    targetSquare = currentSquare + 10;
                    addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
                }
            }
            break;
        }
        case 0b100: {
            // bishop
            for (int i = 1; i <= minTopLeft; ++i) {
                targetSquare = currentSquare - i * 9;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor))
                    break;
            }
            for (int i = 1; i <= minTopRight; ++i) {
                targetSquare = currentSquare - i * 7;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor))
                    break;
            }
            for (int i = 1; i <= minBottomLeft; ++i) {
                targetSquare = currentSquare + i * 7;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor))
                    break;
            }
            for (int i = 1; i <= minBottomRight; ++i) {
                targetSquare = currentSquare + i * 9;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor))
                    break;
            }
            break;
        }
        case 0b1000: {
            // rook
            for (int i = 1; i <= spacesAbove; ++i) {
                targetSquare = currentSquare - i * 8;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor))
                    break;
            }
            for (int i = 1; i <= spacesBelow; ++i) {
                targetSquare = currentSquare + i * 8;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor))
                    break;
            }
            for (int i = 1; i <= spacesLeft; ++i) {
                targetSquare = currentSquare - i;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor))
                    break;
            }
            for (int i = 1; i <= spacesRight; ++i) {
                targetSquare = currentSquare + i;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor))
                    break;
            }
            break;
        }
        case 0b10000: {
            // queen
            for (int i = 1; i <= minTopLeft; ++i) {
                targetSquare = currentSquare - i * 9;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor))
                    break;
            }
            for (int i = 1; i <= minTopRight; ++i) {
                targetSquare = currentSquare - i * 7;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor))
                    break;
            }
            for (int i = 1; i <= minBottomLeft; ++i) {
                targetSquare = currentSquare + i * 7;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor))
                    break;
            }
            for (int i = 1; i <= minBottomRight; ++i) {
                targetSquare = currentSquare + i * 9;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor))
                    break;
            }
            for (int i = 1; i <= spacesAbove; ++i) {
                targetSquare = currentSquare - i * 8;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor))
                    break;
            }
            for (int i = 1; i <= spacesBelow; ++i) {
                targetSquare = currentSquare + i * 8;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor))
                    break;
            }
            for (int i = 1; i <= spacesLeft; ++i) {
                targetSquare = currentSquare - i;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor))
                    break;
            }
            for (int i = 1; i <= spacesRight; ++i) {
                targetSquare = currentSquare + i;
                if (!addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor))
                    break;
            }
            break;
        }
        case 0b100000: {
            // king
            if (minBottomLeft) {
                targetSquare = currentSquare + 7;
                addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);

            }
            if (minBottomRight) {
                targetSquare = currentSquare + 9;
                addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
            }
            if (minTopLeft) {
                targetSquare = currentSquare - 9;
                addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
            }
            if (minTopRight) {
                targetSquare = currentSquare - 7;
                addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
            }
            if (spacesAbove) {
                targetSquare = currentSquare - 8;
                addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
            }
            if (spacesBelow) {
                targetSquare = currentSquare + 8;
                addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
            }
            if (spacesLeft) {
                targetSquare = currentSquare - 1;
                addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
            }
            if (spacesRight) {
                targetSquare = currentSquare + 1;
                addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
            }

            bool leftCastleDisabled = selectedPieceColor == ChessPiece::WHITE ? !wCastleQueenSide : !bCastleQueenSide;
            bool rightCastleDisabled = selectedPieceColor == ChessPiece::WHITE ? !wCastleKingSide : !bCastleKingSide;

            if (spacesLeft > 1 && !leftCastleDisabled) {
                // castle left
                targetSquare = currentSquare - 2;
                addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
            }
            if (spacesRight > 1 && !rightCastleDisabled) {
                // castle right
                targetSquare = currentSquare + 2;
                addTarget(currentSquare, targetSquare, selectedPieceColor, oppositePieceColor);
            }
            break;
        }
    }

    return possibilities;
}

void
ChessBoard::drawPiece(unsigned short pieceCode, float xPos, float yPos, float boardEdge) {
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
                           ChessPiece::PieceColor selectedPieceColor, ChessPiece::PieceColor oppositePieceColor) {
    unsigned short selectedPiece = squares[originSquare];
    ChessPiece::PieceColor targetPieceColor = getPieceColorFromPieceCode(squares[targetSquare]);

    if (targetPieceColor == selectedPieceColor)
        return false;

    if (targetSquare == 62 && originSquare == 60) {
        // white wants to castle king side
        if (squares[61] != 0 || squares[62] != 0 || isChecked(60) || isChecked(61) || isChecked(62))
            return false;
    }
    if (targetSquare == 58 && originSquare == 60) {
        // white wants to castle queen side
        if (squares[57] != 0 || squares[58] != 0 || squares[59] != 0 || isChecked(60) || isChecked(58) ||
            isChecked(59))
            return false;
    }
    if (targetSquare == 6 && originSquare == 4) {
        // black wants to castle king side
        if (squares[5] != 0 || squares[6] != 0 || isChecked(4) || isChecked(5) || isChecked(6))
            return false;
    }
    if (targetSquare == 2 && originSquare == 4) {
        // black wants to castle queen side
        if (squares[1] != 0 || squares[2] != 0 || squares[3] != 0 || isChecked(4) || isChecked(2) || isChecked(3))
            return false;
    }

    ChessBoardState previousState = ChessBoardState::fromChessBoard(this);

    takePiece(originSquare, targetSquare);

    // if king is selected, kingSquare is the targetSquare. else it is the saved kingsquare.
    if (!isChecked((selectedPiece & 0b00100000) != 0 ? targetSquare : (selectedPieceColor == ChessPiece::WHITE
                                                                       ? wKingSquare : bKingSquare)))
        possibilities.insert(targetSquare);

    undoMove(previousState);

    if (targetPieceColor == oppositePieceColor) return false;
    return true;
}

unsigned short ChessBoard::takePiece(unsigned short origin, unsigned short targetSquare) {
    unsigned short targetPiece = squares[targetSquare];
    unsigned short originPiece = squares[origin];
    //squares[targetSquare] = selectedPieceCode;
    squares[targetSquare] = originPiece;

    squares[origin] = 0;


    if (debugging) {
        redrawWindow();
    }

    return targetPiece;
}

unsigned short ChessBoard::makeMove(unsigned short originSquare, unsigned short targetSquare) {

    unsigned short targetPiece = squares[targetSquare];
    unsigned short originPiece = squares[originSquare];
    squares[targetSquare] = targetPiece;
    squares[originSquare] = 0;

    ChessPiece::PieceColor selectedPieceColor = getPieceColorFromPieceCode(originPiece);

    if ((targetSquare / 8 == 7 || targetSquare / 8 == 0) &&
        (originPiece & 0b00000001) == 0b00000001) {
        // promote
        // promotionPrompt(targetSquareIndex);
        squares[targetSquare] = 0b00010000 | selectedPieceColor;
    } else {
        squares[targetSquare] = originPiece;

        if (originPiece == B_KING) {
            bKingSquare = targetSquare;
        } else if (originPiece == W_KING) {
            wKingSquare = targetSquare;
        }

        // send tower back if castling
        if (wCastleKingSide && originSquare == 60 && targetSquare == 62) {
            //do castle white king, king side
            squares[63] = 0;
            squares[61] = W_ROOK;
        }
        if (wCastleQueenSide && originSquare == 60 && targetSquare == 58) {
            //do castle white king, queen side
            squares[56] = 0;
            squares[59] = W_ROOK;
        }
        if (bCastleKingSide && originSquare == 4 && targetSquare == 6) {
            //do castle black king, king side
            squares[7] = 0;
            squares[5] = B_ROOK;
        }
        if (bCastleQueenSide && originSquare == 4 && targetSquare == 2) {
            //do castle black king, queen side
            squares[0] = 0;
            squares[3] = B_ROOK;
        }

        if (originSquare == 63 || originSquare == 60 || targetSquare == 63)
            wCastleKingSide = false;
        if (originSquare == 56 || originSquare == 60 || targetSquare == 56)
            wCastleQueenSide = false;
        if (originSquare == 0 || originSquare == 4 || targetSquare == 0)
            bCastleQueenSide = false;
        if (originSquare == 7 || originSquare == 4 || targetSquare == 7)
            bCastleKingSide = false;
    }

    if (abs(originSquare - targetSquare) == 16 && (originPiece & 0b00000001) == PAWN_CODE) {
        // if pawn moved two squares, mark middle square as potential target
        unsigned short row = originSquare / 8;
        enPassantEnabledSquare = (row == 1) ? originSquare + 8 : originSquare - 8;
        if (!(enPassantEnabledSquare / 8 == 2 &&
              getColorCodeFromPieceCode(originPiece) ==
              getColorCodeFromPieceCode(squares[enPassantEnabledSquare + 8])) &&
            !(enPassantEnabledSquare / 8 == 5 &&
              getColorCodeFromPieceCode(originPiece) ==
              getColorCodeFromPieceCode(squares[enPassantEnabledSquare - 8]))) {
            enPassantEnabledSquare = 0b11111111;
        }

    } else if (targetSquare == enPassantEnabledSquare && (originPiece & 0b00000001) == PAWN_CODE) {
        // if a pawn attacks an enPassantEnabledSquare, an en passant occurred. Opponent pawn must be removed.
        unsigned short row = targetSquare / 8;
        squares[targetSquare + ((row == 2) ? 8 : -8)] = 0;
        enPassantEnabledSquare = 0b11111111;
    } else {
        enPassantEnabledSquare = 0b11111111;
    }

    turn = !turn;
    return targetPiece;
}

void ChessBoard::undoMove(ChessBoardState previousBoardState) {
    wKingSquare = previousBoardState.wKingSquare;
    bKingSquare = previousBoardState.bKingSquare;
    wCastleKingSide = previousBoardState.wCastleKingSide;
    wCastleQueenSide = previousBoardState.wCastleQueenSide;
    bCastleKingSide = previousBoardState.bCastleKingSide;
    bCastleQueenSide = previousBoardState.bCastleQueenSide;
    enPassantEnabledSquare = previousBoardState.enPassantEnabledSquare;
    turn = previousBoardState.playerTurn;

    std::copy_n(previousBoardState.squares, 64, squares);

    attackedSquares.clear();

    if (debugging) {
        redrawWindow();
    }
}

const unsigned short *ChessBoard::getSquares() const {
    return squares;
}

unsigned short ChessBoard::getEnPassantEnabledSquare() const {
    return enPassantEnabledSquare;
}

bool ChessBoard::isBCastleKingSide() const {
    return bCastleKingSide;
}

bool ChessBoard::isBCastleQueenSide() const {
    return bCastleQueenSide;
}

bool ChessBoard::isWCastleKingSide() const {
    return wCastleKingSide;
}

bool ChessBoard::isWCastleQueenSide() const {
    return wCastleQueenSide;
}

unsigned short ChessBoard::getWKingSquare() const {
    return wKingSquare;
}

unsigned short ChessBoard::getBKingSquare() const {
    return bKingSquare;
}

bool ChessBoard::getTurn() const {
    return turn;
}

void ChessBoard::redrawWindow() {
    window.clear(sf::Color(35, 57, 76));
    draw();
    window.display();
}

inline unsigned short ChessBoard::getPieceCode(unsigned short pieceValue) {
    return pieceValue & 0b00111111;
}
