#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include "base.h"
#include "bishop.h"
#include "king.h"
#include "queen.h"
#include "pawn.h"
#include "rook.h"
#include "knight.h"

using namespace std;

// ============================================================
//  CONSTANTS
// ============================================================
const int TILE = 80;
const int BOARD_SIZE = 8;
const int WIN_W = TILE * BOARD_SIZE + 260;
const int WIN_H = TILE * BOARD_SIZE + 60;

const sf::Color LIGHT_SQ(240, 217, 181);
const sf::Color DARK_SQ(101, 67, 33);
const sf::Color SELECTED(255, 255, 0, 200);
const sf::Color VALID_DOT(50, 205, 50, 180);
const sf::Color PANEL_BG(15, 15, 25);
const sf::Color TEXT_COL(255, 255, 255);
const sf::Color WHITE_PC(255, 255, 255);
const sf::Color BLACK_PC(20, 20, 20);
const sf::Color BTN_COL(60, 20, 120);
const sf::Color BTN_HOV(120, 40, 220);
const sf::Color GOLD(255, 200, 0);

// ============================================================
//  SCREEN ENUM
// ============================================================
enum Screen { MENU, NAME_INPUT, RULES, GAME };

// ============================================================
//  BUTTON
// ============================================================
class Button {
public:
    sf::RectangleShape box;
    sf::Text           label;

    Button(sf::Font& font, const string& text, float x, float y, float w, float h)
        : label(font, text, 20)
    {
        box.setSize(sf::Vector2f(w, h));
        box.setPosition(sf::Vector2f(x, y));
        box.setFillColor(BTN_COL);
        box.setOutlineColor(sf::Color(100, 100, 140));
        box.setOutlineThickness(1);
        label.setFillColor(TEXT_COL);
        sf::FloatRect bounds = label.getLocalBounds();
        label.setPosition(sf::Vector2f(
            x + (w - bounds.size.x) / 2.f,
            y + (h - bounds.size.y) / 2.f - 4.f
        ));
    }

    void setLabel(const string& text) {
        label.setString(text);
    }

    void draw(sf::RenderWindow& win, sf::Vector2i mouse) {
        bool hover = box.getGlobalBounds().contains(
            sf::Vector2f((float)mouse.x, (float)mouse.y));
        box.setFillColor(hover ? BTN_HOV : BTN_COL);
        win.draw(box);
        win.draw(label);
    }

    bool isClicked(sf::Vector2i pos) {
        return box.getGlobalBounds().contains(
            sf::Vector2f((float)pos.x, (float)pos.y));
    }
};

// ============================================================
//  DRAW BACKGROUND
// ============================================================
void drawBackground(sf::RenderWindow& win) {
    win.clear(sf::Color(10, 5, 30));
}

// ============================================================
//  DRAW MENU
// ============================================================
void drawMenu(sf::RenderWindow& win, sf::Font& font,
    Button& btnNew, Button& btnLoad,
    Button& btnRules, Button& btnQuit,
    sf::Vector2i mouse) {
    drawBackground(win);

    sf::Text title(font, "CHESS", 72);
    title.setFillColor(GOLD);
    title.setStyle(sf::Text::Bold);
    sf::FloatRect tb = title.getLocalBounds();
    title.setPosition(sf::Vector2f((WIN_W - tb.size.x) / 2.f, 80.f));
    win.draw(title);

    sf::Text sub(font, "A Two Player Game", 22);
    sub.setFillColor(sf::Color(160, 160, 180));
    sf::FloatRect sb = sub.getLocalBounds();
    sub.setPosition(sf::Vector2f((WIN_W - sb.size.x) / 2.f, 170.f));
    win.draw(sub);

    sf::RectangleShape div(sf::Vector2f(300, 2));
    div.setFillColor(sf::Color(80, 80, 100));
    div.setPosition(sf::Vector2f((WIN_W - 300) / 2.f, 210.f));
    win.draw(div);

    btnNew.draw(win, mouse);
    btnLoad.draw(win, mouse);
    btnRules.draw(win, mouse);
    btnQuit.draw(win, mouse);
}

