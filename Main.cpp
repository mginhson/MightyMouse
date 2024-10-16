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

#define ABS(x) ((x) > 0 ? (x) : -(x))



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
#define TIME_LIMIT (60*4)

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
void floodFill (Maze_t& maze, std::vector<Cell_t*> & initialCells);
void pathTo (Maze_t& maze, std::vector<Cell_t*>& targets, bool (*cutoff)(void));
bool hasReachedTarget (Maze_t& maze, std::vector<Cell_t*>& targets);
bool hasFinished (Maze_t& maze);
void updateCell (Maze_t& maze);
void displayFloodfill (Maze_t& maze);
void firstRun (Maze_t& maze);
void secondRun (Maze_t& maze);
void thirdRun (Maze_t& maze);
void goBackToBeginning (Maze_t& maze);
void mouseForward (Maze_t& maze);
void mouseLeft (Maze_t& maze);
void mouseRight (Maze_t& maze);
void cleanAllWalls(Maze_t& maze);
void setAllWalls(Maze_t& maze);
unsigned int timeElapsedInSeconds(void);
bool firstRunCutoff(void);

std::chrono::_V2::steady_clock::time_point globalStartTime = std::chrono::steady_clock::now();

int main(int argc, char* argv[]) 
{
    
    Maze_t maze;
    int triesLeft = 3;    
    initMaze(maze);

    while (triesLeft)
    {
        switch(triesLeft--)
        {
            case 1:
            {
               thirdRun(maze); 
            }break;

            case 2:
            {
                secondRun(maze);
            }break;

            case 3:
            {
                firstRun(maze);
            }break;

            default:
                break;

        }
        

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


            maze.board[i][j].mark = false;
            maze.board[i][j].floodfillMark = false;
            maze.board[i][j].x = i;
            maze.board[i][j].y = j;
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
    std::vector<Cell_t*>vec = {&maze.board[0][MAZE_SIZE - 1]};
    pathTo(maze, vec, &firstRunCutoff);
    vec[0] = &maze.board[MAZE_SIZE - 1][MAZE_SIZE - 1];
    pathTo(maze, vec, &firstRunCutoff);
    vec[0] = &maze.board[MAZE_SIZE - 1][0];
    pathTo(maze, vec, &firstRunCutoff);

    vec.resize(4);
    vec[0] = &maze.board[7][7];
    vec[1] = &maze.board[7][8];
    vec[2] = &maze.board[8][7];
    vec[3] = &maze.board[8][8];

    pathTo(maze,vec, NULL);


    goBackToBeginning(maze);

    return;
}

void secondRun(Maze_t& maze)
{
    std::vector<Cell_t*> vec = 
    {
        &maze.board[7][7],
        &maze.board[7][8],
        &maze.board[8][7],
        &maze.board[8][8],
    };

    pathTo(maze,vec, NULL);
    goBackToBeginning(maze);
    
}

void thirdRun(Maze_t& maze)
{
    std::vector<Cell_t*> vec = 
    {
        &maze.board[7][7],
        &maze.board[7][8],
        &maze.board[8][7],
        &maze.board[8][8],
    };

    pathTo(maze,vec, NULL);
    
}

void goBackToBeginning(Maze_t& maze)
{
    std::vector<Cell_t*> vec2 = {&maze.board[0][0]};

    pathTo(maze,vec2, NULL);
}


void pathTo (Maze_t& maze, std::vector<Cell_t*>& targets, bool (*cutoff)(void))
{
    while (!hasReachedTarget(maze, targets) && (cutoff == NULL || !cutoff()))
    {
        updateCell(maze);
        floodFill(maze, targets);
        displayFloodfill(maze);
        
        Orientation_t bestOrientation = maze.mouse.orientation;
        unsigned int value = ~0; //max value
        unsigned int i;
        
        if (maze.board[maze.mouse.x][maze.mouse.y].walls[up] == 0 &&
            maze.board[maze.mouse.x][maze.mouse.y + 1].floodfillValue < value)
        {
            value = maze.board[maze.mouse.x][maze.mouse.y + 1].floodfillValue;
            bestOrientation = up;   
        }
        
        if (maze.board[maze.mouse.x][maze.mouse.y].walls[down] == 0 &&
            maze.board[maze.mouse.x][maze.mouse.y - 1].floodfillValue < value)
        {
            value = maze.board[maze.mouse.x][maze.mouse.y - 1].floodfillValue;
            bestOrientation = down;
        }

        if (maze.board[maze.mouse.x][maze.mouse.y].walls[left] == 0 &&
            maze.board[maze.mouse.x - 1][maze.mouse.y].floodfillValue < value)
        {
            value = maze.board[maze.mouse.x - 1][maze.mouse.y].floodfillValue;
            bestOrientation = left;
        }

        if (maze.board[maze.mouse.x][maze.mouse.y].walls[right] == 0 &&
            maze.board[maze.mouse.x + 1][maze.mouse.y].floodfillValue < value)
        {
            value = maze.board[maze.mouse.x + 1][maze.mouse.y].floodfillValue;
            bestOrientation = right;
        }

        if (((maze.mouse.orientation+1)%4) == bestOrientation)
        {
            while(maze.mouse.orientation != bestOrientation)
                mouseRight(maze);
        }
        else
        {
            while(maze.mouse.orientation != bestOrientation)
                mouseLeft(maze);
        }
        
        
    
        mouseForward(maze);
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
            if (maze.mouse.x != 0 || maze.mouse.y != 0)
                maze.board[maze.mouse.x][maze.mouse.y].walls[down] = 0;

            if (API::wallFront())     
            {    
                maze.board[maze.mouse.x][maze.mouse.y].walls[up] = 1;
                if (maze.mouse.y+1 < MAZE_SIZE)
                    maze.board[maze.mouse.x][maze.mouse.y + 1].walls[down] = 1;
            }
            
            if (API::wallLeft()) 
            {
                maze.board[maze.mouse.x][maze.mouse.y].walls[left] = 1;
                if (maze.mouse.x - 1 >= 0)
                    maze.board[maze.mouse.x - 1][maze.mouse.y].walls[right] = 1;
            }    
            
            if (API::wallRight()) 
            {
                maze.board[maze.mouse.x][maze.mouse.y].walls[right] = 1;
                if (maze.mouse.x + 1 < MAZE_SIZE)
                    maze.board[maze.mouse.x + 1][maze.mouse.y].walls[left] = 1;
            }

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
            {
                maze.board[maze.mouse.x][maze.mouse.y].walls[left] = 1;
                if (maze.mouse.x - 1 >= 0)
                    maze.board[maze.mouse.x - 1][maze.mouse.y].walls[right] = 1;
            }
            
            if (API::wallLeft()) 
            {
                maze.board[maze.mouse.x][maze.mouse.y].walls[down] = 1;
                if (maze.mouse.y - 1 >= 0)
                    maze.board[maze.mouse.x][maze.mouse.y - 1].walls[up] = 1;
            }   
            
            if (API::wallRight()) 
            {    
                maze.board[maze.mouse.x][maze.mouse.y].walls[up] = 1;
                if (maze.mouse.y + 1 < MAZE_SIZE)
                    maze.board[maze.mouse.x][maze.mouse.y + 1].walls[down] = 1;
            }
                
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
            {
                maze.board[maze.mouse.x][maze.mouse.y].walls[right] = 1;
                if (maze.mouse.x + 1 < MAZE_SIZE)
                    maze.board[maze.mouse.x + 1][maze.mouse.y].walls[left] = 1;
            }   
            
            if (API::wallLeft()) 
            {    
                maze.board[maze.mouse.x][maze.mouse.y].walls[up] = 1;
                if (maze.mouse.y + 1 < MAZE_SIZE)
                    maze.board[maze.mouse.x][maze.mouse.y + 1].walls[down] = 1;
            }
            
            if (API::wallRight()) 
            {
                maze.board[maze.mouse.x][maze.mouse.y].walls[down] = 1;
                if (maze.mouse.y - 1 >= 0)
                    maze.board[maze.mouse.x][maze.mouse.y - 1].walls[up] = 1;
            }
                
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
            {
                maze.board[maze.mouse.x][maze.mouse.y].walls[down] = 1;
                if (maze.mouse.y - 1 >= 0)
                    maze.board[maze.mouse.x][maze.mouse.y - 1].walls[up] = 1;
            }
            
            if (API::wallLeft()) 
            {
                maze.board[maze.mouse.x][maze.mouse.y].walls[right] = 1;
                if (maze.mouse.x + 1 < MAZE_SIZE)
                    maze.board[maze.mouse.x + 1][maze.mouse.y].walls[left] = 1;
            }
            
            if (API::wallRight()) 
            {
                maze.board[maze.mouse.x][maze.mouse.y].walls[left] = 1;
                if (maze.mouse.x - 1 >= 0)
                    maze.board[maze.mouse.x - 1][maze.mouse.y].walls[right] = 1;
            }
            
        }break;

        default:
            break;
    }

    //Now, we set the walls (Visual help on the emulator)
    
    if (maze.board[maze.mouse.x][maze.mouse.y].walls[up]) 
        API::setWall(maze.mouse.x, maze.mouse.y, 'n');  
    
    if (maze.board[maze.mouse.x][maze.mouse.y].walls[down]) 
        API::setWall(maze.mouse.x, maze.mouse.y, 's');
    

    if (maze.board[maze.mouse.x][maze.mouse.y].walls[right]) 
        API::setWall(maze.mouse.x, maze.mouse.y, 'e');
    

    if (maze.board[maze.mouse.x][maze.mouse.y].walls[left]) 
        API::setWall(maze.mouse.x, maze.mouse.y, 'w');
    

}



