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
    up = 0,
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
    bool walls[4];
    bool mark;
}Cell_t;

typedef struct{
    Cell_t board[MAZE_SIZE][MAZE_SIZE];
    Mouse_t mouse;
}Maze_t;


void initMaze (Maze_t& maze);
bool hasFinished (Maze_t& maze);
void updateCell (Maze_t& maze);


int main(int argc, char* argv[]) 
{
    
    log("Running...");

    Maze_t maze;
    
    initMaze(maze);

    while(true)
    {
        updateCell(maze);
        while(API::wallFront())
        {
            API::turnRight();
            switch(maze.mouse.orientation)
            {
                case up: maze.mouse.orientation = right; break;
                case right: maze.mouse.orientation = down; break;
                case down: maze.mouse.orientation = left; break;
                case left: maze.mouse.orientation = up; break;
                default: break;
            }
        }
        switch(maze.mouse.orientation)
        {
            case up: maze.mouse.y += 1; break;
            case down: maze.mouse.y -= 1; break;
            case right: maze.mouse.x +=1; break;
            case left: maze.mouse.x -= 1; break;
        }
        API::moveForward();
    }

        
    
}


    void initMaze(Maze_t & maze) {
        maze.mouse.x = 0;
        maze.mouse.y = 0;
        maze.mouse.orientation = up;
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
                else if (i >= 8 && j <= 7) maze.board[i][j].floodfillValue = i - 8 + 7 - j;

                maze.board[i][j].walls[up] = 0;
                maze.board[i][j].walls[down] = 0;
                maze.board[i][j].walls[left] = 0;
                maze.board[i][j].walls[right] = 0;

                maze.board[i][j].mark = 0;
            }
        }
    }


bool hasFinished (Maze_t& maze)
{
    return 1;
}

/**
 * @brief Updates the cell if needed 
 */
void updateCell (Maze_t& maze)
{
    //Just in case
    if (maze.board[maze.mouse.x][maze.mouse.y].mark == 1)
        return;
    log("Updating new cell");
    maze.board[maze.mouse.x][maze.mouse.y].mark = 1;
    
    switch (maze.mouse.orientation)
    {
        /**
         * -------
         * |  ^  |
         * |     |
         */
        case up:
        {
             
            maze.board[maze.mouse.x][maze.mouse.y].walls[down] = 0;

            if (API::wallFront())     
                maze.board[maze.mouse.x][maze.mouse.y].walls[up] = 1;
            
            if (API::wallLeft()) 
                maze.board[maze.mouse.x][maze.mouse.y].walls[left] = 1;
            
            if (API::wallRight()) 
                maze.board[maze.mouse.x][maze.mouse.y].walls[right] = 1;

        }break;

        /**
         * -------
         * |    
         * |  <   
         * -------
         */
        case left:
        {
            
            maze.board[maze.mouse.x][maze.mouse.y].walls[right] = 0;
            if (API::wallFront()) 
                maze.board[maze.mouse.x][maze.mouse.y].walls[left] = 1;
            
            if (API::wallLeft()) 
                maze.board[maze.mouse.x][maze.mouse.y].walls[down] = 1;
            
            if (API::wallRight()) 
                maze.board[maze.mouse.x][maze.mouse.y].walls[up] = 1;
                
        }break;

        /**
         * -------
         *    >  |
         *       |
         * -------
         */
        case right:
        {
            maze.board[maze.mouse.x][maze.mouse.y].walls[left] = 0;

            if (API::wallFront()) 
                maze.board[maze.mouse.x][maze.mouse.y].walls[right] = 1;
            
            if (API::wallLeft()) 
                maze.board[maze.mouse.x][maze.mouse.y].walls[up] = 1;
            
            if (API::wallRight()) 
                maze.board[maze.mouse.x][maze.mouse.y].walls[down] = 1;
                
        }break;

        /**
         * 
         * |     |
         * |  v  |
         * -------
         */
        case down:
        {
            maze.board[maze.mouse.x][maze.mouse.y].walls[up] = 0;
            if (API::wallFront()) 
                maze.board[maze.mouse.x][maze.mouse.y].walls[down] = 1;
            
            if (API::wallLeft()) 
                maze.board[maze.mouse.x][maze.mouse.y].walls[right] = 1;
            
            if (API::wallRight()) 
                maze.board[maze.mouse.x][maze.mouse.y].walls[left] = 1;
            
        }break;

        default:
            break;
    }

    //Now, we set the walls (Visual help on the emulator)
    
    if (maze.board[maze.mouse.x][maze.mouse.y].walls[up]) 
    {
        log("Found a wall: up");
        API::setWall(maze.mouse.x, maze.mouse.y, 'n');
    }   
    
    if (maze.board[maze.mouse.x][maze.mouse.y].walls[down]) 
    {
        log("Found a wall: down");
        API::setWall(maze.mouse.x, maze.mouse.y, 's');
    }

    if (maze.board[maze.mouse.x][maze.mouse.y].walls[right]) 
    {    
        log("Found a wall: right");
        API::setWall(maze.mouse.x, maze.mouse.y, 'e');
    }

    if (maze.board[maze.mouse.x][maze.mouse.y].walls[left]) 
    {
        log("Found a wall: left");
        API::setWall(maze.mouse.x, maze.mouse.y, 'w');
    }

}