
#include <iterator>
#include "ChessBoardState.h"

ChessBoardState::ChessBoardState(bool checkmate, bool gameTied, bool gameFinished, bool bCastleKingSide,
                                 bool bCastleQueenSide,
                                 bool wCastleKingSide, bool wCastleQueenSide, unsigned short wKingSquare,
                                 unsigned short bKingSquare,
                                 unsigned short enPassantEnabledSquare, bool playerTurn, unsigned short *squares)
        : checkmate(checkmate),
          gameTied(gameTied),
          gameFinished(gameFinished),
          bCastleKingSide(bCastleKingSide),
          bCastleQueenSide(bCastleQueenSide),
          wCastleKingSide(wCastleKingSide),
          wCastleQueenSide(wCastleQueenSide),
          wKingSquare(wKingSquare),
          bKingSquare(bKingSquare),
          enPassantEnabledSquare(enPassantEnabledSquare),
          playerTurn(playerTurn) {
    copyArray(squares, this->squares);
}

ChessBoardState ChessBoardState::fromChessBoard(ChessBoard *pBoard) {

    unsigned short stateSquares[64];
    copyArray(pBoard->getSquares(), stateSquares);

    return {pBoard->checkmate,
            pBoard->gameTied,
            pBoard->gameFinished,
            pBoard->isBCastleKingSide(),
            pBoard->isBCastleQueenSide(),
            pBoard->isWCastleKingSide(),
            pBoard->isWCastleQueenSide(),
            pBoard->getWKingSquare(),
            pBoard->getBKingSquare(),
            pBoard->getEnPassantEnabledSquare(),
            pBoard->getTurn(),
            stateSquares};
}

void ChessBoardState::copyArray(const unsigned short *pSrc, unsigned short *pDest) {
    std::copy_n(pSrc, 64, pDest);
}