void setAllWalls(Maze_t & maze){
    int i;
    int j;

    for (i = 0; i < MAZE_SIZE; i++) {
        for (j = 0; j < MAZE_SIZE; j++) {
            if (maze.board[i][j].walls[up]) API::setWall(i, j ,'n');
            if (maze.board[i][j].walls[right]) API::setWall(i, j, 'e');
            if (maze.board[i][j].walls[left]) API::setWall(i, j,'w');
            if (maze.board[i][j].walls[down]) API::setWall(i, j, 's');
        }
    }
}
void cleanAllWalls(Maze_t& maze) {
    int i;
    int j;
    for (i = 0; i < MAZE_SIZE; i++) {
        for (j = 0; j < MAZE_SIZE; j++) {
            API::clearWall(i, j, 'n');
            API::clearWall(i, j, 'e');
            API::clearWall(i, j, 'w');
            API::clearWall(i, j, 's');
        }
    }
}
void floodFill(Maze_t& maze, std::vector<Cell_t*> &initialCells) {
    // Chequeo el valor de floodfill de las direcciones alrededor del mouse

    std::queue<Cell_t*> cellQueue;
    // Se cargan las primeras 4 direcciones de destino
    for (auto cell : initialCells) {
        cellQueue.push(cell);
        cell->floodfillValue = 0;
        cell->floodfillMark = true;
    }
    /*
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
    */
    int floodfillAsignmentValue = 1;
    // Mientras que la lista este vacia

    while (!cellQueue.empty()) {
        // Obtenemos el tama�o de la lista
        int queueSize = cellQueue.size();
        
        int i;
        // Itero sobre todos los elementos cargados de la lista
        for (i = 0; i < queueSize; i++) { 
            // Obtengo el primer elemento de la cola y lo marco como visto
            //log("Bucleando");
            Cell_t* pCell = cellQueue.front();
            //pCell->floodfillMark = true;
            // Al recorrer siempre alrededor, se llega siempre de la manera mas corta al camino
            if (pCell->x + 1 < MAZE_SIZE && !maze.board[pCell->x + 1][pCell->y].floodfillMark && !(maze.board[pCell->x][pCell->y].walls[right])) {
                
                cellQueue.push(&(maze.board[pCell->x + 1][pCell->y]));
                maze.board[pCell->x + 1][pCell->y].floodfillMark = true;
                maze.board[pCell->x + 1][pCell->y].floodfillValue = floodfillAsignmentValue;
                
            }
            if (pCell->x - 1 >= 0 && !maze.board[pCell->x - 1][pCell->y].floodfillMark && !(maze.board[pCell->x][pCell->y].walls[left])) {
                cellQueue.push(&(maze.board[pCell->x - 1][pCell->y]));
                maze.board[pCell->x - 1][pCell->y].floodfillMark = true;
                maze.board[pCell->x - 1][pCell->y].floodfillValue = floodfillAsignmentValue;
              
            }
            if (pCell->y + 1 < MAZE_SIZE && !maze.board[pCell->x][pCell->y + 1].floodfillMark && !(maze.board[pCell->x][pCell->y].walls[up])) {
                cellQueue.push(&(maze.board[pCell->x][pCell->y + 1]));
                maze.board[pCell->x][pCell->y + 1].floodfillMark = true;
                maze.board[pCell->x][pCell->y + 1].floodfillValue = floodfillAsignmentValue;
                
            }
            if (pCell->y - 1 >= 0 && !maze.board[pCell->x][pCell->y - 1].floodfillMark && !(maze.board[pCell->x][pCell->y].walls[down])) {
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


void mouseForward (Maze_t& maze)
{
    switch(maze.mouse.orientation)
    {
        case up: maze.mouse.y += 1; break;
        case down: maze.mouse.y -= 1; break;
        case right: maze.mouse.x +=1; break;
        case left: maze.mouse.x -= 1; break;
    }
    API::moveForward();

    return ;
}

void mouseLeft (Maze_t& maze)
{
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

void mouseRight (Maze_t& maze)
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

bool hasReachedTarget(Maze_t& maze, std::vector<Cell_t*>& targets)
{
    unsigned int i;
    for (i = 0; i < targets.size(); i++)
    {
        if ((maze.mouse.x == targets[i]->x) && (maze.mouse.y == targets[i]->y))
            return 1;
    }
    
    return 0;
}

bool hasFinished (Maze_t& maze)
{
    if ((maze.mouse.x != 7) || (maze.mouse.y != 8))
        return 0;
    if ((maze.mouse.y != 7) || (maze.mouse.y != 8))
        return 0;
    
    return 1;
}

unsigned int timeElapsedInSeconds (void)
{
    
    auto endingTime = std::chrono::steady_clock::now();
    unsigned int time = std::chrono::duration_cast<std::chrono::seconds>(endingTime-globalStartTime).count();
    log(std::to_string(time));

    return time;
}

bool firstRunCutoff(void)
{
    log(std::to_string(timeElapsedInSeconds()));
    if (timeElapsedInSeconds() > TIME_LIMIT/2)
    {
        log("Cutoff Triggered");
        return 1;
    }   
    
    return 0;
}