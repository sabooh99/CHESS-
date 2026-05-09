#pragma once
#include "base.h"

class King : public Piece {
public:
    King(string color, int r, int c)
        : Piece(color, r, c) {
    }

    bool isValidMove(int toRow, int toCol, Board& b) override {

        int rowDiff = toRow - getRow();

        if (rowDiff < 0)
            rowDiff = -rowDiff;

        int colDiff = toCol - getCol();

        if (colDiff < 0)
            colDiff = -colDiff;

        if (rowDiff > 1 || colDiff > 1)
            return false;

        if (rowDiff == 0 && colDiff == 0)
            return false;

        Piece* dest = b.getPiece(toRow, toCol);

        if (dest != nullptr && dest->getColor() == getColor())
            return false;
        
        int oldrow=getrow();
        int oldcol=getcol();
        b.simulateMove(oldrow, oldcol, toRow, toCol);

        bool inCheck = b.isInCheck(getColor());

        b.undoMove(oldrow, oldcol, toRow, toCol, dest);

        return !inCheck;
    }

    string getName() const override {
        return "king";
    }

    char getSymbol() const override {
        return 'K';
    }
};
