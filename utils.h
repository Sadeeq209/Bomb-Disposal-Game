#pragma once
#include <iostream>
#include <string>

enum Color {
    COLOR_WHITE  = 7,
    COLOR_RED    = 12,
    COLOR_GREEN  = 10,
    COLOR_YELLOW = 14,
    COLOR_BLUE   = 9,
    COLOR_GRAY   = 8
};

void setColor(Color color);
void resetColor();
void clearScreen();
void setCursorPosition(int x, int y);
void hideCursor();
void showCursor();
void waitForEnter();
void printSeparator();
void printTitle(const std::string& text);