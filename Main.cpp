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

enum orientations {
    up,
    right,
    down,
    left
};

typedef struct{
    int x;
    int y;
    int Orient;
}Mouse_t;


void goBackToBeginning (MovementStack_t& movementStack);
bool hasFinished (int x, int y);

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
    unsigned char availableNeighbor[4];
}Cell_t;

typedef struct{
    Cell_t board[16][16];
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

            while (API::wallFront()) 
            {
                API::turnRight();
                chain.push_back(movement_right);
            }

            API::moveForward();
            chain.push_back(movement_forward);
        }

        goBackToBeginning(chain);
        
    }
}


void goBackToBeginning (MovementStack_t& movementStack)
{
    
    /**
     * We need to retrace back our steps, the opposite way. So, we first
     * turn around, then read the chain sequence backwards.
     */

    API::turnLeft();
    API::turnLeft();

    while ( movementStack.size() > 0 )
    {
            
        switch (movementStack.back())
        {
            case movement_left:
            {
                API::turnRight();
            }break;

            case movement_right:
            {
                API::turnLeft();
            }break;

            case movement_forward:
            {
                API::moveForward();
            }break;
            
            default:
                break;
        }

        movementStack.pop_back();
    }
}