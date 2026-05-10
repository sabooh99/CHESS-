#include "knight.h"
#include <cmath>
using namespace std;


Knight::Knight(string colour, int row, int col)
    : Piece(colour, row, col) {}


char Knight::getSymbol() const {
    return (getColor() == "white") ? 'N' : 'n';
}

string Knight::getName() const {
    return "knight";
}


bool Knight::isValidMove(int toRow, int toCol, Board& b) {

    if (!inBounds(toRow, toCol))
        return false;

    int rowDiff = abs(toRow - getRow());
    int colDiff = abs(toCol - getCol());

    
    bool validShape = (rowDiff == 2 && colDiff == 1) ||
                      (rowDiff == 1 && colDiff == 2);

    if (!validShape)
        return false;

    
    Piece* dest = b.getPiece(toRow, toCol);
    return (dest == nullptr || dest->getColor() != getColor());
}
