#pragma once
#include "base.h"

class Rook : public Piece {
private:
    bool hasMoved;

public:
    Rook(string colour, int row, int col);

    char   getSymbol()    const override;
    string getName()      const override;

    
    void onMoved()              override;
    bool getHasMoved()    const override;

    
    bool isValidMove(int toRow, int toCol, Board& b) override;
};
