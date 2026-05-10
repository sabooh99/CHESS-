#include "queen.h"

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
Queen::Queen(string color, int r, int c)
    : Piece(color, r, c) {
}

// ---------------------------------------------------------
// Identity
// ---------------------------------------------------------
string Queen::getName() const {
    return "queen";
}

char Queen::getSymbol() const {
    return 'Q';
}

// ---------------------------------------------------------
// MOVE VALIDATION
// Queen moves diagonally or in straight lines any distance.
// Path must be clear; destination may be empty or enemy.
// Check-exposure validation is the caller's responsibility.
// ---------------------------------------------------------
bool Queen::isValidMove(int toRow, int toCol, Board& b) {

    if (!Piece::inBounds(toRow, toCol))
        return false;

    if (toRow == getRow() && toCol == getCol())
        return false;

    int rowDiff = toRow - getRow();
    int colDiff = toCol - getCol();

    bool isDiagonal = (abs(rowDiff) == abs(colDiff));
    bool isStraight = (rowDiff == 0 || colDiff == 0);

    if (!isDiagonal && !isStraight)
        return false;

    int rowStep = (rowDiff == 0) ? 0 : (rowDiff > 0 ? 1 : -1);
    int colStep = (colDiff == 0) ? 0 : (colDiff > 0 ? 1 : -1);

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
