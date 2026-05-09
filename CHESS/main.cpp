#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include "base.h"
#include "bishop.cpp"
#include "king.cpp"
#include "queen.cpp"
#include "pawn.cpp"
#include "rook.cpp"
#include "knight.cpp"

using namespace std;

const int TILE = 80;
const int BOARD_SIZE = 8;
const int WIN_W = TILE * BOARD_SIZE + 260;
const int WIN_H = TILE * BOARD_SIZE + 60;

const sf::Color LIGHT_SQ(240, 217, 181);
const sf::Color DARK_SQ(181, 136, 99);
const sf::Color SELECTED(247, 247, 105, 180);
const sf::Color VALID_DOT(100, 200, 100, 160);
const sf::Color PANEL_BG(30, 30, 40);
const sf::Color TEXT_COL(230, 230, 230);
const sf::Color WHITE_PC(255, 255, 255);
const sf::Color BLACK_PC(30, 30, 30);
const sf::Color BTN_COL(70, 70, 100);
const sf::Color BTN_HOV(100, 100, 140);
const sf::Color GOLD(200, 160, 80);

enum Screen { MENU, NAME_INPUT, RULES, GAME };

struct Button {
    sf::RectangleShape box;
    sf::Text           label;

    Button(sf::Font& font, const string& text, float x, float y, float w, float h) {
        box.setSize(sf::Vector2f(w, h));
        box.setPosition(sf::Vector2f(x, y));
        box.setFillColor(BTN_COL);
        box.setOutlineColor(sf::Color(100, 100, 140));
        box.setOutlineThickness(1);
        label = sf::Text(font, text, 20);
        label.setFillColor(TEXT_COL);
        sf::FloatRect bounds = label.getLocalBounds();
        label.setPosition(sf::Vector2f(
            x + (w - bounds.size.x) / 2.f,
            y + (h - bounds.size.y) / 2.f - 4.f
        ));
    }

    void draw(sf::RenderWindow& win, sf::Vector2i mouse) {
        bool hover = box.getGlobalBounds().contains(sf::Vector2f((float)mouse.x, (float)mouse.y));
        box.setFillColor(hover ? BTN_HOV : BTN_COL);
        win.draw(box);
        win.draw(label);
    }

    bool isClicked(sf::Vector2i pos) {
        return box.getGlobalBounds().contains(sf::Vector2f((float)pos.x, (float)pos.y));
    }
};

void drawBackground(sf::RenderWindow& win) {
    win.clear(sf::Color(20, 20, 30));
}

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

void setupBoard(Board& b) {
    b.setPiece(0, 0, new Rook("black", 0, 0));
    b.setPiece(0, 1, new Knight("black", 0, 1));
    b.setPiece(0, 2, new Bishop("black", 0, 2));
    b.setPiece(0, 3, new Queen("black", 0, 3));
    b.setPiece(0, 4, new King("black", 0, 4));
    b.setPiece(0, 5, new Bishop("black", 0, 5));
    b.setPiece(0, 6, new Knight("black", 0, 6));
    b.setPiece(0, 7, new Rook("black", 0, 7));
    for (int c = 0; c < 8; c++)
        b.setPiece(1, c, new Pawn("black", 1, c));

    b.setPiece(7, 0, new Rook("white", 7, 0));
    b.setPiece(7, 1, new Knight("white", 7, 1));
    b.setPiece(7, 2, new Bishop("white", 7, 2));
    b.setPiece(7, 3, new Queen("white", 7, 3));
    b.setPiece(7, 4, new King("white", 7, 4));
    b.setPiece(7, 5, new Bishop("white", 7, 5));
    b.setPiece(7, 6, new Knight("white", 7, 6));
    b.setPiece(7, 7, new Rook("white", 7, 7));
    for (int c = 0; c < 8; c++)
        b.setPiece(6, c, new Pawn("white", 6, c));
}

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(WIN_W, WIN_H)), "Chess");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("Cinzel-Regular.ttf"))
        if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))
            return -1;

    Screen currentScreen = MENU;
    string player1 = "", player2 = "";
    int activeField = 0;

    float bx = (WIN_W - 280) / 2.f;
    Button btnNew(font, "New Game", bx, 260, 280, 55);
    Button btnLoad(font, "Load Game", bx, 330, 280, 55);
    Button btnRules(font, "Rules", bx, 400, 280, 55);
    Button btnQuit(font, "Quit", bx, 470, 280, 55);
    Button btnBack(font, "Back", (WIN_W - 200) / 2.f, 570, 200, 50);

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

    while (window.isOpen()) {

        sf::Vector2i mouse = sf::Mouse::getPosition(window);

        while (const std::optional event = window.pollEvent()) {

            if (event->is<sf::Event::Closed>())
                window.close();

            // ---- MENU ----
            if (currentScreen == MENU) {
                if (const auto* click = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (click->button == sf::Mouse::Button::Left) {
                        sf::Vector2i cp(click->position.x, click->position.y);
                        if (btnNew.isClicked(cp)) { player1 = ""; player2 = ""; activeField = 0; currentScreen = NAME_INPUT; }
                        else if (btnLoad.isClicked(cp)) { /* Part 2 */ }
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
                        if (activeField == 0 && player1.size() > 0)
                            activeField = 1;
                        else if (activeField == 1 && player2.size() > 0) {
                            board.resetBoard();
                            setupBoard(board);
                            currentTurn = "white"; moveCount = 0;
                            gameOver = false; paused = false;
                            winner = ""; pieceSelected = false;
                            selRow = -1; selCol = -1;
                            drawOffered = false;
                            statusMsg = player1 + "'s turn (White)";
                            currentScreen = GAME;
                        }
                    }
                    else if (c == 8) {
                        if (activeField == 0 && player1.size() > 0) player1.erase(player1.size() - 1);
                        else if (activeField == 1 && player2.size() > 0) player2.erase(player2.size() - 1);
                    }
                    else if (c >= 32 && c < 128) {
                        if (activeField == 0 && player1.size() < 15) player1 += (char)c;
                        else if (activeField == 1 && player2.size() < 15) player2 += (char)c;
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

            // ---- GAME — Part 2 goes here ----
        }

        if (currentScreen == MENU)       drawMenu(window, font, btnNew, btnLoad, btnRules, btnQuit, mouse);
        else if (currentScreen == NAME_INPUT) drawNameInput(window, font, player1, player2, activeField);
        else if (currentScreen == RULES)      drawRules(window, font, btnBack, mouse);
        // game drawing — Part 2

        window.display();
    }

    return 0;
}