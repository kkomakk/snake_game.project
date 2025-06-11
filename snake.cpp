// 움직이는 맵을 구성하는 snakegame 구성
#include "snake.h"
#include <ncurses.h>
#include <cstdlib>
#include <ctime>

char room[ROOM_X][ROOM_Y];

SnakeGame::SnakeGame()
{
    width = 20;
    height = 20;
    direction = KEY_RIGHT;
    gameOver = false;
    nextstage = false;
    moving_gate = false;
    enter_gate = false;
    pass_gate = false;
    first_gate = false;
    Growth_time = time(NULL);
    Poison_time = time(NULL);
    D_Growth_time = time(NULL);
    Gate_time = time(NULL);
    start_time = time(NULL);
    Growth_cnt = 0;
    Poison_cnt = 0;
    D_Growth_cnt = 0;
    gate_cnt = 0;
    score = 0;
    out_dir = 0;
    out_gate_last_x = 0;
    out_gate_last_y = 0;
    new_wall_X = 3;
    new_wall_Y = ROOM_Y / 2;
    new_wall_time = time(NULL);
    snake.push_back({width / 2, height / 2}); // set snake start position
    snake.push_back({width / 2 + 1, height / 2});
    snake.push_back({width / 2 + 2, height / 2});
    walls.clear();
}

bool SnakeGame::HandleInput()
{
    int key = getch();
    switch (key)
    {
    case KEY_UP:
        if (direction == KEY_DOWN)
        {
            gameOver = true;
            mvprintw(0, 0, "                                ");
            mvprintw(0, 0, "Move Opposite side");
            return false;
        }
        else
        {
            direction = KEY_UP;
        }
        break;
    case KEY_DOWN:
        if (direction == KEY_UP)
        {
            gameOver = true;
            mvprintw(0, 0, "                                ");
            mvprintw(0, 0, "Move Opposite side");
            return false;
        }
        else
        {
            direction = KEY_DOWN;
        }
        break;
    case KEY_LEFT:
        if (direction == KEY_RIGHT)
        {
            gameOver = true;
            mvprintw(0, 0, "                                ");
            mvprintw(0, 0, "Move Opposite side");
            return false;
        }
        else
        {
            direction = KEY_LEFT;
        }
        break;
    case KEY_RIGHT:
        if (direction == KEY_LEFT)
        {
            gameOver = true;
            mvprintw(0, 0, "                                ");
            mvprintw(0, 0, "Move Opposite side");
            return false;
        }
        else
        {
            direction = KEY_RIGHT;
        }
        break;
    }
    return true;
}

