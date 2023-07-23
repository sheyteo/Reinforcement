#pragma once

#include "pch.h"

const int GRID_SIZE = 25; // Size of the grid world
const int MAX_STEPS = 200; // Size of the grid world
const int NUM_ACTIONS = 4; // Number of possible actions (up, down, left, right)

enum Actions { UP, DOWN, LEFT, RIGHT };


// A simple Map with obstacles and a goal
// goal = reward
// obstacle = penalty
// just walking = small penalty
// going out of map(bounds) = small penalty

class Environment {
private:
    std::vector<std::vector<int>> grid;

public:
    Environment() {
        grid.resize(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));

        //// Place obstacles (1) and goal (2)
        grid[0][2] = 1;
        grid[1][2] = 1;
        grid[2][2] = 1;
        grid[3][2] = 1;
        grid[4][2] = 1;
        grid[5][2] = 1;
        grid[2][4] = 1;
        grid[3][4] = 1;
        grid[4][4] = 1;
        grid[6][6] = 1;
        grid[7][6] = 1;

        grid[3][8] = 1;
        grid[3][9] = 1;
        grid[3][10] = 1;
        grid[3][11] = 1;
        grid[10][5] = 1;
        grid[10][6] = 1;
        grid[10][7] = 1;
        grid[10][8] = 1;
        grid[15][10] = 1;
        grid[16][10] = 1;
        grid[17][10] = 1;
        grid[18][10] = 1;

        grid[8][14] = 1;
        grid[9][14] = 1;
        grid[10][14] = 1;
        grid[10][13] = 1;
        grid[11][13] = 1;
        grid[11][12] = 1;
        grid[12][12] = 1;
        grid[12][11] = 1;
        grid[13][11] = 1;
        grid[13][10] = 1;

        grid[20][5] = 1;
        grid[20][6] = 1;
        grid[20][7] = 1;
        grid[21][5] = 1;
        grid[21][6] = 1;
        grid[21][7] = 1;
        grid[22][5] = 1;
        grid[22][6] = 1;
        grid[22][7] = 1;

        grid[18][2] = 1;
        grid[18][3] = 1;
        grid[19][2] = 1;
        grid[19][3] = 1;

        grid[5][18] = 1;
        grid[5][19] = 1;
        grid[5][20] = 1;
        grid[6][18] = 1;
        grid[6][19] = 1;
        grid[6][20] = 1;
        grid[7][18] = 1;
        grid[7][19] = 1;
        grid[7][20] = 1;

        grid[15][15] = 1;
        grid[16][15] = 1;
        grid[17][15] = 1;
        grid[15][16] = 1;
        grid[16][16] = 1;
        grid[17][16] = 1;
        grid[15][17] = 1;
        grid[16][17] = 1;
        grid[17][17] = 1;

        grid[23][23] = 1;
        grid[23][24] = 1;
        grid[24][23] = 1;
        grid[24][24] = 1;

        grid[2][7] = 1;
        grid[2][8] = 1;
        grid[2][9] = 1;
        grid[2][10] = 1;
        grid[2][11] = 1;
        grid[2][12] = 1;
        grid[2][13] = 1;

        grid[11][4] = 1;
        grid[11][5] = 1;
        grid[12][4] = 1;
        grid[12][5] = 1;

        grid[14][15] = 1;
        grid[15][15] = 1;
        grid[16][15] = 1;
        grid[14][16] = 1;
        grid[16][16] = 1;
        grid[14][17] = 1;
        grid[15][17] = 1;
        grid[16][17] = 1;

        grid[21][2] = 1;
        grid[21][3] = 1;
        grid[21][4] = 1;
        grid[22][2] = 1;
        grid[22][3] = 1;
        grid[22][4] = 1;

        grid[7][21] = 1;
        grid[7][22] = 1;
        grid[8][21] = 1;
        grid[8][22] = 1;

