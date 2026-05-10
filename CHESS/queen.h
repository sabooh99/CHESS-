#pragma once
#pragma once
#include "base.h"

class Queen : public Piece {
public:
    Queen(string color, int r, int c);

    // Note: check-exposure is NOT validated here.
    // The game loop must wrap non-king isValidMove calls
    // in simulate/isInCheck/undo before committing the move.
    bool isValidMove(int toRow, int toCol, Board& b) override;

    string getName()   const override;
    char   getSymbol() const override;
};