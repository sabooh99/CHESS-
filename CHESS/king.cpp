#pragma once
#include "base.h"

class King : public Piece {
public:
    King(string color, int r, int c)
        : Piece(color, r, c) {
    }

    bool isValidMove(int toRow, int toCol, Board& b) override {

        if (!Piece::inBounds(toRow, toCol))   
            return false;

        int rowDiff = abs(toRow - getRow());
        int colDiff = abs(toCol - getCol());

        if (rowDiff > 1 || colDiff > 1)
            return false;

        if (rowDiff == 0 && colDiff == 0)
            return false;

        Piece* dest = b.getPiece(toRow, toCol);
        if (dest != nullptr && dest->getColor() == getColor())
            return false;

        for (int r = toRow - 1; r <= toRow + 1; r++) {
            for (int c = toCol - 1; c <= toCol + 1; c++) {
                if (r == toRow && c == toCol) continue;
                if (!Piece::inBounds(r, c)) continue;
                Piece* p = b.getPiece(r, c);
                if (p && p->getName() == "king" && p->getColor() != getColor())
                    return false;
            }
        }

        return true;
    }

    string getName() const override {
        return "king";
    }

    char getSymbol() const override {
        return 'K';
    }
};
