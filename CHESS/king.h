#pragma once
#pragma once
#include "base.h"

class King : public Piece {
private:
    bool hasMoved;

    bool canCastle(int toCol, Board& b);

public:
    King(string color, int r, int c);

    void   onMoved()          override;
    bool   getHasMoved() const override;
    string getName()     const override;
    char   getSymbol()   const override;

    bool isValidMove(int toRow, int toCol, Board& b) override;
};