void SnakeGame::Update()
{
    int x = snake[0].x;
    int y = snake[0].y;

    int be_x = x;
    int be_y = y;

    switch (direction)
    {
    case KEY_UP:
        y--;
        break;
    case KEY_DOWN:
        y++;
        break;
    case KEY_LEFT:
        x--;
        break;
    case KEY_RIGHT:
        x++;
        break;
    }

    // snake.insert(snake.begin(), {x, y});
    // mvprintw(y, x, "#"); // move snake

    if (room[y - 1][x - 1] == '1')
    { // snake collision wall
        mvprintw(0, 0, "                                ");
        mvprintw(0, 0, "wall crash");
        gameOver = true; // restart game & endgame
    }

    if (room[y - 1][x - 1] == '+')
    { // snake collision GrowthItem
        mvprintw(0, 0, "                                ");
        mvprintw(0, 0, "Message: Plus item");
        snake.insert(snake.end(), {snake.back().x, snake.back().y}); // insert plus tail
        room[y - 1][x - 1] = ' ';                                    // renew room property
        int erase_pos;
        for (int i = 0; i < Growth_items.size(); i++)
        { // find item in Growth_items
            if (Growth_items[i].x == y && Growth_items[i].y == x)
            {
                erase_pos = i;
                break;
            }
        }
        Growth_items.erase(Growth_items.begin() + erase_pos);

        Growth_cnt += 1;
        score += 1;
    }

    if (room[y - 1][x - 1] == '-')
    { // snake collision PoisionItem
        mvprintw(0, 0, "                                ");
        mvprintw(0, 0, "Message: Minus item");
        room[y - 1][x - 1] = ' '; // renew room property
        int erase_pos;
        for (int i = 0; i < Poison_items.size(); i++)
        { // find item in Poison_items
            if (Poison_items[i].x == y && Poison_items[i].y == x)
            {
                erase_pos = i;
                break;
            }
        }
        Poison_items.erase(Poison_items.begin() + erase_pos);

        mvprintw(snake.back().y, snake.back().x, " "); // remove tail
        room[snake.back().y - 1][snake.back().x - 1] = ' ';
        snake.pop_back(); // remove tail

        if (snake.size() < 3)
        { // if less than 3 size snake
            gameOver = true;
            mvprintw(0, 0, "                                ");
            mvprintw(0, 0, "size under 3");
            return;
        }

        Poison_cnt += 1;
        score -= 1;
    }

    if (room[y - 1][x - 1] == 'D')
    { // snake collision PoisionItem
        mvprintw(0, 0, "                                ");
        mvprintw(0, 0, "Message: Double grow");
        snake.insert(snake.end(), {snake.back().x, snake.back().y}); // insert plus tail 1
        snake.insert(snake.end(), {snake.back().x, snake.back().y}); // insert plus tail 2
        room[y - 1][x - 1] = ' ';                                    // renew room property
        int erase_pos;
        for (int i = 0; i < D_Growth_items.size(); i++)
        { // find item in Poison_items
            if (D_Growth_items[i].x == y && D_Growth_items[i].y == x)
            {
                erase_pos = i;
                break;
            }
        }
        D_Growth_items.erase(D_Growth_items.begin() + erase_pos);

        D_Growth_cnt += 1;
        score += 3;
    }

    if (room[y - 1][x - 1] == 'G')
    { // snake collision Gate
        moving_gate = true;

        if (Gate_items[0].x == y && Gate_items[0].y == x)
        {
            first_gate = true;
            y = Gate_items[1].x; // move to second gate
            x = Gate_items[1].y;
            out_dir = Gate_items[1].dir;
        }
        else
        {
            y = Gate_items[0].x; // move to first gate
            x = Gate_items[0].y;
            out_dir = Gate_items[0].dir;
        }

        if (!enter_gate)
        {
            enter_gate = true;
            switch (out_dir)
            {
            case 0: // out up wall
                direction = KEY_DOWN;
                y++;
                out_gate_last_y = -1;
                break;
            case 1: // out right wall
                direction = KEY_LEFT;
                x--;
                out_gate_last_x = 1;
                break;
            case 2: // out down wall
                direction = KEY_UP;
                y--;
                out_gate_last_y = 1;
                break;
            case 3: // out left wall
                direction = KEY_RIGHT;
                x++;
                out_gate_last_x = -1;
                break;
            case 4: // out inside wall
                switch (direction)
                {
                case KEY_DOWN:
                    if (room[y][x - 1] != '1')
                    { // maintain dir
                        y++;
                        out_gate_last_y = -1;
                        break;
                    }
                    if (room[y - 1][x - 2] != '1')
                    { // turn clock
                        direction = KEY_LEFT;
                        x--;
                        out_gate_last_x = 1;
                        break;
                    }
                    if (room[y - 1][x] != '1')
                    { // turn o'clock
                        direction = KEY_RIGHT;
                        x++;
                        out_gate_last_x = -1;
                        break;
                    }
                    if (room[y - 2][x - 1] != '1')
                    { // turn back
                        direction = KEY_UP;
                        y--;
                        out_gate_last_y = 1;
                        break;
                    }
                case KEY_UP:
                    if (room[y - 2][x - 1] != '1')
                    { // maintain dir
                        y--;
                        out_gate_last_y = 1;
                        break;
                    }
                    if (room[y - 1][x] != '1')
                    { // turn clock
                        direction = KEY_RIGHT;
                        x++;
                        out_gate_last_x = -1;
                        break;
                    }
                    if (room[y - 1][x - 2] != '1')
                    { // turn o'clock
                        direction = KEY_LEFT;
                        x--;
                        out_gate_last_x = 1;
                        break;
                    }
                    if (room[y][x - 1] != '1')
                    { // turn back
                        direction = KEY_DOWN;
                        y++;
                        out_gate_last_y = -1;
                        break;
                    }
                case KEY_RIGHT:
                    if (room[y - 1][x] != '1')
                    { // maintain dir
                        x++;
                        out_gate_last_x = -1;
                        break;
                    }
                    if (room[y][x - 1] != '1')
                    { // turn clock
                        direction = KEY_DOWN;
                        y++;
                        out_gate_last_y = -1;
                        break;
                    }
                    if (room[y - 2][x - 1] != '1')
                    { // turn o'clock
                        direction = KEY_UP;
                        y--;
                        out_gate_last_y = 1;
                        break;
                    }
                    if (room[y - 1][x - 2] != '1')
                    { // turn back
                        direction = KEY_LEFT;
                        x--;
                        out_gate_last_x = 1;
                        break;
                    }
                case KEY_LEFT:
                    if (room[y - 1][x - 2] != '1')
                    { // maintain dir
                        x--;
                        out_gate_last_x = 1;
                        break;
                    }
                    if (room[y - 2][x - 1] != '1')
                    { // turn clock
                        direction = KEY_UP;
                        y--;
                        out_gate_last_y = 1;
                        break;
                    }
                    if (room[y][x - 1] != '1')
                    { // turn o'clock
                        direction = KEY_DOWN;
                        y++;
                        out_gate_last_y = -1;
                        break;
                    }
                    if (room[y - 1][x] != '1')
                    { // turn back
                        direction = KEY_RIGHT;
                        x++;
                        out_gate_last_x = -1;
                        break;
                    }
                    break;
                }
            }
        }
        mvprintw(0, 0, "                                ");
        mvprintw(0, 0, "Message: Gate Moving..");
        gate_cnt += 1;
        score += 5;
    }

    snake.insert(snake.begin(), {x, y});

    attron(COLOR_PAIR(2));
    mvprintw(y, x, "3"); // move snake head
    attroff(COLOR_PAIR(2));

    attron(COLOR_PAIR(1));
    for (int i = 1; i < snake.size(); ++i)
    {
        mvprintw(snake[i].y, snake[i].x, "4"); // move snake body
    }
    attroff(COLOR_PAIR(1));

    if (room[snake.back().y - 1][snake.back().x - 1] != '1')
    {
        mvprintw(snake.back().y, snake.back().x, " ");      // room recovery
        room[snake.back().y - 1][snake.back().x - 1] = ' '; // renew room property(tail moved snake)
    }
    snake.pop_back();

    if (room[y - 1][x - 1] == '3' || room[y - 1][x - 1] == '4')
    { // snake collision snake
        gameOver = true;
        mvprintw(0, 0, "                                ");
        mvprintw(0, 0, "self die");
        return;
    }

    room[y - 1][x - 1] = '#'; // renew room property(head moved snake)

    if (moving_gate)
    {
        // renew room property
        // mvprintw(26,0, "snake: %d, %d gate: %d, %d ", snake.back().y,snake.back().x,Gate_items[0].x,Gate_items[0].y);
        // mvprintw(27,0, "snake: %d, %d gate: %d, %d ", snake.back().y,snake.back().x,Gate_items[1].x,Gate_items[1].y);
        int snake_tail_x = snake.back().y + out_gate_last_y;
        int snake_tail_y = snake.back().x + out_gate_last_x;

        if (first_gate)
        {
            if ((snake_tail_x == Gate_items[1].x && snake_tail_y == Gate_items[1].y))
            {
                moving_gate = false;
                pass_gate = true;
            }
        }
        else
        {
            if (snake_tail_x == Gate_items[0].x && snake_tail_y == Gate_items[0].y)
            {
                moving_gate = false;
                pass_gate = true;
            }
        }

        if (!moving_gate)
        {
            mvprintw(0, 0, "                                ");
            mvprintw(25, 0, "Message: Gate finished!");
            room[Gate_items[0].x - 1][Gate_items[0].y - 1] = '1';
            attron(COLOR_PAIR(1));
            mvprintw(Gate_items[0].x, Gate_items[0].y, "1");
            room[Gate_items[1].x - 1][Gate_items[1].y - 1] = '1';
            mvprintw(Gate_items[1].x, Gate_items[1].y, "1");
            attroff(COLOR_PAIR(1));
            Gate_items.clear();
            enter_gate = false;
            pass_gate = false;
            first_gate = false;
            out_gate_last_x = 0;
            out_gate_last_y = 0;
        }
    }
    refresh();
}

