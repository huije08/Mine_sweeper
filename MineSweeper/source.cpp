#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include "Screen.h"

#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80
#define ENTER 13

#define SIZE 8
extern int menuindex;

extern int mine_board[SIZE][SIZE];
extern int game_board[SIZE][SIZE];
extern int flag_board[SIZE][SIZE];
extern int count_board[SIZE][SIZE];

int first = 1;

enum State
{
    START,
    GAME,
    OVER,
    CLEAR
};

enum Difficulty
{
    EASY=10,
    NORMAL=25,
    HARD=60
};

int cellcount;

int main()
{
    srand(time(NULL));

    CONSOLE_SCREEN_BUFFER_INFO console;

    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(handle, &console);

    int width = console.srWindow.Right - console.srWindow.Left - 2;
    int height = console.srWindow.Bottom - console.srWindow.Top + 1;

    initialize();

    int x = 0;
    int y = 0;
    int time;
    int current_mine;

    int gameState = START;

    char key = 0;
   

    while (1)
    {
        flip();

        clear();

        if (_kbhit())
        {
            key = _getch();

            if (key == -32 || key == 0)
            {
                key = _getch();
            }

            switch (key)
            {
            case UP:
                if (y > 0) y--;
                break;

            case DOWN:
                if (y < SIZE - 1) y++;
                break;

            case LEFT:
                if (gameState == START || gameState == OVER || gameState == CLEAR)
                {
                    menuindex = 1 - menuindex;
                }
                else if(gameState == GAME)
                {
                    if (x > 0) x -= 2;
                }
                break;

            case RIGHT:
                if (gameState == START || gameState == OVER || gameState == CLEAR)
                {
                    menuindex = 1 - menuindex;
                }
                else if (gameState == GAME)
                {
                    if (x < (SIZE - 1) * 2) x += 2;
                }
                break;
            case ENTER:
                if (gameState == START)
                {
                    if (menuindex == 1)
                    {
                        exit(0);
                    }
                    else if(menuindex == 0)
                    {
                        reset_board();
                        gameState = GAME;
                        Set_mine(SIZE, EASY);
                        make_number_map(SIZE, SIZE);
                        cellcount = 100 - EASY;
                    }
                }
                else if (gameState == GAME)
                {
                    cellcount--;
                    if (mine_board[y][x/2] == 9)
                    {
                        gameState = OVER;
                    }
                    else if (cellcount<=0)
                    {
                        gameState = CLEAR;
                    }
                    else
                    {
                        game_board[y][x/2] = 1;
                    } 
                }
                else if (gameState == OVER)
                {
                    if(menuindex == 1)
                    {
                        exit(0);
                    }
                    else if (menuindex == 0)
                    {
                        gameState = START;
                    }
                }
                else if (gameState == CLEAR)
                {
                    if (menuindex == 1)
                    {
                        exit(0);
                    }
                    else if (menuindex == 0)
                    {
                        gameState = START;
                    }
                }
                break;

            case 'f':
            case 'F': // 'F'와 'f' 모두 처리
                if (gameState == GAME)
                {
                    // 이미 열린 칸이면 깃발 못둠
                    if (game_board[y][x / 2] == 0)
                    {
                        // 깃발이 없다면 꽂고 꽂혀있다면 뽑음
                        flag_board[y][x / 2] = !flag_board[y][x / 2]; 
                    }
                }
                break;

            default: render(0, 0, "exception");
                break;
            }
        }

        if (gameState == START)
        {
            drawMenu();
            
        }
        else if(gameState == GAME)
        {
            drawBoard(x, y);
        }
        else if (gameState == OVER)
        {
            drawOver();
        }
        else if (gameState == CLEAR)
        {
            drawClear();
        }
    }

    release();

    return 0;
}