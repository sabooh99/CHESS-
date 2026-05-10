#pragma once
#include <iostream>
#include <string>
#include <cmath>
using namespace std;

class Board;

class Piece {
private:
    string colour;
    bool   isCaptured;
    int    row, col;

public:
    Piece(string c, int r, int column)
        : colour(c), row(r), col(column), isCaptured(false) {
    }

    virtual bool isValidMove(int toRow, int toCol, Board& b) = 0;
    virtual char getSymbol()  const = 0;
    virtual string getName()  const = 0;

    void setPosition(int r, int c) {
        row = r; col = c;
    }
    void markCaptured() {
        isCaptured = true;
    }

    // virtual hook — pieces override if they need to track movement
    virtual void onMoved() {}

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
private:
    Piece* grid[8][8];
    int    halfMoveClock;
    Board(const Board&);
    Board& operator=(const Board&);

public:

    Board() {
        for (int r = 0; r < 8; r++)
            for (int c = 0; c < 8; c++)
                grid[r][c] = nullptr;
        halfMoveClock = 0;
    }

    ~Board() {
        for (int r = 0; r < 8; r++)
            for (int c = 0; c < 8; c++)
                delete grid[r][c];
    }

    Piece* getPiece(int r, int c) const {
        return grid[r][c];
    }

    // delete existing piece before overwriting to avoid memory leak
    void setPiece(int r, int c, Piece* p) {
        if (grid[r][c] != nullptr)
            delete grid[r][c];
        grid[r][c] = p;
    }

    void resetBoard() {
        for (int r = 0; r < 8; r++)
            for (int c = 0; c < 8; c++) {
                delete grid[r][c];
                grid[r][c] = nullptr;
            }
        halfMoveClock = 0;
    }

    void movePieceTo(int fromRow, int fromCol, int toRow, int toCol) {
        if (fromRow == toRow && fromCol == toCol) return;
        if (grid[fromRow][fromCol] == nullptr)    return;

        Piece* moving = grid[fromRow][fromCol];

        bool isCapture = (grid[toRow][toCol] != nullptr);
        bool isPawn = (moving->getName() == "pawn");

        if (isCapture || isPawn)
            halfMoveClock = 0;
        else
            halfMoveClock++;

        if (grid[toRow][toCol] != nullptr) {
            grid[toRow][toCol]->markCaptured();
            delete grid[toRow][toCol];
        }

        grid[toRow][toCol] = moving;
        grid[fromRow][fromCol] = nullptr;
        grid[toRow][toCol]->setPosition(toRow, toCol);

        // notify the piece it moved — overridden by Pawn/Rook to update internal state
        grid[toRow][toCol]->onMoved();
    }

    int  getHalfMoveClock()       const { return halfMoveClock; }
    void resetHalfMoveClock() { halfMoveClock = 0; }
    void incrementHalfMoveClock() { halfMoveClock++; }

    bool isFiftyMoveRule()        const { return halfMoveClock >= 100; }
    bool isSeventyFiveMoveRule()  const { return halfMoveClock >= 150; }

    /// caller must save grid[toRow][toCol] before calling — this does not delete it
    void simulateMove(int fromRow, int fromCol, int toRow, int toCol) {
        grid[toRow][toCol] = grid[fromRow][fromCol];
        grid[fromRow][fromCol] = nullptr;
        grid[toRow][toCol]->setPosition(toRow, toCol);
    }

    void undoMove(int fromRow, int fromCol, int toRow, int toCol, Piece* captured) {
        grid[fromRow][fromCol] = grid[toRow][toCol];
        grid[fromRow][fromCol]->setPosition(fromRow, fromCol);
        grid[toRow][toCol] = captured;
    }

    /// non-const: isValidMove requires Board& so const_cast would be needed otherwise
    bool isInCheck(const string& color) {
        int kingRow = -1, kingCol = -1;
        for (int r = 0; r < 8 && kingRow == -1; r++)
            for (int c = 0; c < 8 && kingRow == -1; c++) {
                Piece* p = grid[r][c];
                if (p && p->getColor() == color && p->getName() == "king") {
                    kingRow = r; kingCol = c;
                }
            }
        if (kingRow == -1) return false;

        for (int r = 0; r < 8; r++)
            for (int c = 0; c < 8; c++) {
                Piece* p = grid[r][c];
                if (p && p->getColor() != color)
                    if (p->isValidMove(kingRow, kingCol, *this))
                        return true;
            }
        return false;
    }

    bool isCheckmate(const string& color) {
        return isInCheck(color) && !hasLegalMove(color);
    }

    bool isStalemate(const string& color) {
        return !isInCheck(color) && !hasLegalMove(color);
    }

private:

    bool hasLegalMove(const string& color) {
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                Piece* p = grid[r][c];
                if (!p || p->getColor() != color) continue;

                for (int tr = 0; tr < 8; tr++) {
                    for (int tc = 0; tc < 8; tc++) {
                        if (!p->isValidMove(tr, tc, *this)) continue;

                        Piece* captured = grid[tr][tc];
                        simulateMove(r, c, tr, tc);
                        bool stillInCheck = isInCheck(color);
                        undoMove(r, c, tr, tc, captured);

                        if (!stillInCheck) return true;
                    }
                }
            }
        }
        return false;
    }
};