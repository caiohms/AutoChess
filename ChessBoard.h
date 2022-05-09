#ifndef AUTOCHESS_CHESSBOARD_H
#define AUTOCHESS_CHESSBOARD_H

#include <SFML/Graphics.hpp>
#include <unordered_set>
#include "ChessPiece.h"

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

    bool & turn;

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

    //POSITION 3
//    unsigned short squares[64] = {
//            0, 0, 0, 0, 0, 0, 0, 0,
//            0, 0, B_PAWN, 0, 0, 0, 0, 0,
//            0, 0, 0, B_PAWN, 0, 0, 0, 0,
//            W_KING, W_PAWN, 0, 0, 0, 0, 0, B_ROOK,
//            0, W_ROOK, 0, 0, 0, B_PAWN, 0, B_KING,
//            0, 0, 0, 0, 0, 0, 0, 0,
//            0, 0, 0, 0, W_PAWN, 0, W_PAWN, 0,
//            0, 0, 0, 0, 0, 0, 0, 0};

//POSITION 5
//    unsigned short wKingSquare = 60;
//    unsigned short bKingSquare = 5;
//
//    bool bCastleKingSide = false;
//    bool bCastleQueenSide = false;
//    bool wCastleKingSide = true;
//    bool wCastleQueenSide = true;
//    unsigned short squares[64] = {
//            B_ROOK, B_KNIGHT, B_BISHOP, B_QUEEN, 0, B_KING, 0, B_ROOK,
//            B_PAWN, B_PAWN, 0, W_PAWN, B_BISHOP, B_PAWN, B_PAWN, B_PAWN,
//            0, 0, B_PAWN, 0, 0, 0, 0, 0,
//            0, 0, 0, 0, 0, 0, 0, 0,
//            0, 0, W_BISHOP, 0, 0, 0, 0, 0,
//            0, 0, 0, 0, 0, 0, 0, 0,
//            W_PAWN, W_PAWN, W_PAWN, 0, W_KNIGHT, B_KNIGHT, W_PAWN, W_PAWN,
//            W_ROOK, W_KNIGHT, W_BISHOP, W_QUEEN, W_KING, 0, 0, W_ROOK};

//    unsigned short squares[64] = {
//            B_ROOK, 0, 0, 0, B_KING, 0, 0, B_ROOK,
//            0, 0, 0, 0, 0, 0, 0, 0,
//            0, 0, 0, 0, 0, 0, 0, 0,
//            0, 0, 0, 0, 0, 0, 0, 0,
//            0, 0, 0, 0, 0, 0, 0, 0,
//            0, 0, 0, 0, 0, 0, 0, 0,
//            0, 0, 0, 0, 0, 0, 0, 0,
//            W_ROOK, 0, 0, 0, W_KING, 0, 0, W_ROOK};

// STANDARD
    unsigned short wKingSquare = 60;
    unsigned short bKingSquare = 4;

    bool bCastleKingSide = true;
    bool bCastleQueenSide = true;
    bool wCastleKingSide = true;
    bool wCastleQueenSide = true;
    unsigned short squares[64] = {
            B_ROOK, B_KNIGHT, B_BISHOP, B_QUEEN, B_KING, B_BISHOP, B_KNIGHT, B_ROOK,
            B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN,
            W_ROOK, W_KNIGHT, W_BISHOP, W_QUEEN, W_KING, W_BISHOP, W_KNIGHT, W_ROOK};

public:

    void initTextures();

    ChessBoard(int width, int height, const sf::Font &font, bool &turn, sf::RenderWindow &window);

    void draw();

    void setBoardSize(const sf::Vector2u &size);

    std::unordered_set<unsigned short> grabPiece(unsigned short squareIdx);

    std::unordered_set<unsigned short> grabPiece(unsigned int squareIdx, bool playerTurn);

    void mouseReleasePiece(unsigned int mouseX, unsigned int mouseY);

    void drawPiece(unsigned short pieceCode, float xPos, float yPos, float boardEdge);

    void setMousePos(int mouseX, int mouseY);

    std::unordered_set<unsigned short>
    possibleMoves(int currentSquare, bool checkingCheck, std::unordered_set<unsigned short> &targetSet);

    int getSquareUnderMousePos(unsigned int mouseX, unsigned int mouseY);

    bool addTarget(unsigned short originSquare, unsigned short targetSquare, ChessPiece::PieceColor selectedPieceColor,
                   ChessPiece::PieceColor oppositePieceColor, bool checkingCheck,
                   std::unordered_set<unsigned short> &set);

    bool isChecked(unsigned int kingSquare);

    unsigned short makeMove(unsigned short originSquare, unsigned short targetSquare);

    void undoMove(ChessBoardState previousBoardState);

    static unsigned int getColorFromPieceCode(unsigned short selectedPieceCode);

    std::unordered_set<unsigned short> getSquaresAttackedByOpponent();

    unsigned short takePiece(unsigned short origin, unsigned short targetSquare);

    void mouseGrabPiece(unsigned int mouseX, unsigned int mouseY);

    long moveMaker(int depth, bool playerTurn);

    const unsigned short *getSquares() const;

    unsigned short getEnPassantEnabledSquare() const;

    bool isBCastleKingSide() const;

    bool isBCastleQueenSide() const;

    bool isWCastleKingSide() const;

    bool isWCastleQueenSide() const;

    unsigned short getWKingSquare()const;

    unsigned short getBKingSquare()const;

    bool getTurn() const;
};


#endif //AUTOCHESS_CHESSBOARD_H