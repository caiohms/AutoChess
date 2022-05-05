
#include <SFML\\Graphics.hpp>
#include "ChessBoard.h"
#include "ChessGame.h"
#include <algorithm>
#include <iostream>
#include <chrono>

#define MIN_WINDOW_WIDTH 1280
#define MIN_WINDOW_HEIGHT 720

#define INIT_WINDOW_WIDTH 1280
#define INIT_WINDOW_HEIGHT 720

int main() {

    sf::RenderWindow window(sf::VideoMode(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT), "O Xadrez da vez");
    sf::Font font;

    if (!font.loadFromFile("resources\\fonts\\Roboto-Regular.ttf")) {
        std::cout << "Failed to load font" << std::endl;
        system("pause");
    }

    PlayerTurn turn = WHITE;
    ChessBoard board(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT, font, turn, window);
    ChessGame game(board, turn);

    window.setFramerateLimit(60);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            switch (event.type) {
                case sf::Event::Resized: {
                    unsigned int w = event.size.width;
                    unsigned int h = event.size.height;

                    if (w < MIN_WINDOW_WIDTH) w = MIN_WINDOW_WIDTH;
                    if (h < MIN_WINDOW_HEIGHT) h = MIN_WINDOW_HEIGHT;

                    unsigned int minSide = std::min(w, h);

                    window.setSize(sf::Vector2u(w, h));
                    window.setView(sf::View(sf::FloatRect(0, 0, (float) w, (float) h)));
                    game.setBoardSize(sf::Vector2u(minSide, minSide));
                    break;
                }

                case sf::Event::MouseMoved: {
                    game.setMousePos(event.mouseMove.x, event.mouseMove.y);
                    break;
                }

                case sf::Event::MouseButtonPressed: {
                    switch (event.key.code) {
                        case sf::Mouse::Button::Left: {
                            int x = event.mouseButton.x;
                            int y = event.mouseButton.y;
//                            game.grabPiece(x, y);

                            for (int i = 1; i <= 7; ++i) {
                                using std::chrono::high_resolution_clock;
                                using std::chrono::duration_cast;
                                using std::chrono::duration;
                                using std::chrono::milliseconds;

                                auto t1 = high_resolution_clock::now();

                                long a = game.moveMaker(i, window, PlayerTurn::BLACK);

                                auto t2 = high_resolution_clock::now();

                                /* Getting number of milliseconds as an integer. */
                                auto ms_int = duration_cast<milliseconds>(t2 - t1);

                                std::cout << "Depth: " << i << " Result: " << a << " positions  Time: " << ms_int.count() << "ms\n";
                            }

                            break;
                        }
                        default:
                            break;
                    }
                    break;
                }

                case sf::Event::MouseButtonReleased: {
                    switch (event.key.code) {
                        case sf::Mouse::Button::Left: {
                            int x = event.mouseButton.x;
                            int y = event.mouseButton.y;
                            game.releasePiece(x, y);
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                }
            }
        }

        window.clear(sf::Color(35, 57, 76));
        game.draw(window);
        window.display();
    }
}

