#pragma once
#include <string>
using namespace std;

class Board; 

class Piece {
private:
    string colour;
    int    row, col;
    bool   isCaptured;

public:
    Piece(string c, int r, int column)
        : colour(c), row(r), col(column), isCaptured(false) {}

    
    virtual bool isValidMove(int toRow, int toCol, Board& b) = 0;

    
    virtual char getSymbol() const = 0;

    
    virtual string getName() const = 0;

    
    virtual void onMoved() {}

    
    virtual bool getHasMoved() const { return false; }

    
    void setPosition(int r, int c) { row = r; col = c; }
    void markCaptured()            { isCaptured = true; }

    
    string getColor()    const { return colour; }
    int    getRow()      const { return row; }
    int    getCol()      const { return col; }
    bool   getCaptured() const { return isCaptured; }

    
    static bool inBounds(int r, int c) {
        return (r >= 0 && r <= 7 && c >= 0 && c <= 7);
    }

    virtual ~Piece() {}
};


class Board {
public:
    
    virtual Piece* getPiece(int row, int col) = 0;
    virtual ~Board() {}
};
