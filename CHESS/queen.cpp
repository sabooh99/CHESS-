#pragma once
#include "base.h"

class Queen : public Piece {
public:
    Queen(string color, int r, int c)
        : Piece(color, r, c) {
    }

    bool isValidMove(int toRow, int toCol, Board& b) override {

        int rowDiff = toRow - getRow();
        int colDiff = toCol - getCol();

        bool isDiagonal = (rowDiff == colDiff || rowDiff == -colDiff);
        bool isStraight = (rowDiff == 0 || colDiff == 0);

        if (!isDiagonal && !isStraight)
            return false;

        int rowStep = (rowDiff == 0) ? 0 : (rowDiff > 0 ? 1 : -1);
        int colStep = (colDiff == 0) ? 0 : (colDiff > 0 ? 1 : -1);

        int r = getRow() + rowStep;
        int c = getCol() + colStep;

        while (r != toRow || c != toCol) {

            if (b.getPiece(r, c) != nullptr)
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
        return "queen";
    }

    char getSymbol() const override {
        return 'Q';
    }
};