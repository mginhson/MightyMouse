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

        }
    }


    


        
    
}

