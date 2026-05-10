#pragma once
#include "base.h"

class Pawn : public Piece {
private:
    bool hasMoved;

public:
    Pawn(string colour, int row, int col);

    char   getSymbol()    const override;
    string getName()      const override;

    
    void onMoved()              override;
    bool getHasMoved()    const override;

    bool isValidMove(int toRow, int toCol, Board& b) override;
};