// ============================================================
//  DRAW NAME INPUT
// ============================================================
void drawNameInput(sf::RenderWindow& win, sf::Font& font,
    const string& name1, const string& name2,
    int activeField) {
    drawBackground(win);

    sf::Text title(font, "PLAYER NAMES", 48);
    title.setFillColor(GOLD);
    title.setStyle(sf::Text::Bold);
    sf::FloatRect tb = title.getLocalBounds();
    title.setPosition(sf::Vector2f((WIN_W - tb.size.x) / 2.f, 80.f));
    win.draw(title);

    sf::Text lbl1(font, "White Player Name:", 22);
    lbl1.setFillColor(TEXT_COL);
    lbl1.setPosition(sf::Vector2f((WIN_W - 400) / 2.f, 200.f));
    win.draw(lbl1);

    sf::RectangleShape box1(sf::Vector2f(400, 50));
    box1.setPosition(sf::Vector2f((WIN_W - 400) / 2.f, 235.f));
    box1.setFillColor(sf::Color(40, 40, 55));
    box1.setOutlineThickness(2);
    box1.setOutlineColor(activeField == 0 ? GOLD : sf::Color(80, 80, 100));
    win.draw(box1);

    sf::Text txt1(font, name1 + (activeField == 0 ? "|" : ""), 22);
    txt1.setFillColor(WHITE_PC);
    txt1.setPosition(sf::Vector2f((WIN_W - 400) / 2.f + 10.f, 243.f));
    win.draw(txt1);

    sf::Text lbl2(font, "Black Player Name:", 22);
    lbl2.setFillColor(TEXT_COL);
    lbl2.setPosition(sf::Vector2f((WIN_W - 400) / 2.f, 320.f));
    win.draw(lbl2);

    sf::RectangleShape box2(sf::Vector2f(400, 50));
    box2.setPosition(sf::Vector2f((WIN_W - 400) / 2.f, 355.f));
    box2.setFillColor(sf::Color(40, 40, 55));
    box2.setOutlineThickness(2);
    box2.setOutlineColor(activeField == 1 ? GOLD : sf::Color(80, 80, 100));
    win.draw(box2);

    sf::Text txt2(font, name2 + (activeField == 1 ? "|" : ""), 22);
    txt2.setFillColor(sf::Color(180, 180, 180));
    txt2.setPosition(sf::Vector2f((WIN_W - 400) / 2.f + 10.f, 363.f));
    win.draw(txt2);

    sf::Text hint(font, "Press Enter to confirm each name", 18);
    hint.setFillColor(sf::Color(120, 120, 140));
    sf::FloatRect hb = hint.getLocalBounds();
    hint.setPosition(sf::Vector2f((WIN_W - hb.size.x) / 2.f, 430.f));
    win.draw(hint);
}

// ============================================================
//  DRAW RULES
// ============================================================
void drawRules(sf::RenderWindow& win, sf::Font& font,
    Button& btnBack, sf::Vector2i mouse) {
    drawBackground(win);

    sf::Text title(font, "RULES", 48);
    title.setFillColor(GOLD);
    title.setStyle(sf::Text::Bold);
    sf::FloatRect tb = title.getLocalBounds();
    title.setPosition(sf::Vector2f((WIN_W - tb.size.x) / 2.f, 40.f));
    win.draw(title);

    sf::RectangleShape div(sf::Vector2f(500, 2));
    div.setFillColor(sf::Color(80, 80, 100));
    div.setPosition(sf::Vector2f((WIN_W - 500) / 2.f, 105.f));
    win.draw(div);

    string rules[] = {
        "Objective  : Checkmate the opponent's King",
        "Pawn       : Forward 1 square (2 on first move)",
        "Rook       : Horizontal or Vertical",
        "Knight     : L-shape, jumps over pieces",
        "Bishop     : Diagonal only",
        "Queen      : Rook + Bishop combined",
        "King       : 1 square in any direction",
        "",
        "Check      : King is under attack",
        "Checkmate  : King cannot escape - game over",
        "Stalemate  : No legal moves - draw",
        "50-move    : Draw offered after 50 moves",
        "75-move    : Automatic draw after 75 moves"
    };

    for (int i = 0; i < 13; i++) {
        sf::Text line(font, rules[i], 18);
        line.setFillColor(sf::Color(180, 180, 200));
        line.setPosition(sf::Vector2f((WIN_W - 500) / 2.f, 125.f + i * 32.f));
        win.draw(line);
    }

    btnBack.draw(win, mouse);
}

// ============================================================
//  SETUP BOARD
// ============================================================
void setupBoard(Board& b) {
    b.setPiece(0, 0, new Rook("black", 0, 0));
    b.setPiece(0, 1, new Knight("black", 0, 1));
    b.setPiece(0, 2, new Bishop("black", 0, 2));
    b.setPiece(0, 3, new Queen("black", 0, 3));
    b.setPiece(0, 4, new King("black", 0, 4));
    b.setPiece(0, 5, new Bishop("black", 0, 5));
    b.setPiece(0, 6, new Knight("black", 0, 6));
    b.setPiece(0, 7, new Rook("black", 0, 7));
    for (int c = 0; c < 8; c++) {
        b.setPiece(1, c, new Pawn("black", 1, c));
    }
    b.setPiece(7, 0, new Rook("white", 7, 0));
    b.setPiece(7, 1, new Knight("white", 7, 1));
    b.setPiece(7, 2, new Bishop("white", 7, 2));
    b.setPiece(7, 3, new Queen("white", 7, 3));
    b.setPiece(7, 4, new King("white", 7, 4));
    b.setPiece(7, 5, new Bishop("white", 7, 5));
    b.setPiece(7, 6, new Knight("white", 7, 6));
    b.setPiece(7, 7, new Rook("white", 7, 7));
    for (int c = 0; c < 8; c++) {
        b.setPiece(6, c, new Pawn("white", 6, c));
    }
}