void SnakeGame::GrowthItem()
{
    int now_time = time(NULL);

    // mvprintw(0, 0, "b_time %d n_time %d G_item:%ld P_item:%ld", Growth_time, now_time, Growth_items.size(), Poison_items.size());
    if (Growth_items.size() > 0)
    { // created item in 10 second & destory
        if (now_time - Growth_items[0].time > 10)
        {
            room[Growth_items[0].x - 1][Growth_items[0].y - 1] = ' ';
            mvprintw(Growth_items[0].x, Growth_items[0].y, " ");
            Growth_items.erase(Growth_items.begin());
        }
    }

    if ((now_time - Growth_time) > 2 && (D_Growth_items.size() + Growth_items.size() + Poison_items.size()) < 3)
    {                                    // 2 second create and lower than 3 .
        srand((unsigned int)time(NULL)); // set random now time
        int item_x = 2 + rand() % (ROOM_X - 2);
        int item_y = 2 + rand() % (ROOM_Y - 2);
        if (room[item_x - 1][item_y - 1] != ' ')
        { // if snake or other item exist

            return;
        }
        attron(COLOR_PAIR(3));
        mvprintw(item_x, item_y, "+");
        attroff(COLOR_PAIR(3));
        room[item_x - 1][item_y - 1] = '+'; // renew room property to '+'
        mvprintw(0, 0, "                                ");
        mvprintw(0, 0, "Message: Plus item created");
        Growth_time = now_time;
        Growth_items.push_back({item_x, item_y, now_time});
    }
}

