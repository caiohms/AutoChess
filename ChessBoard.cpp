
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

        if (std::find(possibilities.begin(), possibilities.end(), i) != possibilities.end()) {
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
    std::set<int> bishopMoves = {+9, +18, +27, +36, +45, +54, +63, 7, 14, 21, 28, 35, 42, 49};
    std::set<int> knightMoves = {+10, +17, +15, +6};
    std::set<int> kingMoves = {+1, +2, -2, +7, +8, +9};
    std::set<int> queenMoves = {+1, +2, +3, +4, +5, +6, +7, +8, +16, +24, +32, +40, +48, +56, +9, +18, +27, +36, +45,
                                +54, +63, 7, 14, 21, 28, 35, 42, 49};

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

void ChessBoard::possibleMoves(int squareIndex, int pieceCode) {
    //std::cout << squareIndex << std::endl;
    //std::cout << pieceCode << std::endl;

    possibilities.clear();
    if (pieceCode == 0) {
        return;
    }

    std::set<int> selectedPieceMovementSet = std::set<int>();

    for (int item: getPieceMovements(pieceCode)) {
        int target = item + squareIndex;

        if (target < 0) continue;
        if (target > 63) continue;

        int leftSpaces = 7 - squareIndex % 7;
        int rightSpaces = squareIndex / 7;

//        if ( item + rightSpaces)


            selectedPieceMovementSet.insert(target);
    }

    for (int movement: selectedPieceMovementSet) {
        possibilities = selectedPieceMovementSet;
    }




/*
//
//    switch (pieceCode) {
//        case 1: {
//            //Diagonal esquerda
//            if (squareIndex != 8 && squareIndex != 16 && squareIndex != 24 && squareIndex != 32 && squareIndex != 40 &&
//                squareIndex != 48 && squares[(squareIndex + 7)] >= 7) {
//                possibilities.insert(squareIndex + 7);
//            }
//
//            //Diagonal direita
//            if (squareIndex != 15 && squareIndex != 23 && squareIndex != 31 && squareIndex != 39 && squareIndex != 47 &&
//                squareIndex != 55 && squares[(squareIndex + 9)] >= 7) {
//                possibilities.insert(squareIndex + 9);
//            }
//
//            // 1 para frente
//            if (squares[squareIndex + 8] == 0) {
//                possibilities.insert(squareIndex + 8);
//            }
//
//            // 2 para frente
//            if (squareIndex >= 8 && squareIndex <= 15 && squares[squareIndex + 8] == 0 &&
//                squares[squareIndex + 16] == 0) {
//                possibilities.insert(squareIndex + 16);
//            }
//
//            break;
//        }
//        case 2: {
//            if (squareIndex != 8 && squareIndex != 16 && squareIndex != 24 &&
//                squareIndex != 32 && squareIndex != 40 && squareIndex != 48 &&
//                squareIndex != 56 && squareIndex != 1 && squareIndex != 9 &&
//                squareIndex != 17 && squareIndex != 25 && squareIndex != 33 &&
//                squareIndex != 41 && squareIndex != 49 && squareIndex != 57 &&
//                squareIndex != 2 && squareIndex != 3 && squareIndex != 4 &&
//                squareIndex != 5 && squareIndex != 6 && squareIndex != 7 &&
//                squareIndex != 0 &&
//                (squares[squareIndex - 10] ==  0 || squares[squareIndex - 10] >= 7)) {
//
//                possibilities.insert(squareIndex - 10);
//
//            }
//
//            if (squareIndex != 8 && squareIndex != 16 && squareIndex != 24 &&
//                squareIndex != 32 && squareIndex != 40 && squareIndex != 48 &&
//                squareIndex != 56 && squareIndex != 1 && squareIndex != 9 &&
//                squareIndex != 17 && squareIndex != 25 && squareIndex != 33 &&
//                squareIndex != 41 && squareIndex != 49 && squareIndex != 57 &&
//                squareIndex != 58 && squareIndex != 59 && squareIndex != 60 &&
//                squareIndex != 61 && squareIndex != 62 && squareIndex != 63 &&
//                squareIndex != 0 &&
//                (squares[squareIndex + 6] ==  0 || squares[squareIndex + 6] >= 7)) {
//
//                possibilities.insert(squareIndex + 6);
//            }
//
//            if (squareIndex != 8 && squareIndex != 16 && squareIndex != 24 &&
//                squareIndex != 32 && squareIndex != 40 && squareIndex != 48 &&
//                squareIndex != 56 && squareIndex != 1 && squareIndex != 2 &&
//                squareIndex != 3 && squareIndex != 4 && squareIndex != 5 &&
//                squareIndex != 6 && squareIndex != 7 && squareIndex != 9 &&
//                squareIndex != 10 && squareIndex != 11 && squareIndex != 12 &&
//                squareIndex != 13 && squareIndex != 14 && squareIndex != 15 &&
//                squareIndex != 0 &&
//                (squares[squareIndex - 17] ==  0 || squares[squareIndex - 17] >= 7)) {
//
//                possibilities.insert(squareIndex - 17);
//            }
//
//            if (squareIndex != 7 && squareIndex != 15 && squareIndex != 23 &&
//                squareIndex != 31 && squareIndex != 39 && squareIndex != 47 &&
//                squareIndex != 55 && squareIndex != 63 && squareIndex != 2 &&
//                squareIndex != 3 && squareIndex != 4 && squareIndex != 5 &&
//                squareIndex != 6 && squareIndex != 0 && squareIndex != 9 &&
//                squareIndex != 10 && squareIndex != 11 && squareIndex != 12 &&
//                squareIndex != 13 && squareIndex != 14 && squareIndex != 8 &&
//                squareIndex != 1 &&
//                (squares[squareIndex - 15] ==  0 || squares[squareIndex - 15] >= 7)) {
//
//                possibilities.insert(squareIndex - 15);
//            }
//
//            if (squareIndex != 7 && squareIndex != 15 && squareIndex != 23 &&
//                squareIndex != 31 && squareIndex != 39 && squareIndex != 47 &&
//                squareIndex != 55 && squareIndex != 63 && squareIndex != 6 &&
//                squareIndex != 14 && squareIndex != 22 && squareIndex != 30 &&
//                squareIndex != 38 && squareIndex != 46 && squareIndex != 54 &&
//                squareIndex != 62 && squareIndex != 0 && squareIndex != 1 &&
//                squareIndex != 2 && squareIndex != 3 && squareIndex != 4 &&
//                squareIndex != 5 &&
//                (squares[squareIndex - 6] ==  0 || squares[squareIndex - 6] >= 7)) {
//
//                possibilities.insert(squareIndex - 6);
//            }
//
//            if (squareIndex != 7 && squareIndex != 15 && squareIndex != 23 &&
//                squareIndex != 31 && squareIndex != 39 && squareIndex != 47 &&
//                squareIndex != 55 && squareIndex != 63 && squareIndex != 6 &&
//                squareIndex != 14 && squareIndex != 22 && squareIndex != 30 &&
//                squareIndex != 38 && squareIndex != 46 && squareIndex != 54 &&
//                squareIndex != 62 && squareIndex != 56 && squareIndex != 57 &&
//                squareIndex != 58 && squareIndex != 59 && squareIndex != 60 &&
//                squareIndex != 61 &&
//                (squares[squareIndex + 10] ==  0 || squares[squareIndex + 10] >= 7)) {
//
//                possibilities.insert(squareIndex + 10);
//            }
//
//            if (squareIndex != 7 && squareIndex != 15 && squareIndex != 23 &&
//                squareIndex != 31 && squareIndex != 39 && squareIndex != 47 &&
//                squareIndex != 55 && squareIndex != 63 && squareIndex != 48 &&
//                squareIndex != 49 && squareIndex != 50 && squareIndex != 51 &&
//                squareIndex != 52 && squareIndex != 53 && squareIndex != 54 &&
//                squareIndex != 56 && squareIndex != 57 && squareIndex != 58 &&
//                squareIndex != 59 && squareIndex != 60 && squareIndex != 61 &&
//                squareIndex != 62 &&
//                (squares[squareIndex + 17] ==  0 || squares[squareIndex + 17] >= 7)) {
//
//                possibilities.insert(squareIndex + 17);
//            }
//
//            if (squareIndex != 0 && squareIndex != 8 && squareIndex != 16 &&
//                squareIndex != 24 && squareIndex != 32 && squareIndex != 40 &&
//                squareIndex != 48 && squareIndex != 56 && squareIndex != 55 &&
//                squareIndex != 49 && squareIndex != 50 && squareIndex != 51 &&
//                squareIndex != 52 && squareIndex != 53 && squareIndex != 54 &&
//                squareIndex != 63 && squareIndex != 57 && squareIndex != 58 &&
//                squareIndex != 59 && squareIndex != 60 && squareIndex != 61 &&
//                squareIndex != 62 &&
//                (squares[squareIndex + 15] ==  0 || squares[squareIndex + 15] >= 7)) {
//
//                possibilities.insert(squareIndex + 15);
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
//            if (squareIndex != 8 && squareIndex != 16 && squareIndex != 24 && squareIndex != 32 && squareIndex != 40 &&
//                squareIndex != 48 && squares[(squareIndex - 9)] <= 7 && squares[(squareIndex - 9)] > 0) {
//                possibilities.insert(squareIndex - 9);
//            }
//
//            //Diagonal direita
//            if (squareIndex != 15 && squareIndex != 23 && squareIndex != 31 && squareIndex != 39 && squareIndex != 47 &&
//                squareIndex != 55 && squares[(squareIndex -7)] <= 7 && squares[(squareIndex - 7)] > 0) {
//                possibilities.insert(squareIndex - 7);
//            }
//
//            // 1 para frente
//            if (squares[squareIndex - 8] == 0) {
//                possibilities.insert(squareIndex - 8);
//            }
//
//            // 2 para frente
//            if (squareIndex >= 48 && squareIndex <= 55 && squares[squareIndex - 8] == 0 &&
//                squares[squareIndex - 16] == 0) {
//                possibilities.insert(squareIndex - 16);
//            }
//            break;
//        }
//        case 8: {
//            if (squareIndex != 8 && squareIndex != 16 && squareIndex != 24 &&
//                squareIndex != 32 && squareIndex != 40 && squareIndex != 48 &&
//                squareIndex != 56 && squareIndex != 1 && squareIndex != 9 &&
//                squareIndex != 17 && squareIndex != 25 && squareIndex != 33 &&
//                squareIndex != 41 && squareIndex != 49 && squareIndex != 57 &&
//                squareIndex != 2 && squareIndex != 3 && squareIndex != 4 &&
//                squareIndex != 5 && squareIndex != 6 && squareIndex != 7 &&
//                squareIndex != 0 &&
//                squares[squareIndex - 10] < 7) {
//
//                possibilities.insert(squareIndex - 10);
//
//            }
//
//            if (squareIndex != 8 && squareIndex != 16 && squareIndex != 24 &&
//                squareIndex != 32 && squareIndex != 40 && squareIndex != 48 &&
//                squareIndex != 56 && squareIndex != 1 && squareIndex != 9 &&
//                squareIndex != 17 && squareIndex != 25 && squareIndex != 33 &&
//                squareIndex != 41 && squareIndex != 49 && squareIndex != 57 &&
//                squareIndex != 58 && squareIndex != 59 && squareIndex != 60 &&
//                squareIndex != 61 && squareIndex != 62 && squareIndex != 63 &&
//                squareIndex != 0 &&
//                squares[squareIndex + 6] < 7) {
//
//                possibilities.insert(squareIndex + 6);
//            }
//
//            if (squareIndex != 8 && squareIndex != 16 && squareIndex != 24 &&
//                squareIndex != 32 && squareIndex != 40 && squareIndex != 48 &&
//                squareIndex != 56 && squareIndex != 1 && squareIndex != 2 &&
//                squareIndex != 3 && squareIndex != 4 && squareIndex != 5 &&
//                squareIndex != 6 && squareIndex != 7 && squareIndex != 9 &&
//                squareIndex != 10 && squareIndex != 11 && squareIndex != 12 &&
//                squareIndex != 13 && squareIndex != 14 && squareIndex != 15 &&
//                squareIndex != 0 &&
//                squares[squareIndex - 17] < 7) {
//
//                possibilities.insert(squareIndex - 17);
//            }
//
//            if (squareIndex != 7 && squareIndex != 15 && squareIndex != 23 &&
//                squareIndex != 31 && squareIndex != 39 && squareIndex != 47 &&
//                squareIndex != 55 && squareIndex != 63 && squareIndex != 2 &&
//                squareIndex != 3 && squareIndex != 4 && squareIndex != 5 &&
//                squareIndex != 6 && squareIndex != 0 && squareIndex != 9 &&
//                squareIndex != 10 && squareIndex != 11 && squareIndex != 12 &&
//                squareIndex != 13 && squareIndex != 14 && squareIndex != 8 &&
//                squareIndex != 1 &&
//                squares[squareIndex - 15] < 7) {
//
//                possibilities.insert(squareIndex - 15);
//            }
//
//            if (squareIndex != 7 && squareIndex != 15 && squareIndex != 23 &&
//                squareIndex != 31 && squareIndex != 39 && squareIndex != 47 &&
//                squareIndex != 55 && squareIndex != 63 && squareIndex != 6 &&
//                squareIndex != 14 && squareIndex != 22 && squareIndex != 30 &&
//                squareIndex != 38 && squareIndex != 46 && squareIndex != 54 &&
//                squareIndex != 62 && squareIndex != 0 && squareIndex != 1 &&
//                squareIndex != 2 && squareIndex != 3 && squareIndex != 4 &&
//                squareIndex != 5 &&
//                squares[squareIndex - 6] < 7) {
//
//                possibilities.insert(squareIndex - 6);
//            }
//
//            if (squareIndex != 7 && squareIndex != 15 && squareIndex != 23 &&
//                squareIndex != 31 && squareIndex != 39 && squareIndex != 47 &&
//                squareIndex != 55 && squareIndex != 63 && squareIndex != 6 &&
//                squareIndex != 14 && squareIndex != 22 && squareIndex != 30 &&
//                squareIndex != 38 && squareIndex != 46 && squareIndex != 54 &&
//                squareIndex != 62 && squareIndex != 56 && squareIndex != 57 &&
//                squareIndex != 58 && squareIndex != 59 && squareIndex != 60 &&
//                squareIndex != 61 &&
//                squares[squareIndex +10] < 7) {
//
//                possibilities.insert(squareIndex + 10);
//            }
//
//            if (squareIndex != 7 && squareIndex != 15 && squareIndex != 23 &&
//                squareIndex != 31 && squareIndex != 39 && squareIndex != 47 &&
//                squareIndex != 55 && squareIndex != 63 && squareIndex != 48 &&
//                squareIndex != 49 && squareIndex != 50 && squareIndex != 51 &&
//                squareIndex != 52 && squareIndex != 53 && squareIndex != 54 &&
//                squareIndex != 56 && squareIndex != 57 && squareIndex != 58 &&
//                squareIndex != 59 && squareIndex != 60 && squareIndex != 61 &&
//                squareIndex != 62 &&
//                squares[squareIndex +17] < 7) {
//
//                possibilities.insert(squareIndex + 17);
//            }
//
//            if (squareIndex != 0 && squareIndex != 8 && squareIndex != 16 &&
//                squareIndex != 24 && squareIndex != 32 && squareIndex != 40 &&
//                squareIndex != 48 && squareIndex != 56 && squareIndex != 55 &&
//                squareIndex != 49 && squareIndex != 50 && squareIndex != 51 &&
//                squareIndex != 52 && squareIndex != 53 && squareIndex != 54 &&
//                squareIndex != 63 && squareIndex != 57 && squareIndex != 58 &&
//                squareIndex != 59 && squareIndex != 60 && squareIndex != 61 &&
//                squareIndex != 62 &&
//                squares[squareIndex +15] < 7) {
//
//                possibilities.insert(squareIndex + 15);
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

std::set<int> ChessBoard::getPieceMovements(int pieceCode) {
    switch (pieceCode) {
        case 1:
            return bPawn.moves;
            break;
        case 6:
            return bQueen.moves;
    }
    return {};
}


void generateMoves(std::set<int> &set) {
    for (int i: set) {
        set.insert(i * -1);
    }
};
