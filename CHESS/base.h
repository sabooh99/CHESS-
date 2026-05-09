#pragma once
#include <iostream>
#include <string>
#include <cmath>
using namespace std;

// Forward declaration so Piece can reference Board
class Board;

// ---------------------------------------------------------
//  PIECE  –  Abstract base class for all chess pieces
// ---------------------------------------------------------

class Piece {
private:
    string colour;
    bool   isCaptured;
    int    row, col;

public:
    Piece(string c, int r, int column)
        : colour(c), row(r), col(column), isCaptured(false) {
    }

    // ------- pure virtuals (every piece must implement) -------

    virtual bool isValidMove(int toRow, int toCol, Board& b) = 0;
    virtual char getSymbol()  const = 0;
    virtual string getName()  const = 0;

    // ------- setters -------

    void setPosition(int r, int c) {
        row = r; col = c;
    }
    void markCaptured() {
        isCaptured = true;
    }

    // ------- getters -------
    string getColor()    const {
        return colour;
    }
    int    getRow()      const {
        return row;
    }
    int    getCol()      const {
        return col;
    }
    bool   getCaptured() const {
        return isCaptured;
    }

    // ------- utility -------

    static bool inBounds(int r, int c) {
        return (r >= 0 && r <= 7 && c >= 0 && c <= 7);
    }

    virtual ~Piece() {}
};


// -----------------------------------------------------------
//  BOARD  8 x 8
// ------------------------------------------------------------
class Board {
private:
    Piece* grid[8][8];
    int    halfMoveClock;
    Board(const Board&);            // prevent copying
    Board& operator=(const Board&); // prevent assignment

public:

    // ---------- constructor / destructor ----------

    Board() {
        for (int r = 0; r < 8; r++)
            for (int c = 0; c < 8; c++)
                grid[r][c] = nullptr;
        halfMoveClock = 0;
    }

    ~Board() {
        for (int r = 0; r < 8; r++)
            for (int c = 0; c < 8; c++)
                delete grid[r][c];      // delete is safe on nullptr
    }

    // ---------- piece access ----------

    Piece* getPiece(int r, int c) const {
        return grid[r][c];
    }

    // Place a piece directly (used during board setup)
    void setPiece(int r, int c, Piece* p) {
        grid[r][c] = p;
    }

    // ---------- reset (used for new game / load game) ----------

    void resetBoard() {
        for (int r = 0; r < 8; r++)
            for (int c = 0; c < 8; c++) {
                delete grid[r][c];
                grid[r][c] = nullptr;
            }
        halfMoveClock = 0;
    }

    // ---------- real move (updates state permanently) ----------

    void movePieceTo(int fromRow, int fromCol, int toRow, int toCol) {
        if (fromRow == toRow && fromCol == toCol) return;
        if (grid[fromRow][fromCol] == nullptr)    return;

        Piece* moving = grid[fromRow][fromCol];

        // update half-move clock before anything is deleted
        bool isCapture = (grid[toRow][toCol] != nullptr);
        bool isPawn = (moving->getName() == "pawn");

        if (isCapture || isPawn)
            halfMoveClock = 0;
        else
            halfMoveClock++;

        // capture destination if occupied
        if (grid[toRow][toCol] != nullptr) {
            grid[toRow][toCol]->markCaptured();
            delete grid[toRow][toCol];
        }

        grid[toRow][toCol] = moving;
        grid[fromRow][fromCol] = nullptr;
        grid[toRow][toCol]->setPosition(toRow, toCol);
    }

    // ---------- fifty / seventy-five move rule ----------

    int  getHalfMoveClock()      const { return halfMoveClock; }
    void resetHalfMoveClock() { halfMoveClock = 0; }
    void incrementHalfMoveClock() { halfMoveClock++; }

    // 50 full moves = 100 half moves — offer draw
    bool isFiftyMoveRule()       const { return halfMoveClock >= 100; }

    // 75 full moves = 150 half moves — automatic draw
    bool isSeventyFiveMoveRule() const { return halfMoveClock >= 150; }

    // ---------- simulation helpers (for check / checkmate logic) ----------

    // simulateMove does NOT delete captured piece – caller owns it
    void simulateMove(int fromRow, int fromCol, int toRow, int toCol) {
        grid[toRow][toCol] = grid[fromRow][fromCol];
        grid[fromRow][fromCol] = nullptr;
        grid[toRow][toCol]->setPosition(toRow, toCol);
    }

    // Restore board exactly as it was before simulateMove
    void undoMove(int fromRow, int fromCol, int toRow, int toCol, Piece* captured) {
        grid[fromRow][fromCol] = grid[toRow][toCol];
        grid[fromRow][fromCol]->setPosition(fromRow, fromCol);
        grid[toRow][toCol] = captured;          // may be nullptr, its ok
    }

    // ---------- check detection ----------

    bool isInCheck(const string& color) const {

        // 1. find the king
        int kingRow = -1, kingCol = -1;
        for (int r = 0; r < 8 && kingRow == -1; r++)
            for (int c = 0; c < 8 && kingRow == -1; c++) {
                Piece* p = grid[r][c];
                if (p && p->getColor() == color && p->getName() == "king") {
                    kingRow = r; kingCol = c;
                }
            }
        if (kingRow == -1) return false;    // king not on board (shouldn't happen)

        // 2. see if any enemy piece can reach the king
        for (int r = 0; r < 8; r++)
            for (int c = 0; c < 8; c++) {
                Piece* p = grid[r][c];
                if (p && p->getColor() != color)
                    if (p->isValidMove(kingRow, kingCol, *const_cast<Board*>(this)))
                        return true;
            }
        return false;
    }

    // ---------- checkmate / stalemate ----------

    bool isCheckmate(const string& color) {
        return isInCheck(color) && !hasLegalMove(color);
    }

    bool isStalemate(const string& color) {
        return !isInCheck(color) && !hasLegalMove(color);
    }

private:

    // Returns true if 'color' has at least one legal move
    bool hasLegalMove(const string& color) {
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                Piece* p = grid[r][c];
                if (!p || p->getColor() != color) continue;

                for (int tr = 0; tr < 8; tr++) {
                    for (int tc = 0; tc < 8; tc++) {
                        if (!p->isValidMove(tr, tc, *this)) continue;

                        // Try the move
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