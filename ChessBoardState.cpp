
#include <iterator>
#include "ChessBoardState.h"

ChessBoardState::ChessBoardState(bool bCastleKingSide, bool bCastleQueenSide, bool wCastleKingSide,
                                 bool wCastleQueenSide, unsigned short wKingSquare, unsigned short bKingSquare,
                                 unsigned short enPassantEnabledSquare, bool playerTurn, unsigned short *squares)
        : bCastleKingSide(bCastleKingSide),
          bCastleQueenSide(bCastleQueenSide),
          wCastleKingSide(wCastleKingSide),
          wCastleQueenSide(wCastleQueenSide),
          wKingSquare(wKingSquare),
          bKingSquare(bKingSquare),
          enPassantEnabledSquare(enPassantEnabledSquare),
          playerTurn(playerTurn) {
    copyArray(squares, this->squares);
}
//
//ChessBoardState::ChessBoardState(const ChessBoard &board)
//        : bCastleKingSide(board.isBCastleKingSide()),
//          bCastleQueenSide(board.isBCastleQueenSide()),
//          wCastleKingSide(board.isWCastleKingSide()),
//          wCastleQueenSide(board.isWCastleQueenSide()),
//          bKingSquare(board.getBKingSquare()),
//          wKingSquare(board.getWKingSquare()),
//          enPassantEnabledSquare(board.getEnPassantEnabledSquare()),
//          playerTurn(board.getTurn()) {
//    copyArray(board.getSquares(), this->squares);
//}

ChessBoardState ChessBoardState::fromChessBoard(ChessBoard *pBoard) {

    unsigned short stateSquares[64];
    copyArray(pBoard->getSquares(), stateSquares);

    return {pBoard->isBCastleKingSide(), pBoard->isBCastleQueenSide(), pBoard->isWCastleKingSide(),
            pBoard->isWCastleQueenSide(), pBoard->getWKingSquare(), pBoard->getBKingSquare(),
            pBoard->getEnPassantEnabledSquare(), pBoard->getTurn(), stateSquares};
}

void ChessBoardState::copyArray(const unsigned short *pSrc, unsigned short *pDest) {
    std::copy_n(pSrc, 64, pDest);
}