        grid[18][20] = 1;
        grid[18][21] = 1;
        grid[18][22] = 1;
        grid[19][20] = 1;
        grid[19][22] = 1;
        grid[20][20] = 1;
        grid[20][21] = 1;
        grid[20][22] = 1;

        grid[9][9] = 1;
        grid[9][10] = 1;
        grid[9][11] = 1;
        grid[10][8] = 1;
        grid[10][9] = 1;
        grid[10][10] = 1;
        grid[10][11] = 1;
        grid[10][12] = 1;
        grid[11][9] = 1;
        grid[11][10] = 1;
        grid[11][11] = 1;

        grid[6][18] = 1;
        grid[6][19] = 1;
        grid[6][20] = 1;
        grid[7][18] = 1;
        grid[7][20] = 1;
        grid[8][18] = 1;
        grid[8][20] = 1;
        grid[9][18] = 1;
        grid[9][20] = 1;
        grid[10][18] = 1;
        grid[10][19] = 1;
        grid[10][20] = 1;

        grid[5][5] = 1;
        grid[5][6] = 1;
        grid[5][7] = 1;
        grid[6][5] = 1;
        grid[6][7] = 1;
        grid[7][5] = 1;
        grid[7][6] = 1;
        grid[7][7] = 1;

        grid[13][4] = 1;
        grid[13][5] = 1;
        grid[13][6] = 1;
        grid[13][7] = 1;
        grid[13][8] = 1;
        grid[14][5] = 1;
        grid[14][6] = 1;
        grid[14][7] = 1;

        grid[19][11] = 1;
        grid[19][12] = 1;
        grid[20][11] = 1;
        grid[20][12] = 1;

        grid[4][16] = 1;
        grid[5][16] = 1;
        grid[6][16] = 1;
        grid[7][16] = 1;
        grid[8][16] = 1;
        grid[9][16] = 1;
        grid[10][16] = 1;
        grid[11][16] = 1;
        grid[12][16] = 1;
        grid[13][16] = 1;

        grid[16][23] = 1;
        grid[16][24] = 1;
        grid[17][23] = 1;
        grid[17][24] = 1;

        grid[1][20] = 1;
        grid[2][20] = 1;
        grid[3][20] = 1;
        grid[4][20] = 1;
        grid[5][20] = 1;
        grid[6][20] = 1;
        grid[7][20] = 1;
        grid[8][20] = 1;
        grid[9][20] = 1;

        grid[17][4] = 1;
        grid[17][5] = 1;
        grid[17][6] = 1;
        grid[17][7] = 1;
        grid[18][4] = 1;
        grid[18][7] = 1;
        grid[19][4] = 1;
        grid[19][7] = 1;
        grid[20][4] = 1;
        grid[20][5] = 1;
        grid[20][6] = 1;
        grid[20][7] = 1;

        grid[22][19] = 1;
        grid[22][20] = 1;
        grid[23][19] = 1;
        grid[23][20] = 1;
        grid[24][19] = 1;
        grid[24][20] = 1;

        grid[3][2] = 1;
        grid[3][3] = 1;
        grid[3][4] = 1;
        grid[4][2] = 1;
        grid[4][4] = 1;
        grid[5][2] = 1;
        grid[5][4] = 1;

        grid[14][23] = 1;
        grid[14][24] = 1;
        grid[15][23] = 1;
        grid[15][24] = 1;

        grid[8][17] = 1;
        grid[17][19] = 1;
        grid[17][20] = 1;
        grid[17][18] = 1;


        grid[10][21] = 1;
        grid[11][21] = 1;
        grid[12][21] = 1;
        grid[13][21] = 1;

        grid[8][7] = 1;
        grid[9][7] = 1;

