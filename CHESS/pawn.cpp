#include "pawn.h"
#include <cmath>
using namespace std;


Pawn::Pawn(string colour, int row, int col)
    : Piece(colour, row, col), hasMoved(false) {}


char Pawn::getSymbol() const {
    return (getColor() == "white") ? 'P' : 'p';
}

string Pawn::getName() const {
    return "pawn";
}
void Pawn::onMoved() {
    hasMoved = true;
}

bool Pawn::getHasMoved() const {
    return hasMoved;
}


bool Pawn::isValidMove(int toRow, int toCol, Board& b) {

    if (!inBounds(toRow, toCol))
        return false;

    int fromRow = getRow();
    int fromCol = getCol();

  
    int direction = (getColor() == "white") ? -1 : 1;

    int rowDiff = toRow - fromRow;
    int colDiff = toCol - fromCol;

    
    if (colDiff == 0 && rowDiff == direction) {
        return (b.getPiece(toRow, toCol) == nullptr);
    }

    
    bool startingRow = (getColor() == "white" && fromRow == 6) ||
                       (getColor() == "black" && fromRow == 1);

    if (colDiff == 0 && rowDiff == 2 * direction && !hasMoved && startingRow) {
        int midRow = fromRow + direction;
        return (b.getPiece(midRow, fromCol) == nullptr &&
                b.getPiece(toRow,  toCol)   == nullptr);
    }

    
    if (rowDiff == direction && (colDiff == 1 || colDiff == -1)) {
        Piece* target = b.getPiece(toRow, toCol);
        return (target != nullptr && target->getColor() != getColor());
    }

    return false; 
}
