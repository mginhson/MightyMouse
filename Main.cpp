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

typedef struct{
    unsigned int floodfillValue;
    CellKind_t cellKind;
}Cell_t;

typedef struct{
    Cell_t board[16][16];
    Mouse_t mouse;
}Maze_t;


void goBackToBeginning (Maze_t& maze);
bool hasFinished (Maze_t& maze);


int main(int argc, char* argv[]) 
{
    
    log("Running...");
    Maze_t maze;
    int triesLeft = 3;

    while(triesLeft)
    {
        /**
         * At this point, the mouse is supposed to be at {0,0}.
         */

        while (!hasFinished(maze))
        {

        }
    }


    


        
    
}

