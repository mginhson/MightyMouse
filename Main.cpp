#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <chrono>
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

#define MAZE_SIZE 16

typedef struct{
    unsigned int floodfillValue;
    bool walls[4];
    bool mark;
    bool floodfillMark;
    int x;
    int y;
}Cell_t;

typedef struct{
    Cell_t board[MAZE_SIZE][MAZE_SIZE];
    Mouse_t mouse;

}Maze_t;

void initMaze (Maze_t& maze);
void floodFill (Maze_t& maze);
bool hasFinished (Maze_t& maze);
void updateCell (Maze_t& maze);
void displayFloodfill (Maze_t& maze);
void firstRun (Maze_t& maze);
void secondRun (Maze_t& maze);
void thirdRun (Maze_t& maze);
void goBackToBeginning (Maze_t& maze);

int main(int argc, char* argv[]) 
{
    auto startTime = std::chrono::steady_clock::now();
    Maze_t maze;
    int triesLeft = 3;    
    initMaze(maze);

    while (triesLeft)
    {
        switch(triesLeft--)
        {
            case 3:
            {
               secondRun(maze); 
            }break;

            case 2:
            {
                secondRun(maze);
            }break;

            case 1:
            {
                secondRun(maze);
            }break;

            default:
                break;

        }
        auto endTime = std::chrono::steady_clock::now();
    }
    /*
    while(true)
    {
        updateCell(maze);
        floodFill(maze);
        displayFloodfill(maze);
        Orientation_t bestOrientation = maze.mouse.orientation;
        unsigned int value = ~0; //max value
        unsigned int i;
        
        if (maze.board[maze.mouse.x][maze.mouse.y].walls[up] == 0 &&
            maze.board[maze.mouse.x][maze.mouse.y].floodfillValue < value)
        {
            value = maze.board[maze.mouse.x][maze.mouse.y].floodfillValue;
            bestOrientation = up;
            log("Updated best orientation");
        }
        
        if (maze.board[maze.mouse.x][maze.mouse.y].walls[down] == 0 &&
            maze.board[maze.mouse.x][maze.mouse.y].floodfillValue < value)
        {
            value = maze.board[maze.mouse.x][maze.mouse.y].floodfillValue;
            bestOrientation = down;
            log("Updated best orientation");
        }

        if (maze.board[maze.mouse.x][maze.mouse.y].walls[left] == 0 &&
            maze.board[maze.mouse.x][maze.mouse.y].floodfillValue < value)
        {
            value = maze.board[maze.mouse.x][maze.mouse.y].floodfillValue;
            bestOrientation = left;
            log("Updated best orientation");
        }

        if (maze.board[maze.mouse.x][maze.mouse.y].walls[right] == 0 &&
            maze.board[maze.mouse.x][maze.mouse.y].floodfillValue < value)
        {
            value = maze.board[maze.mouse.x][maze.mouse.y].floodfillValue;
            bestOrientation = right;
            log("Updated best orientation");
        }


        while(maze.mouse.orientation != bestOrientation)
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
        
    }
    */

        
    
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


            maze.board[i][j].mark = false;
            maze.board[i][j].floodfillMark = false;
            maze.board[i][j].x = j;
            maze.board[i][j].y = i;
                maze.board[i][j].walls[up] = 0;
                maze.board[i][j].walls[down] = 0;
                maze.board[i][j].walls[left] = 0;
                maze.board[i][j].walls[right] = 0;

                maze.board[i][j].mark = 0;
            }
        }

        maze.board[0][0].walls[down] = 1;
        API::setWall(0,0,'s');
        

    }