// ============================================================
//  SAVE / LOAD
// ============================================================
Piece* makePiece(const string& name, const string& color, int r, int c) {
    if (name == "pawn")        return new Pawn(color, r, c);
    else if (name == "rook")   return new Rook(color, r, c);
    else if (name == "knight") return new Knight(color, r, c);
    else if (name == "bishop") return new Bishop(color, r, c);
    else if (name == "queen")  return new Queen(color, r, c);
    else if (name == "king")   return new King(color, r, c);
    else return nullptr;
}

void saveGame(const Board& b, const string& turn, int moveCount,
    const string& p1, const string& p2) {
    ofstream f("savegame.txt");
    if (!f) return;
    f << turn << "\n";
    f << moveCount << "\n";
    f << p1 << "\n";
    f << p2 << "\n";
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Piece* p = b.getPiece(r, c);
            if (p) {
                f << r << " " << c << " "
                    << p->getName() << " " << p->getColor() << "\n";
            }
        }
    }
    f << "END\n";
}

bool loadGame(Board& b, string& turn, int& moveCount,
    string& p1, string& p2) {
    ifstream f("savegame.txt");
    if (!f) return false;
    b.resetBoard();
    getline(f, turn);
    f >> moveCount;
    f.ignore();
    getline(f, p1);
    getline(f, p2);
    string token;
    while (f >> token && token != "END") {
        int r = stoi(token);
        int c;
        string name, color;
        f >> c >> name >> color;
        b.setPiece(r, c, makePiece(name, color, r, c));
    }
    return true;
}

// ============================================================
//  DRAW BOARD
// ============================================================
void drawBoard(sf::RenderWindow& win) {
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            sf::RectangleShape sq(sf::Vector2f(TILE, TILE));
            sq.setPosition(sf::Vector2f(c * TILE, r * TILE));
            sq.setFillColor((r + c) % 2 == 0 ? LIGHT_SQ : DARK_SQ);
            win.draw(sq);
        }
    }
}

void drawHighlight(sf::RenderWindow& win, int row, int col, sf::Color color) {
    sf::RectangleShape sq(sf::Vector2f(TILE, TILE));
    sq.setPosition(sf::Vector2f(col * TILE, row * TILE));
    sq.setFillColor(color);
    win.draw(sq);
}

void drawPieces(sf::RenderWindow& win, const Board& b, sf::Font& font) {
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Piece* p = b.getPiece(r, c);
            if (!p) continue;

            sf::Text shadow(font, string(1, p->getSymbol()), 44);
            shadow.setFillColor(sf::Color(0, 0, 0, 100));
            shadow.setStyle(sf::Text::Bold);
            shadow.setPosition(sf::Vector2f(c * TILE + 18, r * TILE + 12));
            win.draw(shadow);

            sf::Text txt(font, string(1, p->getSymbol()), 44);
            txt.setFillColor(p->getColor() == "white" ? WHITE_PC : BLACK_PC);
            txt.setStyle(sf::Text::Bold);
            txt.setPosition(sf::Vector2f(c * TILE + 16, r * TILE + 10));
            win.draw(txt);
        }
    }
}

