
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
    possibiltyPiece.setFillColor(sf::Color(2, 188, 0));

    initChessPieces();
}

void ChessBoard::draw(sf::RenderWindow &window) {

    window.draw(boardOutline);

    float boardEdge = (float) boardSize.x - 20;

    float squareEdge = boardEdge / 8.0f;

    lightSquare.setSize(sf::Vector2f(squareEdge, squareEdge));
    darkSquare.setSize(sf::Vector2f(squareEdge, squareEdge));
    selectedSquare.setSize(sf::Vector2f(squareEdge, squareEdge));
    possibiltyPiece.setSize(sf::Vector2f(squareEdge, squareEdge));

    for (int i = 0; i < 64; ++i) {
        float xPos = 10 + boardEdge / 8 * (float) (i % 8);
        float yPos = 10 + boardEdge / 8 * (float) (i / 8);
        lightSquare.setPosition(sf::Vector2f(xPos, yPos));
        darkSquare.setPosition(sf::Vector2f(xPos, yPos));
        selectedSquare.setPosition(sf::Vector2f(xPos, yPos));
        possibiltyPiece.setPosition(sf::Vector2f(xPos, yPos));

        if ((i % 2 + (int) (i / 8)) % 2 == 0)
            window.draw(lightSquare);
        else
            window.draw(darkSquare);

        if (selectedSquareIndex == i)
            window.draw(selectedSquare);

        if (std::find(possibilty.begin(), possibilty.end(), i) != possibilty.end()) {
            window.draw(possibiltyPiece);
        }

        ChessBoard::drawPiece(squares[i], xPos, yPos, boardEdge, window);
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

void ChessBoard::initChessPieces() {
    std::set<int> bPawnMoves = {+8, +16, +7, +9};
    std::set<int> wPawnMoves = {-8, -16, -7, -9};
    std::set<int> rookMoves = {+1, +2, +3, +4, +5, +6, +7, +8, +16, +24, +32, +40, +48, +56};
    std::set<int> bishopMoves = {+9, +18, +27, +36, +45, +54, +63};
    std::set<int> knightMoves = {+10, +17, +15, +6};
    std::set<int> kingMoves = {+1, +2, -2, +7, +8, +9};
    std::set<int> queenMoves = {+1, +2, +3, +4, +5, +6, +7, +8, +16, +24, +32, +40, +48, +56, +9, +18, +27, +36, +45,
                                +54, +63};

    generateMoves(queenMoves);
    generateMoves(rookMoves);
    generateMoves(bishopMoves);
    generateMoves(kingMoves);
    generateMoves(knightMoves);

    bPawn.Init("resources\\sprites\\black-pawn.png", bPawnMoves);
    bBishop.Init("resources\\sprites\\black-bishop.png", bishopMoves);
    bKnight.Init("resources\\sprites\\black-knight.png", knightMoves);
    bRook.Init("resources\\sprites\\black-rook.png", rookMoves);
    bQueen.Init("resources\\sprites\\black-queen.png", queenMoves);
    bKing.Init("resources\\sprites\\black-king.png", kingMoves);

    wPawn.Init("resources\\sprites\\white-pawn.png", wPawnMoves);
    wBishop.Init("resources\\sprites\\white-bishop.png", bishopMoves);
    wKnight.Init("resources\\sprites\\white-knight.png", knightMoves);
    wRook.Init("resources\\sprites\\white-rook.png", rookMoves);
    wQueen.Init("resources\\sprites\\white-queen.png", queenMoves);
    wKing.Init("resources\\sprites\\white-king.png", kingMoves);


}

void ChessBoard::grabPiece(unsigned int mouseX, unsigned int mouseY) {
    mouseDragging = true;

    if (mouseX + 10 > boardSize.x || mouseY + 10 > boardSize.y || mouseX < 10 || mouseY < 10) {
        selectedSquareIndex = -1;
        return;
    }

    unsigned short squareX = (mouseX) / ((boardSize.x) / 8);
    unsigned short squareY = (mouseY) / ((boardSize.y) / 8);

    selectedSquareIndex = squareY * 8 + squareX;

    selectedPieceCode = squares[selectedSquareIndex];


    possibleMoves(selectedSquareIndex, selectedPieceCode);

}

void ChessBoard::possibleMoves(int indexSquare, int codePiece) {
    //std::cout << indexSquare << std::endl;
    //std::cout << codePiece << std::endl;

    possibilty.clear();
    if (codePiece == 0) {
        return;
    }
    switch (codePiece) {
        case 1: {
            //Diagonal esquerda
            if (indexSquare != 8 && indexSquare != 16 && indexSquare != 24 && indexSquare != 32 && indexSquare != 40 &&
                indexSquare != 48 && squares[(indexSquare + 7)] >= 7) {
                possibilty.insert(indexSquare + 7);
            }

            //Diagonal direita
            if (indexSquare != 15 && indexSquare != 23 && indexSquare != 31 && indexSquare != 39 && indexSquare != 47 &&
                indexSquare != 55 && squares[(indexSquare + 9)] >= 7) {
                possibilty.insert(indexSquare + 9);
            }

            // 1 para frente
            if (squares[indexSquare + 8] == 0) {
                possibilty.insert(indexSquare + 8);
            }

            // 2 para frente
            if (indexSquare >= 8 && indexSquare <= 15 && squares[indexSquare + 8] == 0 &&
                squares[indexSquare + 16] == 0) {
                possibilty.insert(indexSquare + 16);
            }

            break;
        }
        case 2: {
            if (indexSquare != 8 && indexSquare != 16 && indexSquare != 24 &&
                indexSquare != 32 && indexSquare != 40 && indexSquare != 48 &&
                indexSquare != 56 && indexSquare != 1 && indexSquare != 9 &&
                indexSquare != 17 && indexSquare != 25 && indexSquare != 33 &&
                indexSquare != 41 && indexSquare != 49 && indexSquare != 57 &&
                indexSquare != 2 && indexSquare != 3 && indexSquare != 4 &&
                indexSquare != 5 && indexSquare != 6 && indexSquare != 7 &&
                indexSquare != 0 &&
                (squares[indexSquare - 10] ==  0 || squares[indexSquare - 10] >= 7)) {

                possibilty.insert(indexSquare - 10);

            }

            if (indexSquare != 8 && indexSquare != 16 && indexSquare != 24 &&
                indexSquare != 32 && indexSquare != 40 && indexSquare != 48 &&
                indexSquare != 56 && indexSquare != 1 && indexSquare != 9 &&
                indexSquare != 17 && indexSquare != 25 && indexSquare != 33 &&
                indexSquare != 41 && indexSquare != 49 && indexSquare != 57 &&
                indexSquare != 58 && indexSquare != 59 && indexSquare != 60 &&
                indexSquare != 61 && indexSquare != 62 && indexSquare != 63 &&
                indexSquare != 0 &&
                (squares[indexSquare + 6] ==  0 || squares[indexSquare + 6] >= 7)) {

                possibilty.insert(indexSquare + 6);
            }

            if (indexSquare != 8 && indexSquare != 16 && indexSquare != 24 &&
                indexSquare != 32 && indexSquare != 40 && indexSquare != 48 &&
                indexSquare != 56 && indexSquare != 1 && indexSquare != 2 &&
                indexSquare != 3 && indexSquare != 4 && indexSquare != 5 &&
                indexSquare != 6 && indexSquare != 7 && indexSquare != 9 &&
                indexSquare != 10 && indexSquare != 11 && indexSquare != 12 &&
                indexSquare != 13 && indexSquare != 14 && indexSquare != 15 &&
                indexSquare != 0 &&
                (squares[indexSquare - 17] ==  0 || squares[indexSquare - 17] >= 7)) {

                possibilty.insert(indexSquare - 17);
            }

            if (indexSquare != 7 && indexSquare != 15 && indexSquare != 23 &&
                indexSquare != 31 && indexSquare != 39 && indexSquare != 47 &&
                indexSquare != 55 && indexSquare != 63 && indexSquare != 2 &&
                indexSquare != 3 && indexSquare != 4 && indexSquare != 5 &&
                indexSquare != 6 && indexSquare != 0 && indexSquare != 9 &&
                indexSquare != 10 && indexSquare != 11 && indexSquare != 12 &&
                indexSquare != 13 && indexSquare != 14 && indexSquare != 8 &&
                indexSquare != 1 &&
                (squares[indexSquare - 15] ==  0 || squares[indexSquare - 15] >= 7)) {

                possibilty.insert(indexSquare - 15);
            }

            if (indexSquare != 7 && indexSquare != 15 && indexSquare != 23 &&
                indexSquare != 31 && indexSquare != 39 && indexSquare != 47 &&
                indexSquare != 55 && indexSquare != 63 && indexSquare != 6 &&
                indexSquare != 14 && indexSquare != 22 && indexSquare != 30 &&
                indexSquare != 38 && indexSquare != 46 && indexSquare != 54 &&
                indexSquare != 62 && indexSquare != 0 && indexSquare != 1 &&
                indexSquare != 2 && indexSquare != 3 && indexSquare != 4 &&
                indexSquare != 5 &&
                (squares[indexSquare - 6] ==  0 || squares[indexSquare - 6] >= 7)) {

                possibilty.insert(indexSquare - 6);
            }

            if (indexSquare != 7 && indexSquare != 15 && indexSquare != 23 &&
                indexSquare != 31 && indexSquare != 39 && indexSquare != 47 &&
                indexSquare != 55 && indexSquare != 63 && indexSquare != 6 &&
                indexSquare != 14 && indexSquare != 22 && indexSquare != 30 &&
                indexSquare != 38 && indexSquare != 46 && indexSquare != 54 &&
                indexSquare != 62 && indexSquare != 56 && indexSquare != 57 &&
                indexSquare != 58 && indexSquare != 59 && indexSquare != 60 &&
                indexSquare != 61 &&
                (squares[indexSquare + 10] ==  0 || squares[indexSquare + 10] >= 7)) {

                possibilty.insert(indexSquare + 10);
            }

            if (indexSquare != 7 && indexSquare != 15 && indexSquare != 23 &&
                indexSquare != 31 && indexSquare != 39 && indexSquare != 47 &&
                indexSquare != 55 && indexSquare != 63 && indexSquare != 48 &&
                indexSquare != 49 && indexSquare != 50 && indexSquare != 51 &&
                indexSquare != 52 && indexSquare != 53 && indexSquare != 54 &&
                indexSquare != 56 && indexSquare != 57 && indexSquare != 58 &&
                indexSquare != 59 && indexSquare != 60 && indexSquare != 61 &&
                indexSquare != 62 &&
                (squares[indexSquare + 17] ==  0 || squares[indexSquare + 17] >= 7)) {

                possibilty.insert(indexSquare + 17);
            }

            if (indexSquare != 0 && indexSquare != 8 && indexSquare != 16 &&
                indexSquare != 24 && indexSquare != 32 && indexSquare != 40 &&
                indexSquare != 48 && indexSquare != 56 && indexSquare != 55 &&
                indexSquare != 49 && indexSquare != 50 && indexSquare != 51 &&
                indexSquare != 52 && indexSquare != 53 && indexSquare != 54 &&
                indexSquare != 63 && indexSquare != 57 && indexSquare != 58 &&
                indexSquare != 59 && indexSquare != 60 && indexSquare != 61 &&
                indexSquare != 62 &&
                (squares[indexSquare + 15] ==  0 || squares[indexSquare + 15] >= 7)) {

                possibilty.insert(indexSquare + 15);
            }

            break;
        }
        case 3: {

            break;
        }
        case 4: {
            for (int x: possibilty) {
                std::cout << x << std::endl;
            }
            break;
        }
        case 5: {
            for (int x: possibilty) {
                std::cout << x << std::endl;
            }
            break;
        }
        case 6: {
            for (int x: possibilty) {
                std::cout << x << std::endl;
            }
            break;
        }
        case 7: {
            //Diagonal esquerda
            if (indexSquare != 8 && indexSquare != 16 && indexSquare != 24 && indexSquare != 32 && indexSquare != 40 &&
                indexSquare != 48 && squares[(indexSquare - 9)] <= 7 && squares[(indexSquare - 9)] > 0) {
                possibilty.insert(indexSquare - 9);
            }

            //Diagonal direita
            if (indexSquare != 15 && indexSquare != 23 && indexSquare != 31 && indexSquare != 39 && indexSquare != 47 &&
                indexSquare != 55 && squares[(indexSquare -7)] <= 7 && squares[(indexSquare - 7)] > 0) {
                possibilty.insert(indexSquare - 7);
            }

            // 1 para frente
            if (squares[indexSquare - 8] == 0) {
                possibilty.insert(indexSquare - 8);
            }

            // 2 para frente
            if (indexSquare >= 48 && indexSquare <= 55 && squares[indexSquare - 8] == 0 &&
                squares[indexSquare - 16] == 0) {
                possibilty.insert(indexSquare - 16);
            }
            break;
        }
        case 8: {
            if (indexSquare != 8 && indexSquare != 16 && indexSquare != 24 &&
                indexSquare != 32 && indexSquare != 40 && indexSquare != 48 &&
                indexSquare != 56 && indexSquare != 1 && indexSquare != 9 &&
                indexSquare != 17 && indexSquare != 25 && indexSquare != 33 &&
                indexSquare != 41 && indexSquare != 49 && indexSquare != 57 &&
                indexSquare != 2 && indexSquare != 3 && indexSquare != 4 &&
                indexSquare != 5 && indexSquare != 6 && indexSquare != 7 &&
                indexSquare != 0 &&
                squares[indexSquare - 10] < 7) {

                possibilty.insert(indexSquare - 10);

            }

            if (indexSquare != 8 && indexSquare != 16 && indexSquare != 24 &&
                indexSquare != 32 && indexSquare != 40 && indexSquare != 48 &&
                indexSquare != 56 && indexSquare != 1 && indexSquare != 9 &&
                indexSquare != 17 && indexSquare != 25 && indexSquare != 33 &&
                indexSquare != 41 && indexSquare != 49 && indexSquare != 57 &&
                indexSquare != 58 && indexSquare != 59 && indexSquare != 60 &&
                indexSquare != 61 && indexSquare != 62 && indexSquare != 63 &&
                indexSquare != 0 &&
                squares[indexSquare + 6] < 7) {

                possibilty.insert(indexSquare + 6);
            }

            if (indexSquare != 8 && indexSquare != 16 && indexSquare != 24 &&
                indexSquare != 32 && indexSquare != 40 && indexSquare != 48 &&
                indexSquare != 56 && indexSquare != 1 && indexSquare != 2 &&
                indexSquare != 3 && indexSquare != 4 && indexSquare != 5 &&
                indexSquare != 6 && indexSquare != 7 && indexSquare != 9 &&
                indexSquare != 10 && indexSquare != 11 && indexSquare != 12 &&
                indexSquare != 13 && indexSquare != 14 && indexSquare != 15 &&
                indexSquare != 0 &&
                squares[indexSquare - 17] < 7) {

                possibilty.insert(indexSquare - 17);
            }

            if (indexSquare != 7 && indexSquare != 15 && indexSquare != 23 &&
                indexSquare != 31 && indexSquare != 39 && indexSquare != 47 &&
                indexSquare != 55 && indexSquare != 63 && indexSquare != 2 &&
                indexSquare != 3 && indexSquare != 4 && indexSquare != 5 &&
                indexSquare != 6 && indexSquare != 0 && indexSquare != 9 &&
                indexSquare != 10 && indexSquare != 11 && indexSquare != 12 &&
                indexSquare != 13 && indexSquare != 14 && indexSquare != 8 &&
                indexSquare != 1 &&
                squares[indexSquare - 15] < 7) {

                possibilty.insert(indexSquare - 15);
            }

            if (indexSquare != 7 && indexSquare != 15 && indexSquare != 23 &&
                indexSquare != 31 && indexSquare != 39 && indexSquare != 47 &&
                indexSquare != 55 && indexSquare != 63 && indexSquare != 6 &&
                indexSquare != 14 && indexSquare != 22 && indexSquare != 30 &&
                indexSquare != 38 && indexSquare != 46 && indexSquare != 54 &&
                indexSquare != 62 && indexSquare != 0 && indexSquare != 1 &&
                indexSquare != 2 && indexSquare != 3 && indexSquare != 4 &&
                indexSquare != 5 &&
                squares[indexSquare - 6] < 7) {

                possibilty.insert(indexSquare - 6);
            }

            if (indexSquare != 7 && indexSquare != 15 && indexSquare != 23 &&
                indexSquare != 31 && indexSquare != 39 && indexSquare != 47 &&
                indexSquare != 55 && indexSquare != 63 && indexSquare != 6 &&
                indexSquare != 14 && indexSquare != 22 && indexSquare != 30 &&
                indexSquare != 38 && indexSquare != 46 && indexSquare != 54 &&
                indexSquare != 62 && indexSquare != 56 && indexSquare != 57 &&
                indexSquare != 58 && indexSquare != 59 && indexSquare != 60 &&
                indexSquare != 61 &&
                squares[indexSquare +10] < 7) {

                possibilty.insert(indexSquare + 10);
            }

            if (indexSquare != 7 && indexSquare != 15 && indexSquare != 23 &&
                indexSquare != 31 && indexSquare != 39 && indexSquare != 47 &&
                indexSquare != 55 && indexSquare != 63 && indexSquare != 48 &&
                indexSquare != 49 && indexSquare != 50 && indexSquare != 51 &&
                indexSquare != 52 && indexSquare != 53 && indexSquare != 54 &&
                indexSquare != 56 && indexSquare != 57 && indexSquare != 58 &&
                indexSquare != 59 && indexSquare != 60 && indexSquare != 61 &&
                indexSquare != 62 &&
                squares[indexSquare +17] < 7) {

                possibilty.insert(indexSquare + 17);
            }

            if (indexSquare != 0 && indexSquare != 8 && indexSquare != 16 &&
                indexSquare != 24 && indexSquare != 32 && indexSquare != 40 &&
                indexSquare != 48 && indexSquare != 56 && indexSquare != 55 &&
                indexSquare != 49 && indexSquare != 50 && indexSquare != 51 &&
                indexSquare != 52 && indexSquare != 53 && indexSquare != 54 &&
                indexSquare != 63 && indexSquare != 57 && indexSquare != 58 &&
                indexSquare != 59 && indexSquare != 60 && indexSquare != 61 &&
                indexSquare != 62 &&
                squares[indexSquare +15] < 7) {

                possibilty.insert(indexSquare + 15);
            }
            break;
        }
        case 9: {
            for (int x: possibilty) {
                std::cout << x << std::endl;
            }
            break;
        }
        case 10: {
            for (int x: possibilty) {
                std::cout << x << std::endl;
            }
            break;
        }
        case 11: {
            for (int x: possibilty) {
                std::cout << x << std::endl;
            }
            break;
        }
        case 12: {
            for (int x: possibilty) {
                std::cout << x << std::endl;
            }
            break;
        }
        default: {

        }
    }
}

void ChessBoard::releasePiece(unsigned int mouseX, unsigned int mouseY) {
    mouseDragging = false;
    selectedPieceCode = -1;
}

void ChessBoard::drawPiece(int pieceCode, float xPos, float yPos, float boardEdge, sf::RenderWindow &window) {
    switch (pieceCode) {
        case 1:
            bPawn.draw(boardEdge, xPos, yPos, window);
            break;
        case 2:
            bKnight.draw(boardEdge, xPos, yPos, window);
            break;
        case 3:
            bBishop.draw(boardEdge, xPos, yPos, window);
            break;
        case 4:
            bRook.draw(boardEdge, xPos, yPos, window);
            break;
        case 5:
            bKing.draw(boardEdge, xPos, yPos, window);
            break;
        case 6:
            bQueen.draw(boardEdge, xPos, yPos, window);
            break;
        case 7:
            wPawn.draw(boardEdge, xPos, yPos, window);
            break;
        case 8:
            wKnight.draw(boardEdge, xPos, yPos, window);
            break;
        case 9:
            wBishop.draw(boardEdge, xPos, yPos, window);
            break;
        case 10:
            wRook.draw(boardEdge, xPos, yPos, window);
            break;
        case 11:
            wKing.draw(boardEdge, xPos, yPos, window);
            break;
        case 12:
            wQueen.draw(boardEdge, xPos, yPos, window);
            break;
        default:
            break;
    }
}

void ChessBoard::setMousePos(int mouseX, int mouseY) {
    this->mouseXpos = mouseX;
    this->mouseYpos = mouseY;
}


void generateMoves(std::set<int> &set) {
    for (int i: set) {
        set.insert(i * -1);
    }
};
