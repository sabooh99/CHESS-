#pragma once
#include "base.h"

class Bishop : public Piece {
public:
    Bishop(string color, int r, int c)
        : Piece(color, r, c) {
    }

    bool isValidMove(int toRow, int toCol, Board& b) override {

        int rowDiff = toRow - getRow();
        int colDiff = toCol - getCol();

        if (rowDiff != colDiff && rowDiff != -colDiff)
            return false;

        int rowStep = (rowDiff > 0) ? 1 : -1;
        int colStep = (colDiff > 0) ? 1 : -1;

        int r = getRow() + rowStep;
        int c = getCol() + colStep;

        while (r != toRow && c != toCol) {

            Piece* p = b.getPiece(r, c);

            if (p != nullptr)
                return false;

            r += rowStep;
            c += colStep;
        }

        Piece* dest = b.getPiece(toRow, toCol);

        if (dest != nullptr && dest->getColor() == getColor())
            return false;

        return true;
    }

    string getName() const override {
        return "bishop";
    }

    char getSymbol() const override {
        return 'B';
    }
};