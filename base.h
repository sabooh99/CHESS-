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

    virtual void onMoved() {}

    // ======================================================
    // 🔥 ADDED FOR CASTLING SUPPORT (REQUIRED BY KING/ROOK)
    // ======================================================
    virtual bool getHasMoved() const { return false; }

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

        grid[toRow][toCol]->onMoved();
    }

    int  getHalfMoveClock()       const { return halfMoveClock; }
    void resetHalfMoveClock() { halfMoveClock = 0; }
    void incrementHalfMoveClock() { halfMoveClock++; }

    bool isFiftyMoveRule()        const { return halfMoveClock >= 100; }
    bool isSeventyFiveMoveRule()  const { return halfMoveClock >= 150; }

    bool isSquareAttackedBy(int row, int col, const string& enemyColor) {
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                Piece* p = grid[r][c];
                if (p && p->getColor() == enemyColor) {
                    if (p->isValidMove(row, col, *this))
                        return true;
                }
            }
        }
        return false;
    }

    // ======================================================
    // ---------------------- CASTLING ----------------------
    // ======================================================
    void performCastle(int kingRow, int kingCol,
        int kingToCol,
        int rookCol, int rookToCol) {

        Piece* king = grid[kingRow][kingCol];
        Piece* rook = grid[kingRow][rookCol];

        if (!king || !rook) return;

        // move king
        grid[kingRow][kingToCol] = king;
        grid[kingRow][kingCol] = nullptr;
        king->setPosition(kingRow, kingToCol);
        king->onMoved();

        // move rook
        grid[kingRow][rookToCol] = rook;
        grid[kingRow][rookCol] = nullptr;
        rook->setPosition(kingRow, rookToCol);
        rook->onMoved();
    }

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

                        if (!p->isValidMove(tr, tc, *this))
                            continue;

                        Piece* captured = grid[tr][tc];
                        simulateMove(r, c, tr, tc);
                        bool stillInCheck = isInCheck(color);
                        undoMove(r, c, tr, tc, captured);

                        if (!stillInCheck)
                            return true;
                    }
                }
            }
        }
        return false;
    }
};
