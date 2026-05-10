#include "bishop.h"

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
Bishop::Bishop(string color, int r, int c)
    : Piece(color, r, c) {
}

// ---------------------------------------------------------
// Identity
// ---------------------------------------------------------
string Bishop::getName() const {
    return "bishop";
}

char Bishop::getSymbol() const {
    return 'B';
}

// ---------------------------------------------------------
// MOVE VALIDATION
// Bishop moves diagonally any distance.
// Path must be clear; destination may be empty or enemy.
// ---------------------------------------------------------
bool Bishop::isValidMove(int toRow, int toCol, Board& b) {

    if (!Piece::inBounds(toRow, toCol))
        return false;

    if (toRow == getRow() && toCol == getCol())
        return false;

    int rowDiff = toRow - getRow();
    int colDiff = toCol - getCol();

    // Must move diagonally
    if (abs(rowDiff) != abs(colDiff))
        return false;

    int rowStep = (rowDiff > 0) ? 1 : -1;
    int colStep = (colDiff > 0) ? 1 : -1;

    int r = getRow() + rowStep;
    int c = getCol() + colStep;

    // Check all squares between origin and destination (exclusive)
    while (r != toRow || c != toCol) {
        if (b.getPiece(r, c) != nullptr)
            return false;
        r += rowStep;
        c += colStep;
    }

    // Destination must be empty or occupied by an enemy piece
    Piece* dest = b.getPiece(toRow, toCol);
    return (dest == nullptr || dest->getColor() != getColor());
}
