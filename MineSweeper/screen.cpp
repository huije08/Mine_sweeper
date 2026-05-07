#include <stdio.h>
#include <conio.h>
#include <windows.h>

#include "Screen.h"

#define SIZE 8

#define WHITE 7
#define BLUE 9
int menuindex = 0;
HANDLE screen[2];
int index = 0;
int size = sizeof(screen) / sizeof(screen[0]);

int dx[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
int dy[8] = { -1,-1,-1,  0, 0,  1, 1, 1 };

// 지뢰 위치 
int mine_board[SIZE][SIZE]{ 0 };

// 숫자 칸
int game_board[SIZE][SIZE]{ 0 };

// 깃발 위치
int flag_board[SIZE][SIZE]{ 0 };

void initialize()
{
    CONSOLE_CURSOR_INFO cursor;
    cursor.bVisible = FALSE;
    // for 축소
    for (int i = 0; i < 2; i++)
    {
        screen[i] = CreateConsoleScreenBuffer
        (
            GENERIC_READ | GENERIC_WRITE,
            0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL
        );
        SetConsoleCursorInfo(screen[i], &cursor);
    }
}

void flip()
{
    SetConsoleActiveScreenBuffer(screen[index]);
    index = 1 - index;
}

void clear()
{
    COORD position = { 0, 0 };

    DWORD dword;

    CONSOLE_SCREEN_BUFFER_INFO buffer;

    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(console, &buffer);

    int width = buffer.srWindow.Right - buffer.srWindow.Left + 1;
    int height = buffer.srWindow.Bottom - buffer.srWindow.Top + 1;

    FillConsoleOutputCharacter(screen[index], ' ', width * height, position, &dword);
}

void release()
{
    for (int i = 0; i < size; i++)
    {
        CloseHandle(screen[i]);
    }
}

void render(int x, int y, const char* character)
{
    DWORD dword;
    COORD position = { x, y };

    SetConsoleCursorPosition(screen[index], position);
    WriteFile(screen[index], character, strlen(character), &dword, NULL);
}


void drawBoard(int X, int Y)
{
    for (int y = 0; y < SIZE; y++)
    {
        for (int x = 0; x < SIZE; x++)
        {
            if (x * 2 == X && y == Y)
            {
                SetConsoleTextAttribute(screen[index], BLUE);
            }
            else
            {
                SetConsoleTextAttribute(screen[index], WHITE);
            }

            if (game_board[y][x]== 1)
            {
                switch (mine_board[y][x])
                {
                case 0:
                    render(x * 2, y, "⒪"); 
                case 1:
                    render(x * 2, y, "⑴");
                    break;
                case 2:
                    render(x * 2, y, "⑵");
                    break;
                case 3:
                    render(x * 2, y, "⑶");
                    break;
                case 4:
                    render(x * 2, y, "⑷");
                    break;
                case 5:
                    render(x * 2, y, "⑸");
                    break;
                case 6:
                    render(x * 2, y, "⑹");
                    break;
                case 7:
                    render(x * 2, y, "⑺");
                    break;
                case 8:
                    render(x * 2, y, "⑻");
                    break;
                case 9:
                    render(x * 2, y, "★");
                    break;
                default:
                    break;
                }
            }
            else if (game_board[y][x] == 0)
            {
                if(flag_board[y][x] == 1)
                {
                    render(x * 2, y, "☆");
                }
                else
                {
                    render(x * 2, y, "▣");
                }
               // render(x * 2, y, "▣");
            }
            
        }
    }
}

void drawMenu()
{
    render(0, 0, " __  __ ___       _____    ");
    render(0, 1, "|  \\/  |_ _|_ __ | ____|");
    render(0, 2, "| |\\/| || || '_ \\|  _| ");
    render(0, 3, "| |  | || || | | | |___ ");
    render(0, 4, "|_|__|_|___|_| |_|_____|__       _____ ____  ");
    render(0, 5, "/ ___|_      _| ____| ____|_ __ | ____|  _ \\ ");
    render(0, 6, "\\___ \\ \\ /\\ / /  _| |  _| | '_ \\|  _| | |_) |");
    render(0, 7, " ___) \\ V  V /| |___| |___| |_) | |___|  _ < ");
    render(0, 8, "|____/ \\_/\\_/ |_____|_____| .__/|_____|_| \\_\\");
    render(0, 9, "                          |_|                ");
    if (menuindex == 0)
    {
        render(10, 11, "> START");
        render(19, 11, "  EXIT");
    }
    else if (menuindex == 1)
    {
        render(10, 11, "  START");
        render(19, 11, "> EXIT");
    }

}

void drawOver()
{
    render(0, 0, "  ____    _    __  __ _____ ");
    render(0, 1, " / ___|  / \\  |  \\/  | ____|");
    render(0, 2, "| |  _  / _ \\ | |\\/| |  _|  ");
    render(0, 3, "| |_| |/ ___ \\| |  | | |___ ");
    render(0, 4, " \\____/_/   \\_\\_|__|_|_____|");
    render(0, 5, " / _ \\ \\   / / ____|  _ \\   ");
    render(0, 6, "| | | \\ \\ / /|  _| | |_) |  ");
    render(0, 7, "| |_| |\\ V / | |___|  _ <   ");
    render(0, 8, " \\___/  \\_/  |_____|_| \\_\\  ");

    if (menuindex == 0)
    {
        render(10, 10, "> RETRY");
        render(19, 10, "  EXIT");
    }
    else if (menuindex == 1)
    {
        render(10, 10, "  RETRY");
        render(19, 10, "> EXIT");
    }

}


void Set_mine(int size, int count)
{
    int m = 0;

    while (m < count)
    {
        int x = rand() % size;
        int y = rand() % size;

        // 이미 지뢰면 다시
        if (mine_board[y][x] == 9)
            continue;

        mine_board[y][x] = 9; // 💣 지뢰만 설치
        m++;
    }
}

void reset_board()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int k = 0; k < SIZE; k++)
        {
            mine_board[i][k] = 0;
            game_board[i][k] = 0;
            flag_board[i][k] = 0;
        }
    }
}

void calculateNumbers()
{
    int dx[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    int dy[8] = { -1,-1,-1,  0, 0,  1, 1, 1 };

    for (int y = 0; y < SIZE; y++)
    {
        for (int x = 0; x < SIZE; x++)
        {
            if (mine_board[y][x] == 9)
                continue;

            int count = 0;

            for (int i = 0; i < 8; i++)
            {
                int nx = x + dx[i];
                int ny = y + dy[i];

                if (nx >= 0 && nx < SIZE && ny >= 0 && ny < SIZE)
                {
                    if (mine_board[ny][nx] == 9)
                        count++;
                }
            }

            mine_board[y][x] = count;
        }
    }
}