void firstRun(Maze_t& maze)
{
    while (!hasFinished(maze)) 
    {
        updateCell(maze);
        if (!API::wallLeft()) {
            API::turnLeft();
            switch(maze.mouse.orientation)
            {
                case up: maze.mouse.orientation = left; break;
                case right: maze.mouse.orientation = up; break;
                case down: maze.mouse.orientation = right; break;
                case left: maze.mouse.orientation = down; break;
                default: break;
            }
        }
        while (API::wallFront()) {
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
    
    goBackToBeginning(maze);
    
    return;
}

void secondRun(Maze_t& maze)
{
    while(!hasFinished(maze))
    {
        updateCell(maze);
        floodFill(maze);
        displayFloodfill(maze);
        Orientation_t bestOrientation = maze.mouse.orientation;
        unsigned int value = ~0; //max value
        unsigned int i;
        
        if (maze.board[maze.mouse.x][maze.mouse.y].walls[up] == 0 &&
            maze.board[maze.mouse.x][maze.mouse.y].floodfillValue < value)
        {
            value = maze.board[maze.mouse.x][maze.mouse.y].floodfillValue;
            bestOrientation = up;
            log("Updated best orientation");
        }
        
        if (maze.board[maze.mouse.x][maze.mouse.y].walls[down] == 0 &&
            maze.board[maze.mouse.x][maze.mouse.y].floodfillValue < value)
        {
            value = maze.board[maze.mouse.x][maze.mouse.y].floodfillValue;
            bestOrientation = down;
            log("Updated best orientation");
        }

        if (maze.board[maze.mouse.x][maze.mouse.y].walls[left] == 0 &&
            maze.board[maze.mouse.x][maze.mouse.y].floodfillValue < value)
        {
            value = maze.board[maze.mouse.x][maze.mouse.y].floodfillValue;
            bestOrientation = left;
            log("Updated best orientation");
        }

        if (maze.board[maze.mouse.x][maze.mouse.y].walls[right] == 0 &&
            maze.board[maze.mouse.x][maze.mouse.y].floodfillValue < value)
        {
            value = maze.board[maze.mouse.x][maze.mouse.y].floodfillValue;
            bestOrientation = right;
            log("Updated best orientation");
        }


        while(maze.mouse.orientation != bestOrientation)
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

    goBackToBeginning(maze);
}

void thirdRun(Maze_t& maze)
{

}

void goBackToBeginning(Maze_t& maze)
{

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

void floodFill(Maze_t& maze) {
    // Chequeo el valor de floodfill de las direcciones alrededor del mouse

    std::queue<Cell_t*> cellQueue;
    // Se cargan las primeras 4 direcciones de destino
    cellQueue.push(&(maze.board[7][7]));
    cellQueue.push(&(maze.board[7][8]));
    cellQueue.push(&(maze.board[8][7]));
    cellQueue.push(&(maze.board[8][8]));

    (maze.board[7][7]).floodfillValue = 0;
    (maze.board[7][8]).floodfillValue = 0;
    (maze.board[8][7]).floodfillValue = 0;
    (maze.board[8][8]).floodfillValue = 0;

    (maze.board[7][7]).floodfillMark = true;
    (maze.board[7][8]).floodfillMark = true;
    (maze.board[8][7]).floodfillMark = true;
    (maze.board[8][8]).floodfillMark = true;
    
    int floodfillAsignmentValue = 1;
    // Mientras que la lista este vacia
    while (!cellQueue.empty()) {
        // Obtenemos el tamaño de la lista
        int queueSize = cellQueue.size();
        int i;
        // Itero sobre todos los elementos cargados de la lista
        for (i = 0; i < queueSize; i++) { 
            // Obtengo el primer elemento de la cola y lo marco como visto
            //log("Bucleando");
            Cell_t* pCell = cellQueue.front();
            //pCell->floodfillMark = true;
            // Al recorrer siempre alrededor, se llega siempre de la manera mas corta al camino
            if (pCell->x + 1 < MAZE_SIZE && !maze.board[pCell->x + 1][pCell->y].floodfillMark && !(pCell->walls)[right]) {
                cellQueue.push(&(maze.board[pCell->x + 1][pCell->y]));
                maze.board[pCell->x + 1][pCell->y].floodfillMark = true;
                maze.board[pCell->x + 1][pCell->y].floodfillValue = floodfillAsignmentValue;
            }
            if (pCell->x - 1 > 0 && !maze.board[pCell->x - 1][pCell->y].floodfillMark && !(pCell->walls)[left]) {
                cellQueue.push(&(maze.board[pCell->x - 1][pCell->y]));
                maze.board[pCell->x - 1][pCell->y].floodfillMark = true;
                maze.board[pCell->x - 1][pCell->y].floodfillValue = floodfillAsignmentValue;
            }
            if (pCell->y + 1 < MAZE_SIZE && !maze.board[pCell->x][pCell->y + 1].floodfillMark && !(pCell->walls)[up]) {
                cellQueue.push(&(maze.board[pCell->x][pCell->y + 1]));
                maze.board[pCell->x][pCell->y + 1].floodfillMark = true;
                maze.board[pCell->x][pCell->y + 1].floodfillValue = floodfillAsignmentValue;
            }
            if (pCell->y - 1 > 0 && !maze.board[pCell->x][pCell->y - 1].floodfillMark && !(pCell->walls[down])) {
                cellQueue.push(&(maze.board[pCell->x][pCell->y - 1]));
                maze.board[pCell->x][pCell->y - 1].floodfillMark = true;
                maze.board[pCell->x][pCell->y - 1].floodfillValue = floodfillAsignmentValue;
            }
            cellQueue.pop();
        }
        floodfillAsignmentValue++;
    }
    for (int i = 0; i < MAZE_SIZE; i++)
    {
        for (int j = 0; j < MAZE_SIZE; j++) {
            maze.board[i][j].floodfillMark = false;
        }
    }
}
void displayFloodfill(Maze_t& maze) {
    int i;
    int j;
    for (i = 0; i < MAZE_SIZE; i++) {
        for (j = 0; j < MAZE_SIZE; j++) {
            API::setText(i, j, std::to_string(maze.board[i][j].floodfillValue));
        }
    }
}   


bool hasFinished(Maze_t& maze)
{
    if (maze.mouse.x < 7 || maze.mouse.x > 8)
        return 0;
    if (maze.mouse.y < 7 || maze.mouse.y > 8)
        return 0;
    
    return 1;
}