        grid[16][18] = 2; // New goal position
    }

    int getState(int x, int y) {
        return y * GRID_SIZE + x;
    }

    bool isTerminalState(int x, int y) {
        return grid[y][x] == 2;
    }

    bool isObstacle(int x, int y) {
        return grid[y][x] == 1;
    }

    int at(int x, int y)
    {
        return grid[y][x];
    }

    static void getPositionFromIndex(int index, int& x, int& y) {
        y = index / GRID_SIZE;
        x = index % GRID_SIZE;
    }

    static std::vector<double> oneHotEncoded(const int& x, const int& y) {
        std::vector<double> arr(GRID_SIZE * 2,0);
        arr[x] = 1;
        arr[GRID_SIZE + x] = 1;
        return arr;
    }

    double performAction(int& x, int& y, int action) {
        int newX = x;
        int newY = y;

        double outsideAction = 0; // determined by wether an action was usless in form of movement

        switch (action) {
        case UP:

            if (y == 0)
            {
                outsideAction -= 0.1; //Peanalty for going outside
            }

            newY = std::max(0, y - 1);
            break;
        case DOWN:

            if (y == GRID_SIZE - 1)
            {
                outsideAction -= 0.1; //Peanalty for going outside
            }

            newY = std::min(GRID_SIZE - 1, y + 1);
            break;
        case LEFT:

            if (x == 0)
            {
                outsideAction -= 0.1; //Peanalty for going outside
            }

            newX = std::max(0, x - 1);
            break;
        case RIGHT:

            if (x == GRID_SIZE - 1)
            {
                outsideAction -= 0.1; //Peanalty for going outside
            }

            newX = std::min(GRID_SIZE - 1, x + 1);
            break;
        default:
            break;
        }

        if (!isObstacle(newX, newY)) {
            x = newX;
            y = newY;
        }
        else
        {
            outsideAction -= 0.1; //Peanalty for hitting an obstacle
        }

        if (isTerminalState(x, y)) {
            return 100; // Reward for reaching the goal
        }
        else {
            //can remove outsideAction only expiriment
            return -0.1 +outsideAction;// Negative reward for moving
        }
    }

    void print(int x, int y)
    {
        for (size_t i = 0; i < grid[0].size() + 2; i++)
        {
            std::cout << "-";
        }
        std::cout << "\n";
        for (size_t i = 0; i < grid.size(); i++)
        {
            std::cout << "|";
            for (size_t j = 0; j < grid[i].size(); j++)
            {
                if (y == i && x == j)
                {
                    std::cout << "X";
                }
                else
                {
                    switch (grid[i][j])
                    {
                    case 0:
                        std::cout << " ";
                        break;
                    case 1:
                        std::cout << "+";
                        break;
                    case 2:
                        std::cout << "#";
                        break;
                    default:
                        break;
                    }
                }

            }
            std::cout << "|\n";
        }
        for (size_t i = 0; i < grid[0].size() + 2; i++)
        {
            std::cout << "-";
        }
        std::cout << "\n";
    }

    void printPath(const std::vector<std::pair<int, int>>& recentCoordinates)
    {
        for (size_t i = 0; i < grid[0].size() + 2; i++)
        {
            std::cout << "-";
        }
        std::cout << "\n";
        for (size_t i = 0; i < grid.size(); i++)
        {
            std::cout << "|";
            for (size_t j = 0; j < grid[i].size(); j++)
            {
                bool found = false;
                for (const auto& [x, y] : recentCoordinates)
                {
                    if (y == i && x == j)
                    {
                        std::cout << "%";
                        found = true;
                        break;
                    }
                }

                if (!found)
                {
                    switch (grid[i][j])
                    {
                    case 0:
                        std::cout << " ";
                        break;
                    case 1:
                        std::cout << "+";
                        break;
                    case 2:
                        std::cout << "#";
                        break;
                    default:
                        break;
                    }
                }

            }
            std::cout << "|\n";
        }
        for (size_t i = 0; i < grid[0].size() + 2; i++)
        {
            std::cout << "-";
        }
        std::cout << "\n";
    }
};