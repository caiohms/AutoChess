
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

    bPawn.Init("resources\\sprites\\b_pawn_png_shadow_256px.png", bPawnMoves);
    bBishop.Init("resources\\sprites\\b_bishop_png_shadow_256px.png", bishopMoves);
    bKnight.Init("resources\\sprites\\b_knight_png_shadow_256px.png", knightMoves);
    bRook.Init("resources\\sprites\\b_rook_png_shadow_256px.png", rookMoves);
    bQueen.Init("resources\\sprites\\b_queen_png_shadow_256px.png", queenMoves);
    bKing.Init("resources\\sprites\\b_king_png_shadow_256px.png", kingMoves);

    wPawn.Init("resources\\sprites\\w_pawn_png_shadow_256px.png", wPawnMoves);
    wBishop.Init("resources\\sprites\\w_bishop_png_shadow_256px.png", bishopMoves);
    wKnight.Init("resources\\sprites\\w_knight_png_shadow_256px.png", knightMoves);
    wRook.Init("resources\\sprites\\w_rook_png_shadow_256px.png", rookMoves);
    wQueen.Init("resources\\sprites\\w_queen_png_shadow_256px.png", queenMoves);
    wKing.Init("resources\\sprites\\w_king_png_shadow_256px.png", kingMoves);
}


void generateMoves(std::set<int> &set) {
    for (int i: set) {
        set.insert(i * -1);
    }
};