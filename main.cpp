
#include <SFML\\Graphics.hpp>
#include "ChessBoard.h"
#include "ChessGame.h"
#include "AI.h"
#include <iostream>

// Window sizes definition
#define MIN_WINDOW_WIDTH 480
#define MIN_WINDOW_HEIGHT 480
#define INIT_WINDOW_WIDTH 1280
#define INIT_WINDOW_HEIGHT 720

int main() {

    sf::RenderWindow window(sf::VideoMode(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT), "Le Forque");
    window.setFramerateLimit(60);
    sf::Font font;
    if (!font.loadFromFile("resources\\fonts\\Roboto-Regular.ttf")) {
        std::cout << "Failed to load font" << std::endl;
        system("pause");
    }

    // The turn variable is global and referenced by multiple objects
    bool turn = true;

    // Creating ChessBoard, AI, ChessGame objects
    ChessBoard board(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT, font, turn, window);
    AI ai;
    ChessGame game(board, turn, ai);

    while (window.isOpen()) {
        sf::Event event;

        // Catches window events
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            switch (event.type) {
                // If the window is resized, we need to resize all game elements drawn within it
                case sf::Event::Resized: {
                    unsigned int w = event.size.width;
                    unsigned int h = event.size.height;

                    // Assure window size greater than minimum values
                    if (w < MIN_WINDOW_WIDTH) w = MIN_WINDOW_WIDTH;
                    if (h < MIN_WINDOW_HEIGHT) h = MIN_WINDOW_HEIGHT;

                    // The chess board is a square so we use the minimum value between width and height
                    unsigned int minSide = std::min(w, h);

                    window.setSize(sf::Vector2u(w, h));
                    window.setView(sf::View(sf::FloatRect(0, 0, (float) w, (float) h)));
                    game.setGameSize(sf::Vector2u(minSide, minSide));
                    break;
                }

                case sf::Event::MouseMoved: {
                    // Send mouse movement to the game object. This is used for dragging pieces
                    game.setMousePos(event.mouseMove.x, event.mouseMove.y);
                    break;
                }

                case sf::Event::MouseButtonPressed: {
                    // When any mouse button is pressed, we determine which button was pressed and act accordingly.
                    // Note that this event only catches Mouse Down events (releasing is handled in another event below)
                    switch (event.mouseButton.button) {
                        case sf::Mouse::Button::Left: {
                            int x = event.mouseButton.x;
                            int y = event.mouseButton.y;
                            game.grabPiece(x, y);

                            // The commented out code below is used to test performance and output the possible board
                            // states to a file for debugging purposes.

//                            std::ofstream out("txt.txt");

//                            for (int i = 5; i <= 5; ++i) {
//                                using std::chrono::high_resolution_clock;
//                                using std::chrono::duration_cast;
//                                using std::chrono::duration;
//                                using std::chrono::milliseconds;
//
//                                auto t1 = high_resolution_clock::now();
//
//                                long a = game.moveMaker(i, window, !turn, out);
//
//                                auto t2 = high_resolution_clock::now();
//                                auto ms_int = duration_cast<milliseconds>(t2 - t1);
//
//                                std::cout << "Depth: " << i << " Result: " << a << " positions  Time: " << ms_int.count() << "ms\n";
//                            }
//                            out.close();

                            break;
                        }
                        default:
                            break;
                    }
                    break;
                }

                case sf::Event::MouseButtonReleased: {

                    switch (event.mouseButton.button) {
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

