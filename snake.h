// 움직이는 맵 구성하는 SnakeGame 클래스의 헤더파일 (스네이크 머리, 몸통, 아이템들, 시간, 스코어 등을 나타냄)
#ifndef SNAKE_H
#define SNAKE_H

#define ROOM_X 23 // set room row size
#define ROOM_Y 30 // set room column size
#include <vector>
#include <map>
extern char room[ROOM_X][ROOM_Y];

struct Point
{
    int x, y;
};

struct items
{
    int x, y, time, dir;
};

class SnakeGame
{
public:
    SnakeGame();
    bool Run(int stage, int *all_score);

private:
    // void Initialize();
    bool HandleInput();
    void Update();
    void GrowthItem();
    void PoisonItem();
    void D_GrowthItem();
    void GateItem();
    bool IsGameOver();
    void Update_scoreboard();
    void input_wall();
    void new_wall();

    int width, height;
    std::vector<Point> snake;
    std::vector<items> Growth_items;
    std::vector<items> Poison_items;
    std::vector<items> D_Growth_items;
    std::vector<items> Gate_items;
    std::map<std::pair<int, int>, int> walls;
    int start_time;
    int direction;
    int Growth_time;
    int Poison_time;
    int D_Growth_time;
    int Gate_time;
    int Growth_cnt; // how much eat growth iteams
    int Poison_cnt; // how much eat poison_items
    int D_Growth_cnt;
    int gate_cnt;
    bool nextstage;
    bool gameOver;
    bool moving_gate;
    bool enter_gate;
    bool pass_gate;
    bool first_gate;
    int out_dir; // gate out_dir;
    int out_gate_last_x;
    int out_gate_last_y;
    int score;
    int new_wall_X;
    int new_wall_Y;
    int new_wall_time;
};

#endif