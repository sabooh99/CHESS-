
#pragma once
#include "base.h"

class Bishop : public Piece {
public:
    Bishop(string color, int r, int c);

    bool isValidMove(int toRow, int toCol, Board& b) override;

    string getName()   const override;
    char   getSymbol() const override;
};