
#include <SFML\\Graphics.hpp>
#include "ChessPiece.h"
#include "ChessBoard.h"
#include <iostream>

#define MIN_WINDOW_WIDTH 1280
#define MIN_WINDOW_HEIGHT 720

#define INIT_WINDOW_WIDTH 1600
#define INIT_WINDOW_HEIGHT 900

void generateMoves(std::set<int> & set);

#include <iostream>
#include <iterator>
#include <algorithm>

int main() {

    sf::RenderWindow window(sf::VideoMode(1600, 900), "AutoChess");

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

    ChessPiece bPawn("resources\\sprites\\b_pawn_png_shadow_256px.png", bPawnMoves);
    ChessPiece bBishop("resources\\sprites\\b_bishop_png_shadow_256px.png", bishopMoves);
    ChessPiece bKnight("resources\\sprites\\b_knight_png_shadow_256px.png", knightMoves);
    ChessPiece bRook("resources\\sprites\\b_rook_png_shadow_256px.png", rookMoves);
    ChessPiece bQueen("resources\\sprites\\b_queen_png_shadow_256px.png", queenMoves);
    ChessPiece bKing("resources\\sprites\\b_king_png_shadow_256px.png", kingMoves);

    ChessPiece wPawn("resources\\sprites\\w_pawn_png_shadow_256px.png", wPawnMoves);
    ChessPiece wBishop("resources\\sprites\\w_bishop_png_shadow_256px.png", bishopMoves);
    ChessPiece wKnight("resources\\sprites\\w_knight_png_shadow_256px.png", knightMoves);
    ChessPiece wRook("resources\\sprites\\w_rook_png_shadow_256px.png", rookMoves);
    ChessPiece wQueen("resources\\sprites\\w_queen_png_shadow_256px.png", queenMoves);
    ChessPiece wKing("resources\\sprites\\w_king_png_shadow_256px.png", kingMoves);

    ChessBoard board(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT);


    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            switch (event.type) {
                case sf::Event::Resized: {
                    int w = event.size.width;
                    int h = event.size.height;

                    if (w < MIN_WINDOW_WIDTH) w = MIN_WINDOW_WIDTH;
                    if (h < MIN_WINDOW_HEIGHT) h = MIN_WINDOW_HEIGHT;

                    int minSide = std::min(w, h);

                    window.setSize(sf::Vector2u(w, h));
                    window.setView(sf::View(sf::FloatRect(0, 0, w, h)));
                    board.setBoardSize(sf::Vector2u(minSide, minSide));
                    break;
                }

            }

        }

        window.clear(sf::Color(35, 57, 76));
        board.draw(window);
        window.display();
    }
}

void generateMoves(std::set<int> & set) {
    for (int i: set) {
        set.insert(i * -1);
    }
};