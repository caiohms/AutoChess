
#include <algorithm>
#include <iterator>
#include <iostream>
#include "ChessBoardState.h"

ChessBoardState::ChessBoardState(bool bCastleKingSide, bool bCastleQueenSide, bool wCastleKingSide,
                                 bool wCastleQueenSide, unsigned short enPassantEnabledSquare,
                                 const unsigned short *squares)
        : bCastleKingSide(bCastleKingSide),
          bCastleQueenSide(bCastleQueenSide),
          wCastleKingSide(wCastleKingSide),
          wCastleQueenSide(wCastleQueenSide),
          enPassantEnabledSquare(enPassantEnabledSquare) {

    copyArray(squares, this->squares);
}

void ChessBoardState::copyArray(const unsigned short *pSrc, unsigned short *pDest) {
    std::copy_n(pSrc, 64, pDest);
}