void SnakeGame::PoisonItem()
{
    int now_time = time(NULL);

    if (Poison_items.size() > 0)
    { // created item in 10 second & destory
        if (now_time - Poison_items[0].time > 10)
        {
            room[Poison_items[0].x - 1][Poison_items[0].y - 1] = ' ';
            mvprintw(Poison_items[0].x, Poison_items[0].y, " ");
            Poison_items.erase(Poison_items.begin());
        }
    }

    if ((now_time - Poison_time) > 7 && (D_Growth_items.size() + Growth_items.size() + Poison_items.size()) < 3)
    {                                    // 5 second create and lower than 3 .
        srand((unsigned int)time(NULL)); // set random now time
        int item_x = 2 + rand() % (ROOM_X - 2);
        int item_y = 2 + rand() % (ROOM_Y - 2);
        if (room[item_x - 1][item_y - 1] != ' ')
        { // if snake or other item exist
            return;
        }

        attron(COLOR_PAIR(5));
        mvprintw(item_x, item_y, "-");
        attroff(COLOR_PAIR(5));

        room[item_x - 1][item_y - 1] = '-';
        mvprintw(0, 0, "                                ");
        mvprintw(0, 0, "Message: Minus item created");
        Poison_time = now_time;
        Poison_items.push_back({item_x, item_y, now_time});
    }
}

