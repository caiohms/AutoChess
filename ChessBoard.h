#ifndef AUTOCHESS_CHESSBOARD_H
#define AUTOCHESS_CHESSBOARD_H

#include <SFML/Graphics.hpp>
#include <unordered_set>
#include "ChessPiece.h"

enum PlayerTurn {
    WHITE = true, BLACK = false
};

class ChessBoard {

private:
    sf::Font font;

    PlayerTurn &turn;

    unsigned int enPassantEnabledSquare = 0b11111111;

    unsigned short wKingSquare = 60;
    unsigned short bKingSquare = 3;

    bool bCastleKingSide = true;
    bool bCastleQueenSide = true;
    bool wCastleKingSide = true;
    bool wCastleQueenSide = true;

    std::vector<ChessPiece> bCapturedPieces = {};
    std::vector<ChessPiece> wCapturedPieces = {};

    int selectedSquareIndex = -1;
    unsigned short selectedPieceCode = -1;

    int mouseXpos = -1;
    int mouseYpos = -1;

    std::unordered_set<unsigned short> possibilities = {};
    std::unordered_set<unsigned short> attackedSquares = {};

    std::unordered_set<unsigned short> attackedSquaresDraw = {};

    sf::Vector2u boardSize;
    sf::RectangleShape boardOutline;
    sf::RectangleShape lightRect;
    sf::RectangleShape darkRect;
    sf::RectangleShape selectedRect;
    sf::RectangleShape lightTargetedRect;
    sf::RectangleShape darkTargetedRect;
    sf::CircleShape targetCircleShape;

    ChessPiece bPawn = ChessPiece(sf::Sprite());
    ChessPiece bBishop = ChessPiece(sf::Sprite());
    ChessPiece bKnight = ChessPiece(sf::Sprite());
    ChessPiece bRook = ChessPiece(sf::Sprite());
    ChessPiece bQueen = ChessPiece(sf::Sprite());
    ChessPiece bKing = ChessPiece(sf::Sprite());
    ChessPiece wPawn = ChessPiece(sf::Sprite());
    ChessPiece wBishop = ChessPiece(sf::Sprite());
    ChessPiece wKnight = ChessPiece(sf::Sprite());
    ChessPiece wRook = ChessPiece(sf::Sprite());
    ChessPiece wQueen = ChessPiece(sf::Sprite());
    ChessPiece wKing = ChessPiece(sf::Sprite());


//    unsigned short squares[64] = {
//            0b10001000, 0b10000010, 0b10000100, 0b10010000, 0b10100000, 0b10000100, 0b10000010, 0b10001000,
//            0b10000001, 0b10000001, 0b10000001, 0b10000001, 0b10000001, 0b10000001, 0b10000001, 0b10000001,
//            0, 0, 0, 0, 0, 0, 0, 0,
//            0, 0, 0, 0, 0, 0, 0, 0,
//            0, 0, 0, 0, 0b10001000, 0, 0, 0,
//            0, 0, 0, 0, 0, 0, 0, 0b01000001,
//            0, 0, 0, 0, 0, 0, 0, 0,
//            0b01001000, 0, 0, 0, 0b01100000, 0b01000100, 0b01000010, 0b01001000};

    unsigned short squares[64] = {
            0b10001000, 0b10000010, 0b10000100, 0b10010000, 0b10100000, 0b10000100, 0b10000010, 0b10001000,
            0b10000001, 0b10000001, 0b10000001, 0b10000001, 0b10000001, 0b10000001, 0b10000001, 0b10000001,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0b01000001, 0b01000001, 0b01000001, 0b01000001, 0b01000001, 0b01000001, 0b01000001, 0b01000001,
            0b01001000, 0b01000010, 0b01000100, 0b01010000, 0b01100000, 0b01000100, 0b01000010, 0b01001000};

public:

    void initTextures();

    ChessBoard(int width, int height, const sf::Font &font, PlayerTurn &turn);

    void draw(sf::RenderWindow &window);

    void setBoardSize(const sf::Vector2u &size);

    void grabPiece(unsigned int mouseX, unsigned int mouseY);

    void releasePiece(unsigned int mouseX, unsigned int mouseY);

    void drawPiece(unsigned short pieceCode, float xPos, float yPos, float boardEdge, sf::RenderWindow &window);

    void setMousePos(int mouseX, int mouseY);

    void possibleMoves(int currentSquare, bool checkingCheck, std::unordered_set<unsigned short> &targetSet);

    int getSquareUnderMousePos(unsigned int mouseX, unsigned int mouseY);

    bool addTarget(unsigned short originSquare, unsigned short targetSquare, ChessPiece::PieceColor selectedPieceColor,
                   ChessPiece::PieceColor oppositePieceColor, bool checkingCheck,
                   std::unordered_set<unsigned short> &set);

    bool isChecked(unsigned int kingSquare);

    const unsigned short *getSquares() const;

    unsigned short makeMove(unsigned short origin, unsigned short targetSquare);

    void undoMove(unsigned short originSquare, unsigned short targetSquare, unsigned short originalPieceCode,
                  bool wCastleKingSide, bool wCastleQueenSide, bool bCastleKingSide, bool bCastleQueenSide);

    static unsigned int getColorFromPieceCode(unsigned short selectedPieceCode);

    std::unordered_set<unsigned short> getSquaresAttackedByOpponent();
};


#endif //AUTOCHESS_CHESSBOARD_H