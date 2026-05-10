#include <iostream>
#include "base.h"
#include "pawn.h"
#include "rook.h"
#include "knight.h"
using namespace std;


class TestBoard : public Board {
    Piece* grid[8][8];
public:
    TestBoard() {
        for (int r = 0; r < 8; r++)
            for (int c = 0; c < 8; c++)
                grid[r][c] = nullptr;
    }
    void   place(Piece* p)        { grid[p->getRow()][p->getCol()] = p; }
    void   remove(int r, int c)   { grid[r][c] = nullptr; }
    Piece* getPiece(int r, int c) override { return grid[r][c]; }
};

int main() {
    cout << "=== Chess Piece Symbol Test ===\n\n";

    Pawn   wp("white", 6, 0);
    Pawn   bp("black", 1, 0);
    Rook   wr("white", 7, 0);
    Rook   br("black", 0, 0);
    Knight wn("white", 7, 1);
    Knight bn("black", 0, 1);

    cout << "White Pawn   symbol: " << wp.getSymbol() << "  name: " << wp.getName() << "\n";
    cout << "Black Pawn   symbol: " << bp.getSymbol() << "  name: " << bp.getName() << "\n";
    cout << "White Rook   symbol: " << wr.getSymbol() << "  name: " << wr.getName() << "\n";
    cout << "Black Rook   symbol: " << br.getSymbol() << "  name: " << br.getName() << "\n";
    cout << "White Knight symbol: " << wn.getSymbol() << "  name: " << wn.getName() << "\n";
    cout << "Black Knight symbol: " << bn.getSymbol() << "  name: " << bn.getName() << "\n";

    cout << "\n=== Move Validation Test ===\n\n";

    TestBoard board;
    board.place(&wp); 

    cout << "White pawn (6,0) -> (5,0): "
         << (wp.isValidMove(5, 0, board) ? "VALID" : "INVALID") << " (expect VALID)\n";

    cout << "White pawn (6,0) -> (4,0): "
         << (wp.isValidMove(4, 0, board) ? "VALID" : "INVALID") << " (expect VALID)\n";

    cout << "White pawn (6,0) -> (7,0): "
         << (wp.isValidMove(7, 0, board) ? "VALID" : "INVALID") << " (expect INVALID)\n";

    board.place(&wr); 
    cout << "\nWhite rook (7,0) -> (4,0): "
         << (wr.isValidMove(4, 0, board) ? "VALID" : "INVALID") << " (expect VALID)\n";
    cout << "White rook (7,0) -> (7,5): "
         << (wr.isValidMove(7, 5, board) ? "VALID" : "INVALID") << " (expect VALID)\n";
    cout << "White rook (7,0) -> (5,3): "
         << (wr.isValidMove(5, 3, board) ? "VALID" : "INVALID") << " (expect INVALID)\n";

    board.place(&wn); 
    cout << "\nWhite knight (7,1) -> (5,2): "
         << (wn.isValidMove(5, 2, board) ? "VALID" : "INVALID") << " (expect VALID)\n";
    cout << "White knight (7,1) -> (5,0): "
         << (wn.isValidMove(5, 0, board) ? "VALID" : "INVALID") << " (expect VALID)\n";
    cout << "White knight (7,1) -> (5,1): "
         << (wn.isValidMove(5, 1, board) ? "VALID" : "INVALID") << " (expect INVALID)\n";

    cout << "\n=== hasMoved Tracking ===\n";
    cout << "White pawn hasMoved before: " << wp.getHasMoved() << " (expect 0)\n";
    wp.onMoved();
    cout << "White pawn hasMoved after:  " << wp.getHasMoved() << " (expect 1)\n";

    cout << "White rook hasMoved before: " << wr.getHasMoved() << " (expect 0)\n";
    wr.onMoved();
    cout << "White rook hasMoved after:  " << wr.getHasMoved() << " (expect 1)\n";

    cout << "\nAll tests complete.\n";
    return 0;
}
