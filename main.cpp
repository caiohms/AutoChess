#include <SFML\\Graphics.hpp>
#include "ChessPiece.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(640, 480), "SFML Application");
    sf::CircleShape shape;

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


    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        bKing.draw(window);
        window.draw(shape);
        window.display();
    }
}