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
    bool floodFillMark;
    int x;
    int y;
}Cell_t;

typedef struct{
    Cell_t board[MAZE_SIZE][MAZE_SIZE];
    Mouse_t mouse;

}Maze_t;
void initMaze(Maze_t& maze);
void floodFill(Maze_t& maze);
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
    
    std::queue<Cell_t *> cellQueue;
    // Se cargan las primeras 4 direcciones de destino
    cellQueue.push(&(maze.board[7][7]));
    cellQueue.push(&(maze.board[7][8]));
    cellQueue.push(&(maze.board[8][7]));
    cellQueue.push(&(maze.board[8][8]));
    
    int floodfillAsignmentValue = 0;
    while (!cellQueue.empty()) {
        int i;
        for (i = 0; i < cellQueue.size(); i++) {
            Cell_t* pCell = cellQueue.front();
            pCell->mark = true;
            // Al recorrer siempre alrededor, se llega siempre de la manera mas corta al camino

            if (pCell->x + 1 < MAZE_SIZE && !maze.board[pCell->x + 1][pCell->y].mark && (pCell->walls)[right]) {
                cellQueue.push(&(maze.board[pCell->x + 1][pCell->y]));
                maze.board[pCell->x + 1][pCell->y].FloodFillmark = true;
                maze.board[pCell->x + 1][pCell->y].floodfillValue = floodfillAsignmentValue;
            }
            if (pCell->x - 1 > 0 && !maze.board[pCell->x - 1][pCell->y].mark && (pCell->walls)[left]) {
                cellQueue.push(&(maze.board[pCell->x - 1][pCell->y]));
                maze.board[pCell->x - 1][pCell->y].FloodFillmark = true;
                maze.board[pCell->x - 1][pCell->y].floodfillValue = floodfillAsignmentValue;
            }
            if (pCell->y + 1 < MAZE_SIZE && !maze.board[pCell->x][pCell->y + 1].mark && (pCell->walls)[up]) {
                cellQueue.push(&(maze.board[pCell->x][pCell->y + 1]);
                maze.board[pCell->x][pCell->y + 1].FloodFillmark = true;
                maze.board[pCell->x][pCell->y + 1].floodfillValue = floodfillAsignmentValue;
            }
            if (pCell->y - 1 > 0 && maze.board[pCell->x][pCell->y - 1].mark && (pCell->walls[down])) {
                cellQueue.push(&(maze.board[pCell->x][pCell->y - 1]));
                maze.board[pCell->x][pCell->y - 1].FloodFillmark = true;
                maze.board[pCell->x][pCell->y - 1].floodfillValue = floodfillAsignmentValue;
            }
        }

        floodfillAsignmentValue++;
    }
    
    

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
            maze.board[i][j].mark = false;
            maze.board[i][j].floodFillMark = false;
            maze.board[i][j].x = j;
            maze.board[i][j].y = i;
                maze.board[i][j].walls[up] = 0;
                maze.board[i][j].walls[down] = 0;
                maze.board[i][j].walls[left] = 0;
                maze.board[i][j].walls[right] = 0;
            }
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
     * orientation(2bits), left, front, right
     * 
     */
    unsigned char walls = 0b00000;
    if (API::wallLeft()) walls |= 0b100;
    if (API::wallFront()) walls |= 0b010;
    if (API::wallRight()) walls |= 0b001;

    
        
    }


}