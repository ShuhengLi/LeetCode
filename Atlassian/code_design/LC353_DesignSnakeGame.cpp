/*
 * 353. Design Snake Game
 * Medium
 * [Atlassian Classic - Very Frequently Asked]
 *
 * Design a Snake game that is played on a device with screen size height x width.
 * The snake is initially positioned at the top left corner (0, 0) with a length of 1 unit.
 * You are given an array food where food[i] = (ri, ci) is the position of the food.
 * When the snake eats food, its length grows by 1 and the score increases by 1.
 * Each food appears one at a time on the screen. New food only appears after the previous
 * one is eaten. Food is guaranteed not to appear on the cell occupied by the snake.
 *
 * The snake can turn in 4 directions: "U", "D", "L", "R".
 * If the snake goes out of bounds or runs into its own body, the game is over.
 *
 * Atlassian variation: snake grows only on every Nth food eaten.
 *
 * Example:
 * Input: width=3, height=2, food=[[1,2],[0,1]]
 * ["SnakeGame","move","move","move","move","move"]
 * [[3,2,[[1,2],[0,1]]],["R"],["D"],["R"],["U"],["L"]]
 * Output: [null,0,0,1,1,2]
 */
#include <bits/stdc++.h>
using namespace std;

class SnakeGame {
    int width, height;
    vector<vector<int>> food;
    int foodIndex, score;
    deque<pair<int,int>> snake;    // body positions (head at front, tail at back)
    set<pair<int,int>> occupied;   // for O(1) collision check

public:
    SnakeGame(int width, int height, vector<vector<int>>& food)
        : width(width), height(height), food(food), foodIndex(0), score(0) {
        snake.push_front({0, 0});
        occupied.insert({0, 0});
    }

    // Returns score after move, or -1 if game over
    int move(string direction) {
        auto [row, col] = snake.front();
        int newRow = row, newCol = col;

        if (direction == "U") newRow--;
        else if (direction == "D") newRow++;
        else if (direction == "L") newCol--;
        else if (direction == "R") newCol++;

        // Check wall collision
        if (newRow < 0 || newRow >= height || newCol < 0 || newCol >= width)
            return -1;

        // Check if eating food
        bool ateFood = foodIndex < (int)food.size()
            && newRow == food[foodIndex][0]
            && newCol == food[foodIndex][1];

        if (!ateFood) {
            // Remove tail - snake moves forward
            occupied.erase(snake.back());
            snake.pop_back();
        } else {
            // Snake grows - don't remove tail
            score++;
            foodIndex++;
        }

        // Check self collision (must check AFTER removing tail for non-food moves)
        if (occupied.count({newRow, newCol}))
            return -1;

        // Add new head
        snake.push_front({newRow, newCol});
        occupied.insert({newRow, newCol});

        return score;
    }
};

/*
 * Atlassian Variation: Snake grows only every Nth food
 */
class SnakeGameVariation {
    int width, height, growEveryN, foodIndex, score, foodEatenCount;
    vector<vector<int>> food;
    deque<pair<int,int>> snake;
    set<pair<int,int>> occupied;

public:
    SnakeGameVariation(int width, int height, vector<vector<int>>& food, int growEveryN)
        : width(width), height(height), food(food), growEveryN(growEveryN),
          foodIndex(0), score(0), foodEatenCount(0) {
        snake.push_front({0, 0});
        occupied.insert({0, 0});
    }

    int move(string direction) {
        auto [row, col] = snake.front();
        int newRow = row, newCol = col;

        if (direction == "U") newRow--;
        else if (direction == "D") newRow++;
        else if (direction == "L") newCol--;
        else if (direction == "R") newCol++;

        if (newRow < 0 || newRow >= height || newCol < 0 || newCol >= width)
            return -1;

        bool ateFood = foodIndex < (int)food.size()
            && newRow == food[foodIndex][0]
            && newCol == food[foodIndex][1];

        bool shouldGrow = false;
        if (ateFood) {
            score++;
            foodIndex++;
            foodEatenCount++;
            shouldGrow = (foodEatenCount % growEveryN == 0);
        }

        if (!shouldGrow) {
            occupied.erase(snake.back());
            snake.pop_back();
        }

        if (occupied.count({newRow, newCol})) return -1;

        snake.push_front({newRow, newCol});
        occupied.insert({newRow, newCol});

        return score;
    }
};
