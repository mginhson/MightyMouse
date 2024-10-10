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


void initMaze (Maze_t& maze);
void hasFinished (Maze_t& maze);

int main(int argc, char* argv[]) 
{
    
    log("Running...");

  
    

        
    
}
    void initMaze(Maze_t & maze) {
        maze.mouse.x = 0;
        maze.mouse.y = 0;
        int i;
        int j;
        for ( i = 0; i < MAZE_SIZE; i++) {
            for (j = 0; j < MAZE_SIZE; j++) {
                //Primer cuadrante
                if (i >= 8 && j >= 8) maze.board[i][j].floodfillValue = i - 8 + j - 8;
                //Segundo cuadrante
                else if (i <= 7 && j >= 8) maze.board[i][j].floodfillValue = 7 - i + j - 8;
                //Tercer cuadrante
                else if (i <= 7 && j <= 7) maze.board[i][j].floodfillValue = 7 - i + 7 - j;
                //Cuarto cuadrante
                else if (i = > 8 && j <= 7) maze.board[i][j].floodfillValue = i - 8 + 7 - j;
            }
        }
    }

bool hasFinished (Maze_t& maze)
{
    return 1;
}