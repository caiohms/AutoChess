
#include <SFML\\Graphics.hpp>
#include "ChessPiece.h"
#include "ChessBoard.h"
#include <iostream>

#define MIN_WINDOW_WIDTH 1280
#define MIN_WINDOW_HEIGHT 720

#define INIT_WINDOW_WIDTH 1600
#define INIT_WINDOW_HEIGHT 900


int main() {

    sf::RenderWindow window(sf::VideoMode(1600, 900), "AutoChess");

    ChessPiece bPawn("resources\\sprites\\b_pawn_png_shadow_256px.png");
    ChessPiece bBishop("resources\\sprites\\b_bishop_png_shadow_256px.png");
    ChessPiece bKnight("resources\\sprites\\b_knight_png_shadow_256px.png");
    ChessPiece bRook("resources\\sprites\\b_rook_png_shadow_256px.png");
    ChessPiece bQueen("resources\\sprites\\b_queen_png_shadow_256px.png");
    ChessPiece bKing("resources\\sprites\\b_king_png_shadow_256px.png");

    ChessPiece wPawn("resources\\sprites\\w_pawn_png_shadow_256px.png");
    ChessPiece wBishop("resources\\sprites\\w_bishop_png_shadow_256px.png");
    ChessPiece wKnight("resources\\sprites\\w_knight_png_shadow_256px.png");
    ChessPiece wRook("resources\\sprites\\w_rook_png_shadow_256px.png");
    ChessPiece wQueen("resources\\sprites\\w_queen_png_shadow_256px.png");
    ChessPiece wKing("resources\\sprites\\w_king_png_shadow_256px.png");

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