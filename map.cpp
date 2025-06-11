// 움직이지 않는 맵 설정(초기 맵 구성, 스테이지별 맵 구성, 다음스테이지,게임오버 메시지 구성 등)
#include <iostream>
#include <ncurses.h>
#include "snake.h"

void set_room(int stage)
{

    for (int i = 0; i < ROOM_X; i++)
    {
        for (int j = 0; j < ROOM_Y; j++)
        {
            room[i][j] = ' ';
        }
        room[i][0] = '1';
        room[i][ROOM_Y - 1] = '1';
    }

    for (int i = 0; i < ROOM_Y; i++)
    {
        room[0][i] = '1';
        room[ROOM_X - 1][i] = '1';
    }

    room[0][0] = '2';
    room[ROOM_X - 1][0] = '2';
    room[0][ROOM_Y - 1] = '2';
    room[ROOM_X - 1][ROOM_Y - 1] = '2';

    switch (stage)
    {
    case 1: // set stage1 room
        mvprintw(1, COLS / 2 + 2, "Stage 1");
        break;
    case 2: // set stage2 room
        mvprintw(1, COLS / 2 + 2, "Stage 2");
        for (int i = 10; i < 15; i++)
        {
            room[ROOM_X / 4][i] = '1';
        }
        for (int i = 10; i < 15; i++)
        {
            room[i][ROOM_X / 4] = '1';
        }
        break;
    case 3:
        mvprintw(1, COLS / 2 + 2, "Stage 3");
        for (int i = 4; i < 8; i++)
        {
            room[i][ROOM_Y / 5] = '1';
        }
        for (int i = 4; i < 8; i++)
        {
            room[i][ROOM_Y / 5 * 4 - 1] = '1';
        }
        for (int i = 10; i < 20; i++)
        {
            room[ROOM_X / 4 * 3 + 1][i] = '1';
        }
        room[ROOM_X / 4 * 3][10] = '1';
        room[ROOM_X / 4 * 3][19] = '1';
        break;
    case 4:
        mvprintw(1, COLS / 2 + 2, "Stage 4");
        for (int i = 10; i < 15; i++)
        {
            room[ROOM_X / 4][i] = '1';
        }
        for (int i = 4; i < 8; i++)
        {
            room[i][ROOM_Y / 5] = '1';
        }
        for (int i = 22; i < 26; i++)
        {
            room[19][i] = '1';
        }
        for (int i = 12; i < 16; i++)
        {
            room[i][9] = '1';
        }
        for (int i = 9; i < 24; i++)
        {
            room[16][i] = '1';
        }

        break;
    }
}

void print_room(char room[ROOM_X][ROOM_Y])
{
    for (int i = 0; i < ROOM_X; i++)
    {
        for (int j = 0; j < ROOM_Y; j++)
        {
            const char temp = room[i][j];
            attron(COLOR_PAIR(1));
            if (room[i][j] == '1' || room[i][j] == '2')
            {
                mvprintw(1 + i, 1 + j, "%c", room[i][j]);
            }
            attroff(COLOR_PAIR(1));
        }
    }

    int vertical_x = COLS / 2;
    for (int y = 0; y < LINES; ++y)
    { // draw middle line in room
        mvaddch(y, vertical_x, '|');
    }
}

void print_result(bool nextstage, int all_score, int stage)
{
    int height = 8;
    int width = 26;
    int x = (LINES - height) / 2;
    int y = (COLS - width) / 8;

    WINDOW *win = newwin(height, width, x, y);
    box(win, 0, 0);

    if (nextstage)
    {
        mvwprintw(win, height / 2 - 3, width / 2 - 6, "<next stage>");
        mvwprintw(win, height / 2, 1, "Enter: next stage");
        mvwprintw(win, height / 2 + 2, 1, "Total Score : %d", all_score);
    }
    else
    {
        mvwprintw(win, height / 2 - 3, width / 2 - 5, "<GameOver>");
        mvwprintw(win, height / 2, 1, "R: game restart");
        mvwprintw(win, height / 2 + 2, 1, "Total Score : %d", all_score);
    }
    mvwprintw(win, height / 2 - 2, 1, "Press the key...");
    mvwprintw(win, height / 2 + 1, 1, "ESC: close game");

    refresh();
    wrefresh(win);
}