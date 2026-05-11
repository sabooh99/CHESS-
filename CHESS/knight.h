#pragma once
#include "base.h"

class Knight : public Piece {
public:
    Knight(string colour, int row, int col);

    char   getSymbol() const override;
    string getName()   const override;


    bool isValidMove(int toRow, int toCol, Board& b) override;
};
