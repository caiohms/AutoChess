#ifndef AUTOCHESS_CHESSBOARD_H
#define AUTOCHESS_CHESSBOARD_H

#include <SFML/Graphics.hpp>
#include <unordered_set>
#include "ChessPiece.h"
#include <fstream>
#include <string>
#include <iostream>

class ChessBoardState;

#include "ChessBoardState.h"

#define W_PAWN 0b01000001
#define W_KNIGHT 0b01000010
#define W_BISHOP 0b01000100
#define W_ROOK 0b01001000
#define W_QUEEN 0b01010000
#define W_KING 0b01100000

#define B_PAWN 0b10000001
#define B_KNIGHT 0b10000010
#define B_BISHOP 0b10000100
#define B_ROOK 0b10001000
#define B_QUEEN 0b10010000
#define B_KING 0b10100000

#define PAWN_CODE 0b1
#define KNIGHT_CODE 0b10
#define BISHOP_CODE 0b100
#define ROOK_CODE 0b1000
#define QUEEN_CODE 0b10000
#define KING_CODE 0b100000

class ChessBoard {

private:

    bool debugging = false;

    sf::RenderWindow &window;

    sf::Font font;

    bool &turn;

    unsigned short enPassantEnabledSquare = UINT8_MAX;

    std::vector<ChessPiece> bCapturedPieces = {};
    std::vector<ChessPiece> wCapturedPieces = {};

    unsigned int mouseSelectedSquare = 255;

    int mouseXpos = -1;
    int mouseYpos = -1;

    std::unordered_set<unsigned short> possibilities = {};
    std::unordered_set<unsigned short> attackedSquares = {};

//    std::unordered_set<unsigned short> attackedSquaresDraw = {};

    sf::Vector2u boardSize;
    sf::RectangleShape boardOutline;
    sf::RectangleShape lightRect;
    sf::RectangleShape darkRect;
    sf::RectangleShape selectedRect;
    sf::RectangleShape lightTargetedRect;
    sf::RectangleShape darkTargetedRect;
    sf::CircleShape targetCircleShape;

    ChessPiece bPawn = ChessPiece();
    ChessPiece bBishop = ChessPiece();
    ChessPiece bKnight = ChessPiece();
    ChessPiece bRook = ChessPiece();
    ChessPiece bQueen = ChessPiece();
    ChessPiece bKing = ChessPiece();
    ChessPiece wPawn = ChessPiece();
    ChessPiece wBishop = ChessPiece();
    ChessPiece wKnight = ChessPiece();
    ChessPiece wRook = ChessPiece();
    ChessPiece wQueen = ChessPiece();
    ChessPiece wKing = ChessPiece();

    unsigned short wKingSquare = 60;
    unsigned short bKingSquare = 4;

public:
    bool gameTied = false;
    bool checkmate = false;
    bool gameFinished = false;

    bool bCastleKingSide = true;
    bool bCastleQueenSide = true;
    bool wCastleKingSide = true;
    bool wCastleQueenSide = true;

public:

    unsigned short squares[64] = {
            B_ROOK, B_KNIGHT, B_BISHOP, B_QUEEN, B_KING, B_BISHOP, B_KNIGHT, B_ROOK,
            B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN,
            W_ROOK, W_KNIGHT, W_BISHOP, W_QUEEN, W_KING, W_BISHOP, W_KNIGHT, W_ROOK};

    void initTextures();

    ChessBoard(int width, int height, const sf::Font &font, bool &turn, sf::RenderWindow &window);

    void draw();

    void setBoardSize(const sf::Vector2u &size);

    std::unordered_set<unsigned short> grabPiece(unsigned short squareIdx);

    std::unordered_set<unsigned short> grabPiece(unsigned int squareIdx, bool playerTurn);

    void mouseReleasePiece(unsigned int mouseX, unsigned int mouseY);

    void drawPiece(unsigned short pieceCode, float xPos, float yPos, float boardEdge);

    void setMousePos(int mouseX, int mouseY);

    std::unordered_set<unsigned short> possibleMoves(int currentSquare);

    int getSquareUnderMousePos(unsigned int mouseX, unsigned int mouseY);

    bool addTarget(unsigned short originSquare, unsigned short targetSquare, ChessPiece::PieceColor selectedPieceColor,
                   ChessPiece::PieceColor oppositePieceColor);

    bool isChecked(unsigned short square);

    unsigned short makeMove(unsigned short originSquare, unsigned short targetSquare, bool manualMovement);

    void undoMove(ChessBoardState previousBoardState);

    inline static unsigned int getColorCodeFromPieceCode(unsigned short selectedPieceCode);

    inline static ChessPiece::PieceColor getPieceColorFromPieceCode(unsigned short pieceCode);

    unsigned short takePiece(unsigned short origin, unsigned short targetSquare);

    void mouseGrabPiece(unsigned int mouseX, unsigned int mouseY);

    long moveMaker(int depth, bool playerTurn, std::ofstream &ofstream);

    const unsigned short *getSquares() const;

    unsigned short getEnPassantEnabledSquare() const;

    bool isBCastleKingSide() const;

    bool isBCastleQueenSide() const;

    bool isWCastleKingSide() const;

    bool isWCastleQueenSide() const;

    unsigned short getWKingSquare()const;

    unsigned short getBKingSquare()const;

    bool getTurn() const;

    void redrawWindow();

    unsigned short getPieceCode(unsigned short pieceValue);

    void checkGameFinished(bool manualMovement);

    std::string printFen();
};


#endif //AUTOCHESS_CHESSBOARD_H