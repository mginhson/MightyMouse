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
    bool mark;
}Cell_t;

typedef struct{
    Cell_t board[MAZE_SIZE][MAZE_SIZE];
    Mouse_t mouse;

}Maze_t;
void initMaze(Maze_t& maze);

int main(int argc, char* argv[]) {
    
    log("Running...");

  
    


            unsigned char wallAnalyzer = 0b0000;
            if (API::wallRight()) walls |= 0b001;
            if (API::wallFront()) walls |= 0b010;
            if (API::wallLeft() ) walls |= 0b100;

        }
    }

        
    
}
void floodFill(Maze_t & maze) {
    // Chequeo el valor de floodfill de las direcciones alrededor del mouse
    int mouseX = maze.mouse.x;
    int mouseY = maze.mouse.y;
    // Pared de frente
    
    

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

            maze.board[i][j].cellKind = emptyWall;
        }
    }
}

/**
 * @brief Updates the cell if needed 
 */
void updateCell (Maze_t& maze)
{
    //Just in case
    if (maze.board[maze.mouse.x][maze.mouse.y].mark == 1)
        return;
    
    maze.board[maze.mouse.x][maze.mouse.y].mark = 0;

    /**
     * bitwise, from left to right
     * left, front, right
     * 
     */
    unsigned char walls = 0b000;
    if (API::wallLeft()) walls |= 0b100;
    if (API::wallFront()) walls |= 0b010;
    if (API::wallRight()) walls |= 0b001;

    switch (maze.mouse.orientation)
    {
        case left:
        {
            var |= 
        }break;

        case right:
        {
            
        }break;

        case up:
        {

        }break;

        case down:
        {

        }break;

    }


}