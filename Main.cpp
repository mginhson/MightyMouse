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



typedef enum{left,right,forward} Movement_t;

typedef std::vector<Movement_t> MovementStack_t;



void goBackToBeginning (MovementStack_t& movementStack);

typedef enum{

}CellKind_t;

typedef struct{
    unsigned int floodfillValue;

}Cell_t;

int main(int argc, char* argv[]) {
    
    log("Running...");
    
    MovementStack_t chain;
    
    while (true) {
        unsigned int x;
        
        for (x = 0; x < 30; x++)
        {
            if (!API::wallLeft()) 
            {
                API::turnLeft();
                chain.push_back(left);
            }

            while (API::wallFront()) 
            {
                API::turnRight();
                chain.push_back(right);
            }

            API::moveForward();
            chain.push_back(forward);
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
            case left:
            {
                API::turnRight();
            }break;

            case right:
            {
                API::turnLeft();
            }break;

            case forward:
            {
                API::moveForward();
            }break;
            
            default:
                break;
        }

        movementStack.pop_back();
    }
}