#include<iostream>
using namespace std;
class Board {
private:
    Piece* grid[8][8];
    int halfMoveClock;  // add this — counts moves since last capture or pawn move

public:
    Board() {
        for (int r = 0; r < 8; r++)
            for (int c = 0; c < 8; c++)
                grid[r][c] = nullptr;
        halfMoveClock = 0;  // initialize to 0
    }

    // add this getter so game loop can check it
    int getHalfMoveClock() const {
        return halfMoveClock;
    }

    // add this reset so game loop can reset it when needed
    void resetHalfMoveClock() {
        halfMoveClock = 0;
    }

    void incrementHalfMoveClock() {
        halfMoveClock++;
    }

    bool isFiftyMoveRule() const {
        return halfMoveClock >= 100;  // 50 moves each = 100 half moves
    }