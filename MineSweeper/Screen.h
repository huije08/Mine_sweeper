#pragma once
#include <windows.h>

void initialize();
void flip();
void clear();
void release();
void render(int x, int y, const char* character);
void drawBoard(int X, int Y);
void drawMenu();
void drawOver();
void Set_mine(int size, int diff);
void reset_board();
void calculateNumbers();