void SnakeGame::D_GrowthItem()
{
    int now_time = time(NULL);

    if (D_Growth_items.size() > 0)
    { // created item in 10 second & destory
        if (now_time - D_Growth_items[0].time > 10)
        {
            room[D_Growth_items[0].x - 1][D_Growth_items[0].y - 1] = ' ';
            mvprintw(D_Growth_items[0].x, D_Growth_items[0].y, " ");
            D_Growth_items.erase(D_Growth_items.begin());
        }
    }

    if ((now_time - D_Growth_time) > 7 && (D_Growth_items.size() + Growth_items.size() + Poison_items.size()) < 3)
    {                                    // 7 second create and lower than 3 .
        srand((unsigned int)time(NULL)); // set random now time
        int item_x = 2 + rand() % (ROOM_X - 2);
        int item_y = 2 + rand() % (ROOM_Y - 2);
        if (room[item_x - 1][item_y - 1] != ' ')
        { // if snake or other item exist
            return;
        }
        attron(COLOR_PAIR(4));
        mvprintw(item_x, item_y, "D");
        attroff(COLOR_PAIR(4));
        room[item_x - 1][item_y - 1] = 'D'; // renew room property to '+'
        mvprintw(0, 0, "                                ");
        mvprintw(0, 0, "Message: Double Grow item");
        D_Growth_time = now_time;
        D_Growth_items.push_back({item_x, item_y, now_time});
    }
}

void SnakeGame::GateItem()
{
    int now_time = time(NULL);

    if (Gate_items.size() > 0 && !moving_gate)
    {
        if (now_time - Gate_items[0].time > 10)
        {

            int gate1_x = Gate_items[0].x;
            int gate1_y = Gate_items[0].y;
            int gate2_x = Gate_items.back().x;
            int gate2_y = Gate_items.back().y;

            room[gate1_x - 1][gate1_y - 1] = '1';
            room[gate2_x - 1][gate2_y - 1] = '1';
            attron(COLOR_PAIR(1));
            mvprintw(gate1_x, gate1_y, "1");
            mvprintw(gate2_x, gate2_y, "1"); // restore wall
            attroff(COLOR_PAIR(1));

            Gate_items.clear();
        }
    }

    if ((now_time - Gate_time) > 10 && (Gate_items.size() == 0))
    {
        srand((unsigned int)time(NULL)); // set random seed

        int gate1_num = rand() % walls.size();
        int gate1_x;
        int gate1_y;
        int gate1_dir;
        int start = 0;
        for (const auto &[key, value] : walls)
        {
            if (start == gate1_num)
            {
                gate1_x = key.first;
                gate1_y = key.second;
                gate1_dir = value;
            }
            start++;
        }

        int gate2_num = rand() % walls.size();
        while (true)
        {
            gate2_num = rand() % walls.size();
            if (gate2_num != gate1_num)
            {
                break;
            }
        }
        int gate2_x;
        int gate2_y;
        int gate2_dir;
        start = 0;
        for (const auto &[key, value] : walls)
        {
            if (start == gate2_num)
            {
                gate2_x = key.first;
                gate2_y = key.second;
                gate2_dir = value;
            }
            start++;
        }

        attron(COLOR_PAIR(6));
        mvprintw(gate1_x, gate1_y, "G");
        mvprintw(gate2_x, gate2_y, "G");
        attroff(COLOR_PAIR(6));

        room[gate1_x - 1][gate1_y - 1] = 'G';
        room[gate2_x - 1][gate2_y - 1] = 'G';
        mvprintw(0, 0, "                                ");
        mvprintw(0, 0, "Message: Gate created");
        Gate_time = now_time;
        Gate_items.push_back({gate1_x, gate1_y, Gate_time, gate1_dir});
        Gate_items.push_back({gate2_x, gate2_y, Gate_time, gate2_dir});
    }
}

bool SnakeGame::IsGameOver()
{
    return gameOver;
}

