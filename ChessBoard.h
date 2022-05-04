#ifndef AUTOCHESS_CHESSBOARD_H
#define AUTOCHESS_CHESSBOARD_H

#include <SFML/Graphics.hpp>
#include <unordered_set>
#include "ChessPiece.h"

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

enum PlayerTurn {
    WHITE = true, BLACK = false
};

class ChessBoard {

private:
    sf::Font font;

    PlayerTurn &turn;

    unsigned int enPassantEnabledSquare = 0b11111111;

    unsigned short wKingSquare = 60;
    unsigned short bKingSquare = 4;

    bool bCastleKingSide = true;
    bool bCastleQueenSide = true;
    bool wCastleKingSide = true;
    bool wCastleQueenSide = true;

    std::vector<ChessPiece> bCapturedPieces = {};
    std::vector<ChessPiece> wCapturedPieces = {};

    int selectedSquareIndex = -1;
    int mouseSelectedSquare = -1;
    unsigned short selectedPieceCode = 0b11111111;

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
//            0, 0, 0, 0, 0, 0, 0, 0,
//            0, 0, B_PAWN, 0, 0, 0, 0, 0,
//            0, 0, 0, B_PAWN, 0, 0, 0, 0,
//            W_KING, W_PAWN, 0, 0, 0, 0, 0, B_ROOK,
//            0, 0, 0, 0, 0, 0, 0, B_KING,
//            0, 0, 0, 0, 0, 0, 0, 0,
//            0, 0, W_PAWN, 0, 0, 0, 0, 0,
//            0, 0, 0, 0, 0, 0, 0, 0};


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

    ChessBoard(int width, int height, const sf::Font &font, PlayerTurn &turn);

    void draw(sf::RenderWindow &window);

    void setBoardSize(const sf::Vector2u &size);

    std::unordered_set<unsigned short> grabPiece(int i);

    void releasePiece(unsigned int mouseX, unsigned int mouseY);

    void drawPiece(unsigned short pieceCode, float xPos, float yPos, float boardEdge, sf::RenderWindow &window);

    void setMousePos(int mouseX, int mouseY);

    std::unordered_set<unsigned short> possibleMoves(int currentSquare, bool checkingCheck, std::unordered_set<unsigned short> &targetSet);

    int getSquareUnderMousePos(unsigned int mouseX, unsigned int mouseY);

    bool addTarget(unsigned short originSquare, unsigned short targetSquare, ChessPiece::PieceColor selectedPieceColor,
                   ChessPiece::PieceColor oppositePieceColor, bool checkingCheck,
                   std::unordered_set<unsigned short> &set);

    bool isChecked(unsigned int kingSquare);

    const unsigned short *getSquares() const;

    unsigned short makeMove(unsigned short origin, unsigned short targetSquare);

    void undoMove(unsigned short originSquare, unsigned short targetSquare, unsigned short originalPieceCode,
                  bool wCastleKingSideOld, bool wCastleQueenSideOld, bool bCastleKingSideOld, bool bCastleQueenSideOld,
                  unsigned short enPassantEnabledSquareOld, int selectedSquareIndexOld, bool &leCrossaint);

    static unsigned int getColorFromPieceCode(unsigned short selectedPieceCode);

    std::unordered_set<unsigned short> getSquaresAttackedByOpponent();

    unsigned short takePiece(unsigned short origin, unsigned short targetSquare);

    void mouseGrabPiece(unsigned int mouseX, unsigned int mouseY);

    long moveMaker(int depth, sf::RenderWindow &window);
};


#endif //AUTOCHESS_CHESSBOARD_H