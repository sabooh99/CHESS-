#include "rook.h"
using namespace std;
Rook::Rook(string colour, int row, int col)
    : Piece(colour, row, col), hasMoved(false) {}


void Rook::onMoved() {
    hasMoved = true;
}

bool Rook::getHasMoved() const {
    return hasMoved;
}


string Rook::getName() const {
    return "rook";
}

char Rook::getSymbol() const {
    return (getColor() == "white") ? 'R' : 'r';
}


bool Rook::isValidMove(int toRow, int toCol, Board& b) {

    if (!inBounds(toRow, toCol))
        return false;

    int fromRow = getRow();
    int fromCol = getCol();

    
    if (toRow == fromRow && toCol == fromCol)
        return false;

    bool sameRow = (toRow == fromRow);
    bool sameCol = (toCol == fromCol);

    
    if (!sameRow && !sameCol)
        return false;

    
    if (sameRow) {
        int step = (toCol > fromCol) ? 1 : -1;
        for (int c = fromCol + step; c != toCol; c += step)
            if (b.getPiece(fromRow, c) != nullptr)
                return false;
    } else {
        int step = (toRow > fromRow) ? 1 : -1;
        for (int r = fromRow + step; r != toRow; r += step)
            if (b.getPiece(r, fromCol) != nullptr)
                return false;
    }

    Piece* dest = b.getPiece(toRow, toCol);
    return (dest == nullptr || dest->getColor() != getColor());
}