// ============================================================
//  DRAW PANEL
// ============================================================
void drawPanel(sf::RenderWindow& win, sf::Font& font,
    const string& turn, const string& p1, const string& p2,
    int moveCount, bool paused, bool gameOver, const string& winner,
    bool drawOffered,
    Button& btnSave, Button& btnLoadG,
    Button& btnPause, Button& btnNewGame,
    Button& btnAcceptDraw, Button& btnDeclineDraw,
    sf::Vector2i mouse) {

    int px = BOARD_SIZE * TILE;

    sf::RectangleShape panel(sf::Vector2f(260, WIN_H));
    panel.setPosition(sf::Vector2f(px, 0));
    panel.setFillColor(PANEL_BG);
    win.draw(panel);

    sf::Text title(font, "CHESS", 28);
    title.setFillColor(GOLD);
    title.setStyle(sf::Text::Bold);
    title.setPosition(sf::Vector2f(px + 70, 15));
    win.draw(title);

    sf::RectangleShape div1(sf::Vector2f(240, 2));
    div1.setFillColor(sf::Color(80, 80, 100));
    div1.setPosition(sf::Vector2f(px + 10, 58));
    win.draw(div1);

    sf::Text wp(font, "W: " + p1, 18);
    wp.setFillColor(sf::Color(220, 220, 220));
    wp.setPosition(sf::Vector2f(px + 10, 70));
    win.draw(wp);

    sf::Text bp(font, "B: " + p2, 18);
    bp.setFillColor(sf::Color(150, 150, 150));
    bp.setPosition(sf::Vector2f(px + 10, 95));
    win.draw(bp);

    sf::RectangleShape div2(sf::Vector2f(240, 2));
    div2.setFillColor(sf::Color(80, 80, 100));
    div2.setPosition(sf::Vector2f(px + 10, 125));
    win.draw(div2);

    string statusStr;
    if (gameOver)         statusStr = "Winner: " + winner;
    else if (paused)      statusStr = "PAUSED";
    else if (drawOffered) statusStr = "Draw offered!";
    else                  statusStr = (turn == "white" ? p1 : p2) + "'s turn";

    sf::Text status(font, statusStr, 17);
    if (gameOver)            status.setFillColor(sf::Color(220, 80, 80));
    else if (paused)         status.setFillColor(sf::Color(220, 180, 80));
    else if (drawOffered)    status.setFillColor(sf::Color(220, 180, 80));
    else                     status.setFillColor(sf::Color(100, 220, 100));
    status.setPosition(sf::Vector2f(px + 10, 135));
    win.draw(status);

    sf::Text moves(font, "Moves: " + to_string(moveCount), 16);
    moves.setFillColor(sf::Color(140, 140, 160));
    moves.setPosition(sf::Vector2f(px + 10, 160));
    win.draw(moves);

    sf::RectangleShape div3(sf::Vector2f(240, 2));
    div3.setFillColor(sf::Color(80, 80, 100));
    div3.setPosition(sf::Vector2f(px + 10, 188));
    win.draw(div3);

    if (drawOffered && !gameOver) {
        btnAcceptDraw.draw(win, mouse);
        btnDeclineDraw.draw(win, mouse);
    }
    else {
        btnNewGame.draw(win, mouse);
        btnSave.draw(win, mouse);
        btnLoadG.draw(win, mouse);
        btnPause.draw(win, mouse);
    }

    sf::RectangleShape div4(sf::Vector2f(240, 2));
    div4.setFillColor(sf::Color(80, 80, 100));
    div4.setPosition(sf::Vector2f(px + 10, 430));
    win.draw(div4);

    sf::Text legend(font,
        "K=King  Q=Queen\nR=Rook  B=Bishop\nN=Knight  P=Pawn\n\nUPPER=White\nlower=Black",
        14);
    legend.setFillColor(sf::Color(130, 130, 150));
    legend.setPosition(sf::Vector2f(px + 10, 440));
    win.draw(legend);
}

// ============================================================
//  DRAW STATUS BAR
// ============================================================
void drawStatusBar(sf::RenderWindow& win, sf::Font& font, const string& msg) {
    sf::RectangleShape bar(sf::Vector2f(WIN_W, 40));
    bar.setPosition(sf::Vector2f(0, BOARD_SIZE * TILE));
    bar.setFillColor(sf::Color(15, 15, 25));
    win.draw(bar);

    sf::Text txt(font, msg, 16);
    txt.setFillColor(sf::Color(180, 180, 200));
    txt.setPosition(sf::Vector2f(10, BOARD_SIZE * TILE + 10));
    win.draw(txt);
}

// ============================================================
//  COMPUTE VALID MOVES
// ============================================================
void computeValidMoves(Board& b, int row, int col, bool validMoves[8][8]) {
    Piece* p = b.getPiece(row, col);
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (p && p->isValidMove(r, c, b)) {
                Piece* captured = b.getPiece(r, c);
                b.simulateMove(row, col, r, c);
                bool exposesKing = b.isInCheck(p->getColor());
                b.undoMove(row, col, r, c, captured);
                validMoves[r][c] = !exposesKing;
            }
            else {
                validMoves[r][c] = false;
            }
        }
    }
}

// ============================================================
//  PROMOTION — only sets the pending flag, does NOT place a piece
//  Returns true if promotion is needed at (row, col)
// ============================================================
bool checkPromotionNeeded(Board& b, int row, int col) {
    Piece* p = b.getPiece(row, col);
    if (!p || p->getName() != "pawn") return false;
    return (p->getColor() == "white" && row == 0) ||
        (p->getColor() == "black" && row == 7);
}

