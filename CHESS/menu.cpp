#include <iostream>
#include <string>
using namespace std;

bool savedGame = false;

void displayMenu() {
    cout << "\n";
    cout << "  ================================\n";
    cout << "           CHESS GAME             \n";
    cout << "  ================================\n";
    cout << "   1. New Game                    \n";
    cout << "   2. Resume Game                 \n";
    cout << "   3. Rules                       \n";
    cout << "   4. Settings                    \n";
    cout << "   5. Quit                        \n";
    cout << "  ================================\n";
    cout << "  Enter your choice: ";
}

void playGame(bool isNew) {
    if (isNew) {
        cout << "  Setting up new board...\n";
    }
    else {
        cout << "  Restoring saved board...\n";
    }

    // Person 3's game loop goes here
    savedGame = true;
   
}

void startNewGame() {
    cout << "\n";
    cout << "  ================================\n";
    cout << "           NEW GAME               \n";
    cout << "  ================================\n";
    playGame(true);
}

void resumeGame() {
    cout << "\n";
    cout << "  ================================\n";
    cout << "          RESUME GAME             \n";
    cout << "  ================================\n";
    playGame(false);
}

void displayRules() {
    cout << "\n";
    cout << "  ================================\n";
    cout << "             RULES                \n";
    cout << "  ================================\n";
    cout << "  Objective  : Checkmate the      \n";
    cout << "               opponent's King    \n";
    cout << "  Pawn       : Forward 1 (can move 2 in first move)\n";
    cout << "  Rook       : Horizontal/Vertical\n";
    cout << "  Knight     : L-shape, jumps     \n";
    cout << "  Bishop     : Diagonal           \n";
    cout << "  Queen      : Rook + Bishop      \n";
    cout << "  King       : 1 square any direction   \n";
    cout << "  Draw       : Stalemate /        \n";
    cout << "               50-move rule      \n";
    cout << "  ================================\n";
    cout << "  Press Enter to go back...       \n";
    cin.ignore();
    cin.get();
}

void displaySettings() {
    int choice;
    cout << "\n";
    cout << "  ================================\n";
    cout << "            SETTINGS              \n";
    cout << "  ================================\n";
    cout << "   1. Player Names               \n";
    cout << "   2. First Move (White/Black)   \n";
    cout << "   3. Back                       \n";
    cout << "  ================================\n";
    cout << "  Enter your choice: ";
    cin >> choice;

    if (choice == 1) {
        string p1, p2;
        cout << "  Enter White player name: ";
        cin >> p1;
        cout << "  Enter Black player name: ";
        cin >> p2;
        cout << "  Names set: " << p1 << " vs " << p2 << "\n";
    }
    else if (choice == 2) {
        int first;
        cout << "  1. White goes first\n";
        cout << "  2. Black goes first\n";
        cout << "  Enter choice: ";
        cin >> first;
        if (first == 1) cout << "  White will go first.\n";
        else            cout << "  Black will go first.\n";
    }
    else if (choice == 3) {
        return;
    }
    else {
        cout << "\n  Invalid choice.\n";
    }
}

int main() {
    int choice;
    bool running = true;

    while (running) {
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            startNewGame();
            break;
        case 2:
            if (savedGame)
                resumeGame();
            else
                cout << "\n  No saved game found. Start a new game first.\n";
            break;
        case 3:
            displayRules();
            break;
        case 4:
            displaySettings();
            break;
        case 5:
            cout << "\n  Thanks for playing. Goodbye!\n\n";
            running = false;
            break;
        default:
            cout << "\n  Invalid choice. Try again.\n";
        }
    }
    return 0;
}