#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include "API.h"
#include "vector"
#include "queue"
void log(const std::string& text) {
    std::cerr << text << std::endl;
}



typedef enum{movement_left,movement_right,movement_forward} Movement_t;

typedef std::vector<Movement_t> MovementStack_t;

typedef enum {
    up,
    right,
    down,
    left
}Orientation_t;

typedef struct{
    int x;
    int y;
    Orientation_t orientation;
}Mouse_t;




typedef enum{
    leftWall, 
    upWall,
    rightWall,
    downWall,
    leftDownWall,
    rightDownWall,
    rightUpWall,
    leftUpWall,
    leftRigthWall,
    upDownWall,
    leftRightDownWall,
    rigthUpDownWall,
    leftRigthUpWall,
    leftUpDownWall,
    emptyWall,
    CompleteWall
}CellKind_t;
#define MAZE_SIZE 16
typedef struct{
    unsigned int floodfillValue;
    CellKind_t cellKind;
}Cell_t;

typedef struct{
    Cell_t board[MAZE_SIZE][MAZE_SIZE];
    Mouse_t mouse;
}Maze_t;
void initMaze(Maze_t& maze) {
    maze.mouse.x = 0;
    maze.mouse.y = 0;

    for (int i = 0; i < MAZE_SIZE: i++) {
        for (int j = 0; h < MAZE_SIZE; j++) {
            //Primer cuadrante
            if (i >= 8 && j >= 8) maze.board[i][j] = i - 8 + j - 8;
            //Segundo cuadrante
            else if (i <= 7 && j >= 8) maze.board[i][j] = 7 - i + j - 8;
            //Tercer cuadrante
            else if (i <= 7 && j <= 7) maze.board[i][j] = 7 - i + 7 - j;
            //Cuarto cuadrante
            else if (i = > 8 && j <= 7) maze.board[i][j] = i - 8 + 7 - j;
        }
    }
}

int main(int argc, char* argv[]) {
    
    log("Running...");
    
    MovementStack_t chain;
    Mouse_t mouse;
    while (true) {
        unsigned int x;
        
        for (x = 0; x < 30; x++)
        {
            if (!API::wallLeft()) 
            {
                API::turnLeft();
                chain.push_back(movement_left);
            }

        while (!hasFinished(maze))
        {
            /**
             * First, we analyze the cell. If it isn't the wallEmpty kind,
             * it has already been analyzed.
             */


            unsigned char wallAnalyzer = 0b0000;
            if (API::wallRight()) walls |= 0b001;
            if (API::wallFront()) walls |= 0b010;
            if (API::wallLeft() ) walls |= 0b100;

            switch (walls)
            {
                case 0b000:
            }
            
        }
    }


    


        
    
}