// ============================================================
//  DRAW PROMOTION OVERLAY
//  Darkens the board, then shows a centered panel with 4 choices.
//  pieceColor is "white" or "black" — symbols rendered accordingly.
// ============================================================
void drawPromotionOverlay(sf::RenderWindow& win, sf::Font& font,
    const string& pieceColor, sf::Vector2i mouse)
{
    // --- dim overlay covering the whole board area ---
    sf::RectangleShape dim(sf::Vector2f(BOARD_SIZE * TILE, BOARD_SIZE * TILE));
    dim.setFillColor(sf::Color(0, 0, 0, 160));
    win.draw(dim);

    // --- centered panel ---
    const float PW = 360.f, PH = 210.f;
    const float PX = (BOARD_SIZE * TILE - PW) / 2.f;
    const float PY = (BOARD_SIZE * TILE - PH) / 2.f;

    sf::RectangleShape panel(sf::Vector2f(PW, PH));
    panel.setPosition(sf::Vector2f(PX, PY));
    panel.setFillColor(sf::Color(20, 15, 40, 245));
    panel.setOutlineColor(GOLD);
    panel.setOutlineThickness(2);
    win.draw(panel);

    sf::Text heading(font, "Promote Pawn", 22);
    heading.setFillColor(GOLD);
    heading.setStyle(sf::Text::Bold);
    sf::FloatRect hb = heading.getLocalBounds();
    heading.setPosition(sf::Vector2f(PX + (PW - hb.size.x) / 2.f, PY + 12.f));
    win.draw(heading);

    // 4 choice boxes: Queen, Rook, Bishop, Knight
    // symbols and labels match your existing piece symbol convention
    struct Choice { char sym; const char* label; };
    // symbols: uppercase = white, lowercase = black (matching your drawPieces logic)
    Choice choices[4] = {
        { (pieceColor == "white" ? 'Q' : 'q'), "Queen"  },
        { (pieceColor == "white" ? 'R' : 'r'), "Rook"   },
        { (pieceColor == "white" ? 'B' : 'b'), "Bishop" },
        { (pieceColor == "white" ? 'N' : 'n'), "Knight" }
    };

    const float BOX_W = 70.f, BOX_H = 90.f;
    const float GAP = 16.f;
    float totalW = 4 * BOX_W + 3 * GAP;
    float startX = PX + (PW - totalW) / 2.f;
    float startY = PY + 55.f;

    for (int i = 0; i < 4; i++) {
        float bx = startX + i * (BOX_W + GAP);
        float by = startY;

        sf::FloatRect hitBox(sf::Vector2f(bx, by), sf::Vector2f(BOX_W, BOX_H));
        bool hovered = hitBox.contains(sf::Vector2f((float)mouse.x, (float)mouse.y));

        sf::RectangleShape box(sf::Vector2f(BOX_W, BOX_H));
        box.setPosition(sf::Vector2f(bx, by));
        box.setFillColor(hovered ? sf::Color(80, 40, 160) : sf::Color(40, 25, 80));
        box.setOutlineColor(hovered ? GOLD : sf::Color(80, 80, 120));
        box.setOutlineThickness(hovered ? 2.f : 1.f);
        win.draw(box);

        // piece symbol
        sf::Text sym(font, string(1, choices[i].sym), 38);
        sym.setFillColor(pieceColor == "white" ? WHITE_PC : BLACK_PC);
        sym.setStyle(sf::Text::Bold);
        sf::FloatRect sb = sym.getLocalBounds();
        sym.setPosition(sf::Vector2f(bx + (BOX_W - sb.size.x) / 2.f - 2.f,
            by + 8.f));
        win.draw(sym);

        // label below symbol
        sf::Text lbl(font, choices[i].label, 13);
        lbl.setFillColor(sf::Color(200, 200, 220));
        sf::FloatRect lb = lbl.getLocalBounds();
        lbl.setPosition(sf::Vector2f(bx + (BOX_W - lb.size.x) / 2.f,
            by + BOX_H - 22.f));
        win.draw(lbl);
    }
}

// ============================================================
//  HANDLE PROMOTION CLICK
//  Call when promotionPending is true and a left-click fires.
//  Returns the chosen piece name, or "" if click missed all boxes.
// ============================================================
string getPromotionChoice(sf::Vector2i cp, float PX, float PY,
    float PW, float PH)
{
    const float BOX_W = 70.f, BOX_H = 90.f, GAP = 16.f;
    float totalW = 4 * BOX_W + 3 * GAP;
    float startX = PX + (PW - totalW) / 2.f;
    float startY = PY + 55.f;

    const char* names[4] = { "queen", "rook", "bishop", "knight" };

    for (int i = 0; i < 4; i++) {
        float bx = startX + i * (BOX_W + GAP);
        sf::FloatRect hitBox(sf::Vector2f(bx, startY),
            sf::Vector2f(BOX_W, BOX_H));
        if (hitBox.contains(sf::Vector2f((float)cp.x, (float)cp.y)))
            return names[i];
    }
    return "";   // missed — keep overlay open
}

