#include "king.h"

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
King::King(string color, int r, int c)
    : Piece(color, r, c), hasMoved(false) {
}

// ---------------------------------------------------------
// Move tracking
// ---------------------------------------------------------
void King::onMoved() {
    hasMoved = true;
}

bool King::getHasMoved() const {
    return hasMoved;
}

// ---------------------------------------------------------
// Identity
// ---------------------------------------------------------
string King::getName() const {
    return "king";
}

char King::getSymbol() const {
    return 'K';
}
// ---------------------------------------------------------
// CASTLING VALIDATION
// ---------------------------------------------------------
bool King::canCastle(int toCol, Board& b) {

    if (hasMoved)
        return false;

    int row = getRow();
    int fromCol = getCol();

    string enemy = (getColor() == "white") ? "black" : "white";

    // Cannot castle while in check
    if (b.isSquareAttackedBy(row, fromCol, enemy))
        return false;

    bool kingSide = (toCol > fromCol);
    int  rookCol = kingSide ? 7 : 0;
    int  direction = kingSide ? 1 : -1;

    // Rook must exist, be a rook, same colour, and not yet moved
    Piece* rook = b.getPiece(row, rookCol);
    if (!rook)                            return false;
    if (rook->getName() != "rook")       return false;
    if (rook->getColor() != getColor())   return false;
    if (rook->getHasMoved())              return false;

    // All squares between king and rook must be empty
    for (int col = fromCol + direction; col != rookCol; col += direction)
        if (b.getPiece(row, col))
            return false;

    // The two squares the king travels through must not be attacked.
    // step=1 is the transit square, step=2 is the destination — both checked.
    for (int step = 1; step <= 2; step++) {
        int square = fromCol + direction * step;
        if (b.isSquareAttackedBy(row, square, enemy))
            return false;
    }

    return true;
}

// ---------------------------------------------------------
// MOVE VALIDATION
// ---------------------------------------------------------
bool King::isValidMove(int toRow, int toCol, Board& b) {

    if (!inBounds(toRow, toCol))
        return false;

    int rowDiff = abs(toRow - getRow());
    int colDiff = abs(toCol - getCol());

    // -------------------------------------------------
    // NORMAL KING MOVE (one square in any direction)
    // -------------------------------------------------
    if (rowDiff <= 1 && colDiff <= 1) {

        if (rowDiff == 0 && colDiff == 0)
            return false;

        Piece* dest = b.getPiece(toRow, toCol);

        // Cannot capture own piece
        if (dest && dest->getColor() == getColor())
            return false;

        // Cannot move adjacent to enemy king
        for (int r = toRow - 1; r <= toRow + 1; r++) {
            for (int c = toCol - 1; c <= toCol + 1; c++) {

                if (r == toRow && c == toCol) continue;
                if (!Piece::inBounds(r, c))   continue;

                Piece* p = b.getPiece(r, c);
                if (p && p->getName() == "king" && p->getColor() != getColor())
                    return false;
            }
        }

        
        int oldRow=getRow();
        int oldCol=getCol();
        b.simulateMove(oldRow,oldCol,toRow, toCol);
        bool inCheck = b.isInCheck(getColor());
        b.undoMove( oldRow,oldCol,toRow, toCol, dest);

        return !inCheck;
    }

    // -------------------------------------------------
    // CASTLING (king moves exactly 2 squares sideways)
    // -------------------------------------------------
    if (rowDiff == 0 && colDiff == 2)
        return canCastle(toCol, b);

    return false;
}