void SnakeGame::Update_scoreboard()
{
    int vertical_x = COLS / 2 + 2;

    int now_time = time(NULL);
    mvprintw(3, vertical_x, "time: %ds, Every 10s - New wall", now_time - start_time);
    mvprintw(5, vertical_x, "Score  Board");
    mvprintw(6, vertical_x, "B : (%ld) / (15)", snake.size());
    mvprintw(7, vertical_x, "+ : (%d)", Growth_cnt);
    mvprintw(8, vertical_x, "- : (%d)", Poison_cnt);
    mvprintw(9, vertical_x, "D : (%d)", D_Growth_cnt);
    mvprintw(10, vertical_x, "G : (%d)", gate_cnt);

    mvprintw(12, vertical_x, "Stage Score : (%d)", score);

    mvprintw(14, vertical_x, "Mission  Board");
    mvprintw(15, vertical_x, "B : 10 (%c)", snake.size() > 9 ? 'V' : ' ');
    mvprintw(16, vertical_x, "+ : 5 (%c)", Growth_cnt > 4 ? 'V' : ' ');
    mvprintw(17, vertical_x, "- : 2 (%c)", Poison_cnt > 1 ? 'V' : ' ');
    mvprintw(18, vertical_x, "D : 2 (%c)", D_Growth_cnt > 1 ? 'V' : ' ');
    mvprintw(19, vertical_x, "G : 1 (%c)", gate_cnt > 0 ? 'V' : ' ');

    if (snake.size() > 9 && Growth_cnt > 4 && Poison_cnt > 1 && D_Growth_cnt > 1 && gate_cnt > 0)
    {
        nextstage = true;
    }
}

void SnakeGame::input_wall()
{ // update wall vector

    for (int i = 1; i < ROOM_Y - 1; i++)
    { // input up_wall
        walls.insert(std::make_pair(std::make_pair(1, i + 1), 0));
    }
    for (int i = 1; i < ROOM_Y - 1; i++)
    { // input down_wall
        walls.insert(std::make_pair(std::make_pair(ROOM_X, i + 1), 2));
    }
    for (int i = 1; i < ROOM_X - 1; i++)
    { // input right_wall
        walls.insert(std::make_pair(std::make_pair(i + 1, ROOM_Y), 1));
    }
    for (int i = 1; i < ROOM_X - 1; i++)
    { // input left_wall
        walls.insert(std::make_pair(std::make_pair(i + 1, 1), 3));
    }

    for (int i = 1; i < ROOM_X - 1; i++)
    { // input inside_wall
        for (int j = 1; j < ROOM_Y - 1; j++)
        {
            if (room[i][j] == '1')
            {
                walls.insert(std::make_pair(std::make_pair(i + 1, j + 1), 4));
            }
        }
    }
}

void SnakeGame::new_wall()
{
    int now_time = time(NULL);

    if ((now_time - new_wall_time) > 9 && new_wall_X < ROOM_X)
    {
        if (room[new_wall_X - 1][new_wall_Y - 1] == '1')
        { // if snake or other item exist
            new_wall_X += 2;
            return;
        }
        if (room[new_wall_X - 1][new_wall_Y - 1] != ' ')
        { // if snake or other item exist
            return;
        }
        attron(COLOR_PAIR(1));
        mvprintw(new_wall_X, new_wall_Y, "1");
        attroff(COLOR_PAIR(1));
        room[new_wall_X - 1][new_wall_Y - 1] = '1';
        walls.insert(std::make_pair(std::make_pair(new_wall_X, new_wall_Y), 4));
        mvprintw(0, 0, "                                ");
        mvprintw(0, 0, "Message: new wall created");
        new_wall_time = now_time;
        new_wall_X += 2;
    }
}

bool SnakeGame::Run(int stage, int *all_score)
{
    input_wall();
    while (!IsGameOver())
    {
        if (nextstage)
        {
            break;
        }
        if (!HandleInput())
        {
            break;
        } // if go opposite gameover
        Update();
        Update_scoreboard();
        GrowthItem();
        PoisonItem();
        D_GrowthItem();
        GateItem();
        new_wall();
        napms(240 - snake.size() * 10); // if snake size is longer speed up 2-(2)
    }
    *all_score += score;
    return nextstage;
    // mvprintw(0,0, "GAME OVER!!"); //show game over massage
}