// ============================================================
//  POST-MOVE CHECKS  (extracted so we call it from one place)
// ============================================================
void runPostMoveChecks(Board& board, const string& currentTurn,
    const string& player1, const string& player2,
    bool& gameOver, string& winner, string& statusMsg,
    bool& drawOffered)
{
    if (board.isCheckmate(currentTurn)) {
        gameOver = true;
        winner = (currentTurn == "white") ? player2 : player1;
        statusMsg = "CHECKMATE! " + winner + " wins!";
    }
    else if (board.isStalemate(currentTurn)) {
        gameOver = true;
        winner = "Draw";
        statusMsg = "STALEMATE! It's a draw!";
    }
    else if (board.isSeventyFiveMoveRule()) {
        gameOver = true;
        winner = "Draw";
        statusMsg = "75-move rule! Automatic draw!";
    }
    else if (board.isFiftyMoveRule()) {
        drawOffered = true;
        statusMsg = "50 moves! Accept or decline the draw.";
    }
    else if (board.isInCheck(currentTurn)) {
        statusMsg = (currentTurn == "white" ? player1 : player2) + " is in CHECK!";
    }
    else {
        statusMsg = (currentTurn == "white" ? player1 : player2) + "'s turn.";
    }
}

// ============================================================
//  MAIN
// ============================================================
int main() {
    sf::RenderWindow window(
        sf::VideoMode(sf::Vector2u(WIN_W, WIN_H)), "Chess");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("Cinzel-Regular.ttf")) {
        if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
            return -1;
        }
    }

    Screen currentScreen = MENU;
    string player1 = "";
    string player2 = "";
    int    activeField = 0;

    float bx = (WIN_W - 280) / 2.f;
    Button btnNew(font, "New Game", bx, 260, 280, 55);
    Button btnLoad(font, "Load Game", bx, 330, 280, 55);
    Button btnRules(font, "Rules", bx, 400, 280, 55);
    Button btnQuit(font, "Quit", bx, 470, 280, 55);
    Button btnBack(font, "Back", (WIN_W - 200) / 2.f, 570, 200, 50);

    int gx = BOARD_SIZE * TILE + 10;
    Button btnSave(font, "Save Game", gx, 200, 240, 45);
    Button btnLoadG(font, "Load Game", gx, 255, 240, 45);
    Button btnPause(font, "Pause", gx, 310, 240, 45);
    Button btnNewGame(font, "New Game", gx, 365, 240, 45);

    Button btnAcceptDraw(font, "Accept Draw", gx, 200, 240, 45);
    Button btnDeclineDraw(font, "Decline Draw", gx, 255, 240, 45);

    Board  board;
    bool   gameOver = false;
    bool   paused = false;
    string currentTurn = "white";
    string winner = "";
    string statusMsg = "";
    int    moveCount = 0;
    int    selRow = -1, selCol = -1;
    bool   pieceSelected = false;
    bool   validMoves[8][8] = {};
    bool   drawOffered = false;

    // ---- PROMOTION STATE ----
    bool   promotionPending = false;  // true = overlay is showing, game is paused
    int    promoRow = -1, promoCol = -1;   // where the pawn landed
    string promoColor = "";               // "white" or "black"

    // overlay geometry (constant, computed once)
    const float PW = 360.f, PH = 210.f;
    const float PX = (BOARD_SIZE * TILE - PW) / 2.f;
    const float PY = (BOARD_SIZE * TILE - PH) / 2.f;

    while (window.isOpen()) {

        sf::Vector2i mouse = sf::Mouse::getPosition(window);

        while (const std::optional event = window.pollEvent()) {

            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // ---- MENU ----
            if (currentScreen == MENU) {
                if (const auto* click = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (click->button == sf::Mouse::Button::Left) {
                        sf::Vector2i cp(click->position.x, click->position.y);
                        if (btnNew.isClicked(cp)) {
                            player1 = ""; player2 = "";
                            activeField = 0;
                            currentScreen = NAME_INPUT;
                        }
                        else if (btnLoad.isClicked(cp)) {
                            if (loadGame(board, currentTurn, moveCount, player1, player2)) {
                                paused = false; gameOver = false;
                                pieceSelected = false;
                                selRow = selCol = -1;
                                drawOffered = false;
                                promotionPending = false;
                                statusMsg = "Game loaded!";
                                currentScreen = GAME;
                            }
                            else { statusMsg = "No save file found!"; }
                        }
                        else if (btnRules.isClicked(cp)) { currentScreen = RULES; }
                        else if (btnQuit.isClicked(cp)) { window.close(); }
                    }
                }
            }

            // ---- NAME INPUT ----
            else if (currentScreen == NAME_INPUT) {
                if (const auto* text = event->getIf<sf::Event::TextEntered>()) {
                    unsigned int c = text->unicode;
                    if (c == 13) {
                        if (activeField == 0 && player1.size() > 0) {
                            activeField = 1;
                        }
                        else if (activeField == 1 && player2.size() > 0) {
                            board.resetBoard();
                            setupBoard(board);
                            currentTurn = "white";
                            moveCount = 0;
                            gameOver = false;
                            paused = false;
                            winner = "";
                            pieceSelected = false;
                            selRow = selCol = -1;
                            drawOffered = false;
                            promotionPending = false;
                            statusMsg = player1 + "'s turn (White)";
                            currentScreen = GAME;
                        }
                    }
                    else if (c == 8) {
                        if (activeField == 0 && player1.size() > 0)
                            player1.erase(player1.size() - 1);
                        else if (activeField == 1 && player2.size() > 0)
                            player2.erase(player2.size() - 1);
                    }
                    else if (c >= 32 && c < 128) {
                        if (activeField == 0 && player1.size() < 15)
                            player1 += (char)c;
                        else if (activeField == 1 && player2.size() < 15)
                            player2 += (char)c;
                    }
                }
            }

            // ---- RULES ----
            else if (currentScreen == RULES) {
                if (const auto* click = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (click->button == sf::Mouse::Button::Left) {
                        sf::Vector2i cp(click->position.x, click->position.y);
                        if (btnBack.isClicked(cp)) currentScreen = MENU;
                    }
                }
            }

            // ---- GAME ----
            else if (currentScreen == GAME) {
                if (const auto* click = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (click->button == sf::Mouse::Button::Left) {
                        sf::Vector2i cp(click->position.x, click->position.y);

                        // ================================================
                        //  PROMOTION OVERLAY — intercepts ALL clicks when active
                        // ================================================
                        if (promotionPending) {
                            string chosen = getPromotionChoice(cp, PX, PY, PW, PH);
                            if (!chosen.empty()) {
                                // delete the pawn and place the chosen piece
                                Piece* pawn = board.getPiece(promoRow, promoCol);
                                delete pawn;

                                Piece* newPiece = makePiece(chosen, promoColor,
                                    promoRow, promoCol);
                                board.setPiece(promoRow, promoCol, newPiece);

                                promotionPending = false;

                                // NOW switch turn and run post-move checks
                                currentTurn = (currentTurn == "white") ? "black" : "white";
                                runPostMoveChecks(board, currentTurn,
                                    player1, player2,
                                    gameOver, winner,
                                    statusMsg, drawOffered);
                            }
                            // if click missed all boxes → do nothing, keep overlay open
                            continue;  // skip all other click handling this frame
                        }

                        // ================================================
                        //  NORMAL GAME CLICKS (promotion not pending)
                        // ================================================
                        if (btnNewGame.isClicked(cp)) {
                            player1 = ""; player2 = "";
                            activeField = 0;
                            currentScreen = NAME_INPUT;
                        }
                        else if (btnSave.isClicked(cp)) {
                            saveGame(board, currentTurn, moveCount, player1, player2);
                            statusMsg = "Game saved!";
                        }
                        else if (btnLoadG.isClicked(cp)) {
                            if (loadGame(board, currentTurn, moveCount, player1, player2)) {
                                paused = false; gameOver = false;
                                pieceSelected = false;
                                selRow = selCol = -1;
                                drawOffered = false;
                                promotionPending = false;
                                statusMsg = "Game loaded!";
                            }
                            else { statusMsg = "No save file found!"; }
                        }
                        else if (btnPause.isClicked(cp)) {
                            paused = !paused;
                            if (paused) {
                                statusMsg = "Game paused.";
                                btnPause.setLabel("Resume");
                            }
                            else {
                                statusMsg = (currentTurn == "white" ? player1 : player2) + "'s turn.";
                                btnPause.setLabel("Pause");
                            }
                        }
                        else if (drawOffered && btnAcceptDraw.isClicked(cp)) {
                            gameOver = true;
                            winner = "Draw";
                            statusMsg = "Draw accepted!";
                            drawOffered = false;
                        }
                        else if (drawOffered && btnDeclineDraw.isClicked(cp)) {
                            drawOffered = false;
                            statusMsg = (currentTurn == "white" ? player1 : player2) + "'s turn.";
                        }
                        else if (!paused && !gameOver && !drawOffered &&
                            cp.x < BOARD_SIZE * TILE &&
                            cp.y < BOARD_SIZE * TILE) {

                            int col = cp.x / TILE;
                            int row = cp.y / TILE;

                            if (!pieceSelected) {
                                Piece* p = board.getPiece(row, col);
                                if (p && p->getColor() == currentTurn) {
                                    selRow = row; selCol = col;
                                    pieceSelected = true;
                                    computeValidMoves(board, row, col, validMoves);
                                    statusMsg = "Selected " + p->getName() + ". Click destination.";
                                }
                            }
                            else {
                                Piece* p = board.getPiece(selRow, selCol);
                                if (p && p->isValidMove(row, col, board)) {

                                    Piece* captured = board.getPiece(row, col);
                                    board.simulateMove(selRow, selCol, row, col);
                                    bool exposesKing = board.isInCheck(currentTurn);
                                    board.undoMove(selRow, selCol, row, col, captured);

                                    if (exposesKing) {
                                        statusMsg = "Illegal! Your King would be in check.";
                                        pieceSelected = false;
                                        selRow = selCol = -1;
                                    }
                                    else {
                                        // ============================================
                                        // CASTLING DETECTION ADDED HERE
                                        // ============================================

                                        // Check if this is a castling move (King moving 2 squares)
                                        if (p->getName() == "king" && abs(col - selCol) == 2) {
                                            // This is castling
                                            int kingRow = selRow;
                                            int kingCol = selCol;
                                            int kingToCol = col;
                                            int rookCol, rookToCol;

                                            if (kingToCol > kingCol) {
                                                // Kingside castling (O-O)
                                                rookCol = 7;
                                                rookToCol = kingToCol - 1;
                                            }
                                            else {
                                                // Queenside castling (O-O-O)
                                                rookCol = 0;
                                                rookToCol = kingToCol + 1;
                                            }

                                            board.performCastle(kingRow, kingCol, kingToCol, rookCol, rookToCol);
                                        }
                                        else {
                                            // Normal move
                                            board.movePieceTo(selRow, selCol, row, col);
                                        }

                                        moveCount++;

                                        // ---- CHECK FOR PROMOTION ----
                                        if (checkPromotionNeeded(board, row, col)) {
                                            // freeze the game and show the overlay
                                            promotionPending = true;
                                            promoRow = row;
                                            promoCol = col;
                                            promoColor = board.getPiece(row, col)->getColor();
                                            statusMsg = "Choose a piece to promote to!";
                                            // do NOT switch turn yet — that happens after choice
                                        }
                                        else {
                                            // no promotion — switch turn and run checks immediately
                                            currentTurn = (currentTurn == "white") ? "black" : "white";
                                            runPostMoveChecks(board, currentTurn,
                                                player1, player2,
                                                gameOver, winner,
                                                statusMsg, drawOffered);
                                        }

                                        pieceSelected = false;
                                        selRow = selCol = -1;
                                    }
                                }
                                else if (board.getPiece(row, col) &&
                                    board.getPiece(row, col)->getColor() == currentTurn) {
                                    selRow = row; selCol = col;
                                    computeValidMoves(board, row, col, validMoves);
                                    statusMsg = "Selected " + board.getPiece(row, col)->getName() + ".";
                                }
                                else {
                                    pieceSelected = false;
                                    selRow = selCol = -1;
                                }
                            }
                        }
                    }
                }
            }

        } // end event loop

        // ---- DRAW ----
        if (currentScreen == MENU) {
            drawMenu(window, font, btnNew, btnLoad, btnRules, btnQuit, mouse);
        }
        else if (currentScreen == NAME_INPUT) {
            drawNameInput(window, font, player1, player2, activeField);
        }
        else if (currentScreen == RULES) {
            drawRules(window, font, btnBack, mouse);
        }
        else if (currentScreen == GAME) {
            drawBackground(window);
            drawBoard(window);
            if (pieceSelected) {
                drawHighlight(window, selRow, selCol, SELECTED);
                for (int r = 0; r < 8; r++)
                    for (int c = 0; c < 8; c++)
                        if (validMoves[r][c])
                            drawHighlight(window, r, c, VALID_DOT);
            }
            drawPieces(window, board, font);
            drawPanel(window, font, currentTurn, player1, player2,
                moveCount, paused, gameOver, winner, drawOffered,
                btnSave, btnLoadG, btnPause, btnNewGame,
                btnAcceptDraw, btnDeclineDraw, mouse);
            drawStatusBar(window, font, statusMsg);

            // draw promotion overlay on top of everything else
            if (promotionPending) {
                drawPromotionOverlay(window, font, promoColor, mouse);
            }
        }

        window.display();

    } // end main loop

    return 0;
}