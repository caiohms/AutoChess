
#include "ChessBoard.h"
#include "ChessPiece.h"

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

    initChessPieces();
}

void ChessBoard::draw(sf::RenderWindow &window) {

    window.draw(boardOutline);

    float edge = (float) boardSize.x - 20;

    lightSquare.setSize(sf::Vector2f(edge / 8, edge / 8));
    darkSquare.setSize(sf::Vector2f(edge / 8, edge / 8));

    for (int i = 0; i < 64; ++i) {
        float xPos = 10 + edge / 8 * (i % 8);
        float yPos = 10 + edge / 8 * (i / 8);
        lightSquare.setPosition(sf::Vector2f(xPos, yPos));
        darkSquare.setPosition(sf::Vector2f(xPos, yPos));

        if ((i % 2 + (int) (i / 8)) % 2 == 0)
            window.draw(lightSquare);
        else
            window.draw(darkSquare);

        switch (squares[i]) {
            case 1:
                bPawn.draw(edge, xPos, yPos, window);
                break;
            case 2:
                bKnight.draw(edge, xPos, yPos, window);
                break;
            case 3:
                bBishop.draw(edge, xPos, yPos, window);
                break;
            case 4:
                bRook.draw(edge, xPos, yPos, window);
                break;
            case 5:
                bKing.draw(edge, xPos, yPos, window);
                break;
            case 6:
                bQueen.draw(edge, xPos, yPos, window);
                break;
            case 7:
                wPawn.draw(edge, xPos, yPos, window);
                break;
            case 8:
                wKnight.draw(edge, xPos, yPos, window);
                break;
            case 9:
                wBishop.draw(edge, xPos, yPos, window);
                break;
            case 10:
                wRook.draw(edge, xPos, yPos, window);
                break;
            case 11:
                wKing.draw(edge, xPos, yPos, window);
                break;
            case 12:
                wQueen.draw(edge, xPos, yPos, window);
                break;
            default:
                break;
        }
    }
}


void ChessBoard::setBoardSize(const sf::Vector2u &boardSize) {
    boardOutline.setSize(sf::Vector2f((float) boardSize.x - 20, (float) boardSize.y - 20));
    ChessBoard::boardSize = boardSize;
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


void generateMoves(std::set<int> &set) {
    for (int i: set) {
        set.insert(i * -1);
    }
};