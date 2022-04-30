
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
    possibleSquare.setFillColor(sf::Color(2, 188, 0));

    initChessPieces();
}

void ChessBoard::draw(sf::RenderWindow &window) {

    window.draw(boardOutline);

    float boardEdge = (float) boardSize.x - 20;
    float squareEdge = boardEdge / 8.0f;

    lightSquare.setSize(sf::Vector2f(squareEdge, squareEdge));
    darkSquare.setSize(sf::Vector2f(squareEdge, squareEdge));
    selectedSquare.setSize(sf::Vector2f(squareEdge, squareEdge));
    possibleSquare.setSize(sf::Vector2f(squareEdge, squareEdge));

    for (int i = 0; i < 64; ++i) {
        float xPos = 10 + boardEdge / 8 * (float) (i % 8);
        float yPos = 10 + boardEdge / 8 * (float) (i / 8);
        lightSquare.setPosition(sf::Vector2f(xPos, yPos));
        darkSquare.setPosition(sf::Vector2f(xPos, yPos));
        selectedSquare.setPosition(sf::Vector2f(xPos, yPos));
        possibleSquare.setPosition(sf::Vector2f(xPos, yPos));

        if ((i % 2 + (int) (i / 8)) % 2 == 0)
            window.draw(lightSquare);
        else
            window.draw(darkSquare);

        if (selectedSquareIndex == i)
            window.draw(selectedSquare);

        if (possibilities.count(i)) {
            window.draw(possibleSquare);
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

    std::set<int> bishopMoves = {7, 14, 21, 28, 35, 42, 49};

//    std::set<int> bishopMoves = {+9, +18, +27, +36, +45, +54, +63, 7, 14, 21, 28, 35, 42, 49};
    std::set<int> knightMoves = {+10, +17, +15, +6};
    std::set<int> kingMoves = {+1, +2, -2, +7, +8, +9};
    std::set<int> queenMoves = {+1, +2, +3, +4, +5, +6, +7, +8, +16, +24, +32, +40, +48, +56, +9, +18, +27, +36, +45,
                                +54, +63, 7, 14, 21, 28, 35, 42, 49};

//    generateMoves(queenMoves);
//    generateMoves(rookMoves);
//    generateMoves(bishopMoves);
//    generateMoves(kingMoves);
//    generateMoves(knightMoves);

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

void ChessBoard::possibleMoves(int currentSquare, int pieceCode) {
    possibilities.clear();
    if (pieceCode == 0) return;

    std::set<int> selectedPieceMovementSet = std::set<int>();

//    for (int item: getPieceMovements(pieceCode)) {
//        int target = item + currentSquare;

//        if (target < 0) continue;
//        if (target > 63) continue;

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
            if ((pieceCode &0b11000000) == 0b10000000){

            }


            for (int i = 1; i <= spacesAbove; ++i) {
                selectedPieceMovementSet.insert(currentSquare - i * 2);
            }
            for (int i = 1; i <= spacesBelow; ++i) {
                selectedPieceMovementSet.insert(currentSquare + i * 2);
            }

            break;
        }
        case 0b10: {
            // knight
            selectedPieceMovementSet.insert({});
            break;
        }
        case 0b100: {
            // bishop
            for (int i = 1; i <= minTopLeft; ++i) {
                selectedPieceMovementSet.insert(currentSquare - i * 9);
            }
            for (int i = 1; i <= minTopRight; ++i) {
                selectedPieceMovementSet.insert(currentSquare - i * 7);
            }
            for (int i = 1; i <= minBottomLeft; ++i) {
                selectedPieceMovementSet.insert(currentSquare + i * 7);
            }
            for (int i = 1; i <= minBottomRight; ++i) {
                selectedPieceMovementSet.insert(currentSquare + i * 9);
            }
            break;
        }
        case 0b1000: {
            // rook
            for (int i = 1; i <= spacesAbove; ++i) {
                selectedPieceMovementSet.insert(currentSquare - i * 8);
            }
            for (int i = 1; i <= spacesBelow; ++i) {
                selectedPieceMovementSet.insert(currentSquare + i * 8);
            }
            for (int i = 1; i <= spacesLeft; ++i) {
                selectedPieceMovementSet.insert(currentSquare - i);
            }
            for (int i = 1; i <= spacesRight; ++i) {
                selectedPieceMovementSet.insert(currentSquare + i);
            }
            break;
        }
        case 0b10000: {
            // queen
            for (int i = 1; i <= minTopLeft; ++i) {
                selectedPieceMovementSet.insert(currentSquare - i * 9);
            }
            for (int i = 1; i <= minTopRight; ++i) {
                selectedPieceMovementSet.insert(currentSquare - i * 7);
            }
            for (int i = 1; i <= minBottomLeft; ++i) {
                selectedPieceMovementSet.insert(currentSquare + i * 7);
            }
            for (int i = 1; i <= minBottomRight; ++i) {
                selectedPieceMovementSet.insert(currentSquare + i * 9);
            }
            for (int i = 1; i <= spacesAbove; ++i) {
                selectedPieceMovementSet.insert(currentSquare - i * 8);
            }
            for (int i = 1; i <= spacesBelow; ++i) {
                selectedPieceMovementSet.insert(currentSquare + i * 8);
            }
            for (int i = 1; i <= spacesLeft; ++i) {
                selectedPieceMovementSet.insert(currentSquare - i);
            }
            for (int i = 1; i <= spacesRight; ++i) {
                selectedPieceMovementSet.insert(currentSquare + i);
            }
            break;
        }
        case 0b100000: {
            // king
            selectedPieceMovementSet.insert({});
            break;
        }

    }

    possibilities = selectedPieceMovementSet;
/*
//
//    switch (pieceCode) {
//        case 1: {
//            //Diagonal esquerda
//            if (currentSquare != 8 && currentSquare != 16 && currentSquare != 24 && currentSquare != 32 && currentSquare != 40 &&
//                currentSquare != 48 && squares[(currentSquare + 7)] >= 7) {
//                possibilities.insert(currentSquare + 7);
//            }
//
//            //Diagonal direita
//            if (currentSquare != 15 && currentSquare != 23 && currentSquare != 31 && currentSquare != 39 && currentSquare != 47 &&
//                currentSquare != 55 && squares[(currentSquare + 9)] >= 7) {
//                possibilities.insert(currentSquare + 9);
//            }
//
//            // 1 para frente
//            if (squares[currentSquare + 8] == 0) {
//                possibilities.insert(currentSquare + 8);
//            }
//
//            // 2 para frente
//            if (currentSquare >= 8 && currentSquare <= 15 && squares[currentSquare + 8] == 0 &&
//                squares[currentSquare + 16] == 0) {
//                possibilities.insert(currentSquare + 16);
//            }
//
//            break;
//        }
//        case 2: {
//            if (currentSquare != 8 && currentSquare != 16 && currentSquare != 24 &&
//                currentSquare != 32 && currentSquare != 40 && currentSquare != 48 &&
//                currentSquare != 56 && currentSquare != 1 && currentSquare != 9 &&
//                currentSquare != 17 && currentSquare != 25 && currentSquare != 33 &&
//                currentSquare != 41 && currentSquare != 49 && currentSquare != 57 &&
//                currentSquare != 2 && currentSquare != 3 && currentSquare != 4 &&
//                currentSquare != 5 && currentSquare != 6 && currentSquare != 7 &&
//                currentSquare != 0 &&
//                (squares[currentSquare - 10] ==  0 || squares[currentSquare - 10] >= 7)) {
//
//                possibilities.insert(currentSquare - 10);
//
//            }
//
//            if (currentSquare != 8 && currentSquare != 16 && currentSquare != 24 &&
//                currentSquare != 32 && currentSquare != 40 && currentSquare != 48 &&
//                currentSquare != 56 && currentSquare != 1 && currentSquare != 9 &&
//                currentSquare != 17 && currentSquare != 25 && currentSquare != 33 &&
//                currentSquare != 41 && currentSquare != 49 && currentSquare != 57 &&
//                currentSquare != 58 && currentSquare != 59 && currentSquare != 60 &&
//                currentSquare != 61 && currentSquare != 62 && currentSquare != 63 &&
//                currentSquare != 0 &&
//                (squares[currentSquare + 6] ==  0 || squares[currentSquare + 6] >= 7)) {
//
//                possibilities.insert(currentSquare + 6);
//            }
//
//            if (currentSquare != 8 && currentSquare != 16 && currentSquare != 24 &&
//                currentSquare != 32 && currentSquare != 40 && currentSquare != 48 &&
//                currentSquare != 56 && currentSquare != 1 && currentSquare != 2 &&
//                currentSquare != 3 && currentSquare != 4 && currentSquare != 5 &&
//                currentSquare != 6 && currentSquare != 7 && currentSquare != 9 &&
//                currentSquare != 10 && currentSquare != 11 && currentSquare != 12 &&
//                currentSquare != 13 && currentSquare != 14 && currentSquare != 15 &&
//                currentSquare != 0 &&
//                (squares[currentSquare - 17] ==  0 || squares[currentSquare - 17] >= 7)) {
//
//                possibilities.insert(currentSquare - 17);
//            }
//
//            if (currentSquare != 7 && currentSquare != 15 && currentSquare != 23 &&
//                currentSquare != 31 && currentSquare != 39 && currentSquare != 47 &&
//                currentSquare != 55 && currentSquare != 63 && currentSquare != 2 &&
//                currentSquare != 3 && currentSquare != 4 && currentSquare != 5 &&
//                currentSquare != 6 && currentSquare != 0 && currentSquare != 9 &&
//                currentSquare != 10 && currentSquare != 11 && currentSquare != 12 &&
//                currentSquare != 13 && currentSquare != 14 && currentSquare != 8 &&
//                currentSquare != 1 &&
//                (squares[currentSquare - 15] ==  0 || squares[currentSquare - 15] >= 7)) {
//
//                possibilities.insert(currentSquare - 15);
//            }
//
//            if (currentSquare != 7 && currentSquare != 15 && currentSquare != 23 &&
//                currentSquare != 31 && currentSquare != 39 && currentSquare != 47 &&
//                currentSquare != 55 && currentSquare != 63 && currentSquare != 6 &&
//                currentSquare != 14 && currentSquare != 22 && currentSquare != 30 &&
//                currentSquare != 38 && currentSquare != 46 && currentSquare != 54 &&
//                currentSquare != 62 && currentSquare != 0 && currentSquare != 1 &&
//                currentSquare != 2 && currentSquare != 3 && currentSquare != 4 &&
//                currentSquare != 5 &&
//                (squares[currentSquare - 6] ==  0 || squares[currentSquare - 6] >= 7)) {
//
//                possibilities.insert(currentSquare - 6);
//            }
//
//            if (currentSquare != 7 && currentSquare != 15 && currentSquare != 23 &&
//                currentSquare != 31 && currentSquare != 39 && currentSquare != 47 &&
//                currentSquare != 55 && currentSquare != 63 && currentSquare != 6 &&
//                currentSquare != 14 && currentSquare != 22 && currentSquare != 30 &&
//                currentSquare != 38 && currentSquare != 46 && currentSquare != 54 &&
//                currentSquare != 62 && currentSquare != 56 && currentSquare != 57 &&
//                currentSquare != 58 && currentSquare != 59 && currentSquare != 60 &&
//                currentSquare != 61 &&
//                (squares[currentSquare + 10] ==  0 || squares[currentSquare + 10] >= 7)) {
//
//                possibilities.insert(currentSquare + 10);
//            }
//
//            if (currentSquare != 7 && currentSquare != 15 && currentSquare != 23 &&
//                currentSquare != 31 && currentSquare != 39 && currentSquare != 47 &&
//                currentSquare != 55 && currentSquare != 63 && currentSquare != 48 &&
//                currentSquare != 49 && currentSquare != 50 && currentSquare != 51 &&
//                currentSquare != 52 && currentSquare != 53 && currentSquare != 54 &&
//                currentSquare != 56 && currentSquare != 57 && currentSquare != 58 &&
//                currentSquare != 59 && currentSquare != 60 && currentSquare != 61 &&
//                currentSquare != 62 &&
//                (squares[currentSquare + 17] ==  0 || squares[currentSquare + 17] >= 7)) {
//
//                possibilities.insert(currentSquare + 17);
//            }
//
//            if (currentSquare != 0 && currentSquare != 8 && currentSquare != 16 &&
//                currentSquare != 24 && currentSquare != 32 && currentSquare != 40 &&
//                currentSquare != 48 && currentSquare != 56 && currentSquare != 55 &&
//                currentSquare != 49 && currentSquare != 50 && currentSquare != 51 &&
//                currentSquare != 52 && currentSquare != 53 && currentSquare != 54 &&
//                currentSquare != 63 && currentSquare != 57 && currentSquare != 58 &&
//                currentSquare != 59 && currentSquare != 60 && currentSquare != 61 &&
//                currentSquare != 62 &&
//                (squares[currentSquare + 15] ==  0 || squares[currentSquare + 15] >= 7)) {
//
//                possibilities.insert(currentSquare + 15);
//            }
//
//            break;
//        }
//        case 3: {
//
//            break;
//        }
//        case 4: {
//            for (int x: possibilities) {
//                std::cout << x << std::endl;
//            }
//            break;
//        }
//        case 5: {
//            for (int x: possibilities) {
//                std::cout << x << std::endl;
//            }
//            break;
//        }
//        case 6: {
//            for (int x: possibilities) {
//                std::cout << x << std::endl;
//            }
//            break;
//        }
//        case 7: {
//            //Diagonal esquerda
//            if (currentSquare != 8 && currentSquare != 16 && currentSquare != 24 && currentSquare != 32 && currentSquare != 40 &&
//                currentSquare != 48 && squares[(currentSquare - 9)] <= 7 && squares[(currentSquare - 9)] > 0) {
//                possibilities.insert(currentSquare - 9);
//            }
//
//            //Diagonal direita
//            if (currentSquare != 15 && currentSquare != 23 && currentSquare != 31 && currentSquare != 39 && currentSquare != 47 &&
//                currentSquare != 55 && squares[(currentSquare -7)] <= 7 && squares[(currentSquare - 7)] > 0) {
//                possibilities.insert(currentSquare - 7);
//            }
//
//            // 1 para frente
//            if (squares[currentSquare - 8] == 0) {
//                possibilities.insert(currentSquare - 8);
//            }
//
//            // 2 para frente
//            if (currentSquare >= 48 && currentSquare <= 55 && squares[currentSquare - 8] == 0 &&
//                squares[currentSquare - 16] == 0) {
//                possibilities.insert(currentSquare - 16);
//            }
//            break;
//        }
//        case 8: {
//            if (currentSquare != 8 && currentSquare != 16 && currentSquare != 24 &&
//                currentSquare != 32 && currentSquare != 40 && currentSquare != 48 &&
//                currentSquare != 56 && currentSquare != 1 && currentSquare != 9 &&
//                currentSquare != 17 && currentSquare != 25 && currentSquare != 33 &&
//                currentSquare != 41 && currentSquare != 49 && currentSquare != 57 &&
//                currentSquare != 2 && currentSquare != 3 && currentSquare != 4 &&
//                currentSquare != 5 && currentSquare != 6 && currentSquare != 7 &&
//                currentSquare != 0 &&
//                squares[currentSquare - 10] < 7) {
//
//                possibilities.insert(currentSquare - 10);
//
//            }
//
//            if (currentSquare != 8 && currentSquare != 16 && currentSquare != 24 &&
//                currentSquare != 32 && currentSquare != 40 && currentSquare != 48 &&
//                currentSquare != 56 && currentSquare != 1 && currentSquare != 9 &&
//                currentSquare != 17 && currentSquare != 25 && currentSquare != 33 &&
//                currentSquare != 41 && currentSquare != 49 && currentSquare != 57 &&
//                currentSquare != 58 && currentSquare != 59 && currentSquare != 60 &&
//                currentSquare != 61 && currentSquare != 62 && currentSquare != 63 &&
//                currentSquare != 0 &&
//                squares[currentSquare + 6] < 7) {
//
//                possibilities.insert(currentSquare + 6);
//            }
//
//            if (currentSquare != 8 && currentSquare != 16 && currentSquare != 24 &&
//                currentSquare != 32 && currentSquare != 40 && currentSquare != 48 &&
//                currentSquare != 56 && currentSquare != 1 && currentSquare != 2 &&
//                currentSquare != 3 && currentSquare != 4 && currentSquare != 5 &&
//                currentSquare != 6 && currentSquare != 7 && currentSquare != 9 &&
//                currentSquare != 10 && currentSquare != 11 && currentSquare != 12 &&
//                currentSquare != 13 && currentSquare != 14 && currentSquare != 15 &&
//                currentSquare != 0 &&
//                squares[currentSquare - 17] < 7) {
//
//                possibilities.insert(currentSquare - 17);
//            }
//
//            if (currentSquare != 7 && currentSquare != 15 && currentSquare != 23 &&
//                currentSquare != 31 && currentSquare != 39 && currentSquare != 47 &&
//                currentSquare != 55 && currentSquare != 63 && currentSquare != 2 &&
//                currentSquare != 3 && currentSquare != 4 && currentSquare != 5 &&
//                currentSquare != 6 && currentSquare != 0 && currentSquare != 9 &&
//                currentSquare != 10 && currentSquare != 11 && currentSquare != 12 &&
//                currentSquare != 13 && currentSquare != 14 && currentSquare != 8 &&
//                currentSquare != 1 &&
//                squares[currentSquare - 15] < 7) {
//
//                possibilities.insert(currentSquare - 15);
//            }
//
//            if (currentSquare != 7 && currentSquare != 15 && currentSquare != 23 &&
//                currentSquare != 31 && currentSquare != 39 && currentSquare != 47 &&
//                currentSquare != 55 && currentSquare != 63 && currentSquare != 6 &&
//                currentSquare != 14 && currentSquare != 22 && currentSquare != 30 &&
//                currentSquare != 38 && currentSquare != 46 && currentSquare != 54 &&
//                currentSquare != 62 && currentSquare != 0 && currentSquare != 1 &&
//                currentSquare != 2 && currentSquare != 3 && currentSquare != 4 &&
//                currentSquare != 5 &&
//                squares[currentSquare - 6] < 7) {
//
//                possibilities.insert(currentSquare - 6);
//            }
//
//            if (currentSquare != 7 && currentSquare != 15 && currentSquare != 23 &&
//                currentSquare != 31 && currentSquare != 39 && currentSquare != 47 &&
//                currentSquare != 55 && currentSquare != 63 && currentSquare != 6 &&
//                currentSquare != 14 && currentSquare != 22 && currentSquare != 30 &&
//                currentSquare != 38 && currentSquare != 46 && currentSquare != 54 &&
//                currentSquare != 62 && currentSquare != 56 && currentSquare != 57 &&
//                currentSquare != 58 && currentSquare != 59 && currentSquare != 60 &&
//                currentSquare != 61 &&
//                squares[currentSquare +10] < 7) {
//
//                possibilities.insert(currentSquare + 10);
//            }
//
//            if (currentSquare != 7 && currentSquare != 15 && currentSquare != 23 &&
//                currentSquare != 31 && currentSquare != 39 && currentSquare != 47 &&
//                currentSquare != 55 && currentSquare != 63 && currentSquare != 48 &&
//                currentSquare != 49 && currentSquare != 50 && currentSquare != 51 &&
//                currentSquare != 52 && currentSquare != 53 && currentSquare != 54 &&
//                currentSquare != 56 && currentSquare != 57 && currentSquare != 58 &&
//                currentSquare != 59 && currentSquare != 60 && currentSquare != 61 &&
//                currentSquare != 62 &&
//                squares[currentSquare +17] < 7) {
//
//                possibilities.insert(currentSquare + 17);
//            }
//
//            if (currentSquare != 0 && currentSquare != 8 && currentSquare != 16 &&
//                currentSquare != 24 && currentSquare != 32 && currentSquare != 40 &&
//                currentSquare != 48 && currentSquare != 56 && currentSquare != 55 &&
//                currentSquare != 49 && currentSquare != 50 && currentSquare != 51 &&
//                currentSquare != 52 && currentSquare != 53 && currentSquare != 54 &&
//                currentSquare != 63 && currentSquare != 57 && currentSquare != 58 &&
//                currentSquare != 59 && currentSquare != 60 && currentSquare != 61 &&
//                currentSquare != 62 &&
//                squares[currentSquare +15] < 7) {
//
//                possibilities.insert(currentSquare + 15);
//            }
//            break;
//        }
//        case 9: {
//            for (int x: possibilities) {
//                std::cout << x << std::endl;
//            }
//            break;
//        }
//        case 10: {
//            for (int x: possibilities) {
//                std::cout << x << std::endl;
//            }
//            break;
//        }
//        case 11: {
//            for (int x: possibilities) {
//                std::cout << x << std::endl;
//            }
//            break;
//        }
//        case 12: {
//            for (int x: possibilities) {
//                std::cout << x << std::endl;
//            }
//            break;
//        }
//        default: {
//
//        }
//    }
*/
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
