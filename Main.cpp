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

//Configuration Defines
#define MAZE_SIZE 16
#define TIME_LIMIT (60*4)

//Global variables
std::chrono::_V2::steady_clock::time_point globalStartTime = std::chrono::steady_clock::now();


typedef enum {
	up = 0,
	right,
	down,
	left
}Orientation_t;

typedef struct {
	int x;
	int y;
	Orientation_t orientation;
}Mouse_t;



typedef struct {
	unsigned int floodfillValue;
	bool walls[4];
	bool mark;
	bool floodfillMark;
	int x;
	int y;
}Cell_t;

typedef struct {
	Cell_t board[MAZE_SIZE][MAZE_SIZE];
	Mouse_t mouse;

}Maze_t;


void initMaze(Maze_t& maze);
void floodFill(Maze_t& maze, std::vector<Cell_t*>& initialCells);
void pathTo(Maze_t& maze, std::vector<Cell_t*>& targets, bool (*cutoff)(void));
bool hasReachedTarget(Maze_t& maze, std::vector<Cell_t*>& targets);
bool hasFinished(Maze_t& maze);
void updateCell(Maze_t& maze);
void displayFloodfill(Maze_t& maze);
void firstRun(Maze_t& maze);
void secondRun(Maze_t& maze);
void thirdRun(Maze_t& maze);
void goBackToBeginning(Maze_t& maze);
void mouseForward(Maze_t& maze);
void mouseLeft(Maze_t& maze);
void mouseRight(Maze_t& maze);
unsigned int timeElapsedInSeconds(void);
bool firstRunCutoff(void);



int main(int argc, char* argv[])
{
	/**
	 * Inicializamos la cantidad de intentos, y dependiendo
	 * del intento en el que estamos, ejecutamos una
	 * rutina distinta.
	 */
	Maze_t maze;
	int triesLeft = 3;
	initMaze(maze);

	while (triesLeft)
	{
		switch (triesLeft--)
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

/**
 * @brief Inicializa la posición del mouse y da un valor inicial de 
 * floodfill a cada celda del laberinto e inicializa dichas celdas.
 * 
 * @param maze Laberinto a ser inicializado.
 */
void initMaze(Maze_t& maze) {
	maze.mouse.x = 0;
	maze.mouse.y = 0;
	maze.mouse.orientation = up;
	int i;
	int j;

	for (i = 0; i < MAZE_SIZE; i++) {
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
	// Colocamos una pared en la posición detras de la posición inicial
	maze.board[0][0].walls[down] = 1;
	API::setWall(0, 0, 's');


}

/**
 * @brief Estrategia para el primer recorrido del laberinto
 *
 * @param maze Situacion actual del laberinto
 */
void firstRun(Maze_t& maze)
{
	std::vector<Cell_t*>vec = { &maze.board[0][MAZE_SIZE - 1] };
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

	pathTo(maze, vec, NULL);

	//Luego de llegar al centro del laberinto, vuelve al inicio.
	goBackToBeginning(maze);

	return;
}


/**
 * @brief Estrategia para el segundo recorrdio del laberinto.
 *
 * @param maze Situacion actual del laberinto
 */
void secondRun(Maze_t& maze)
{
	/**
	 * En el segundo intento, se intenta ir al centro lo más rápido
	 * posible aprovechando la información recogida en el primer intento.
	 * La idea es dejar un recorrido óptimo para el tercer intento en base a los 
	 * valores de FloodFill.
	 */
	std::vector<Cell_t*> vec =
	{
		&maze.board[7][7],
		&maze.board[7][8],
		&maze.board[8][7],
		&maze.board[8][8],
	};

	pathTo(maze, vec, NULL);
	goBackToBeginning(maze);

}


/**
 * @brief Estrategia para el tercer recorrido del laberinto.
 *
 * @param maze Situacion actual del laberinto
 */
void thirdRun(Maze_t& maze)
{
	/**
	 * Último intento, simplemente llegar al centro lo más eficientemente
	 * posible.
	 */
	std::vector<Cell_t*> vec =
	{
		&maze.board[7][7],
		&maze.board[7][8],
		&maze.board[8][7],
		&maze.board[8][8],
	};

	pathTo(maze, vec, NULL);

}


/**
 * @brief Dada una situación de laberinto, lleva al mouse a su posición inicial
 *
 * @param maze Situacion actual del laberinto
 */
void goBackToBeginning(Maze_t& maze)
{
	std::vector<Cell_t*> vec2 = { &maze.board[0][0] };

	pathTo(maze, vec2, NULL);
}

/**
 * @brief Dirige a maze.mouse hacia tanto una casilla
 * individiual o un grupo de casillas adyacentes en el 
 * laberinto.
 *
 * @param maze Situacion actual del laberinto
 * @param targets Posiciones a las cuales se quiere dirigir a maze.mouse
 * @param cutoff Función opcional para agregar condiciones en caso de 
 * 				 que sea necesario terminar el recorrido antes de llegar
 * 				 al objetivo.
 */
void pathTo(Maze_t& maze, std::vector<Cell_t*>& targets, bool (*cutoff)(void))
{
	/**
	 * La idea es llegar a la/las casillas objetivo lo más óptimamente posible.
	 * Para esto, el maze.mouse se dirige a la casilla adyacente con el menor valor
	 * posible de FloodFill. Si esta casilla no fue analizada anteriormente, se añade
	 * la información que se pueda recopilar de esta a la maze, y se corre de nuevo el 
	 * FloodFill. La búsqueda termina cuando o se llega a alguna de las casillas objetivo,
	 * o cuando, en el caso de tener definida una función de cutoff, esta active un trigger.
	 */
	while (!hasReachedTarget(maze, targets) && (cutoff == NULL || !cutoff()))
	{
		updateCell(maze);
		floodFill(maze, targets);
		displayFloodfill(maze);

		Orientation_t bestOrientation = maze.mouse.orientation;
		unsigned int value = ~0; //Max value
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

		//Elegimos el sentido de rotación más eficiente.
		if (((maze.mouse.orientation + 1) % 4) == bestOrientation)
		{
			while (maze.mouse.orientation != bestOrientation)
				mouseRight(maze);
		}
		else
		{
			while (maze.mouse.orientation != bestOrientation)
				mouseLeft(maze);
		}

		//Una vez que maze.mouse esta orientado en la mejor dirección posible, avanzamos.
		mouseForward(maze);
	}
}

/**
 * @brief Si la celda nunca fue marcada, la analiza
 * 		  e inserta los datos pertinenntes.
 * 
 * @param maze la situación actual de maze.
 */
void updateCell(Maze_t& maze)
{
	//Si ya fue analizada, no hace falta rehacerlo.
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
			if (maze.mouse.y + 1 < MAZE_SIZE)
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


/**
 * @brief Implementación de FloodFill
 *
 * @param maze Situacion actual del laberinto.
 * @param initialCells Celdas desde las cuales se inicia el floodfill.
 */
void floodFill(Maze_t& maze, std::vector<Cell_t*>& initialCells) {

	std::queue<Cell_t*> cellQueue;
	// Se cargan las primeras direcciones de destino
	for (auto cell : initialCells) {
		cellQueue.push(cell);
		cell->floodfillValue = 0;
		cell->floodfillMark = true;
	}

	int floodfillAsignmentValue = 1;

	while (!cellQueue.empty()) {
		
		int queueSize = cellQueue.size();
		int i;
		// Itero sobre todos los elementos cargados en un nivel de floodfill
		for (i = 0; i < queueSize; i++) {
			
			Cell_t* pCell = cellQueue.front();
			
			/* En caso de que las posiciones alrededor de una celda sean validas, no hayan sido ya visitadas
			* y además puedan ser visitadas por la ausencia de una pared, entonces se les asigna el valor
			* de floodfill correspondiente y se las marca como ya visitadas
			*/
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

/**
 * @brief Coloca los valores de floodfill en el laberinto para que se vean gráficamente.
 *
 * @param maze Situacion actual del laberinto
 */
void displayFloodfill(Maze_t& maze) {
	int i;
	int j;
	for (i = 0; i < MAZE_SIZE; i++) {
		for (j = 0; j < MAZE_SIZE; j++) {
			API::setText(i, j, std::to_string(maze.board[i][j].floodfillValue));
		}
	}
}

/**
 * @brief Mueve a maze.mouse para adelante, tanto en
 * 		  la instancia de maze como en el simulador.
 * @param maze La situación actual de maze.
 */
void mouseForward(Maze_t& maze)
{
	switch (maze.mouse.orientation)
	{
		case up:    maze.mouse.y += 1; break;
		case down:  maze.mouse.y -= 1; break;
		case right: maze.mouse.x += 1; break;
		case left:  maze.mouse.x -= 1; break;
		default: break;
	}
	API::moveForward();

	return;
}

/**
 * @brief Rota a maze.mouse hacia la izquierda, tanto en
 * 		  la instancia de maze como en el simulador.
 * @param maze La situación actual de maze.
 */
void mouseLeft(Maze_t& maze)
{
	API::turnLeft();
	switch (maze.mouse.orientation)
	{
		case up:    maze.mouse.orientation = left; break;
		case right: maze.mouse.orientation = up; break;
		case down:  maze.mouse.orientation = right; break;
		case left:  maze.mouse.orientation = down; break;
		default: break;
	}
}

/**
 * @brief Rota a maze.mouse hacia la derecha, tanto en
 * 		  la instancia de maze como en el simulador.
 * @param maze La situación actual de maze.
 */
void mouseRight(Maze_t& maze)
{
	API::turnRight();
	switch (maze.mouse.orientation)
	{
		case up: 	maze.mouse.orientation = right; break;
		case right: maze.mouse.orientation = down; break;
		case down:  maze.mouse.orientation = left; break;
		case left:  maze.mouse.orientation = up; break;
		default: break;
	}
}

/**
 * @brief Analiza si maze.mouse esta actualmente en una
 * 		  de las posiciones a alcanzar.
 * @param maze La situación actual de la maze.
 * @param targets Vector con las posiciones objetivo a alcanzar.
 * @return 1 si llegó a alguna de las casillas objetivo, 0 en 
 * 		   caso contrario.
 */
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

/**
 * @brief Verifica si maze.mouse llegó al centro del Laberinto,
 * 		  caso especial de hasReachedTarget.
 * @param maze La situación actual de la Maze.
 * @return 0 si termino, 1 si aún no terminó.
 */
bool hasFinished(Maze_t& maze)
{
	if ((maze.mouse.x != 7) || (maze.mouse.y != 8))
		return 0;
	if ((maze.mouse.y != 7) || (maze.mouse.y != 8))
		return 0;

	return 1;
}

/**
 * @brief Wrapper de funciones de std::chrono para determinar
 * 		  cuantos segundos pasaron desde el inicio del programa.
 * @return Cantidad de segundos desde el inicio de la ejecución.
 */
unsigned int timeElapsedInSeconds(void)
{

	auto endingTime = std::chrono::steady_clock::now();
	unsigned int time = std::chrono::duration_cast<std::chrono::seconds>(endingTime - globalStartTime).count();
	log(std::to_string(time));

	return time;
}

/**
 * @brief Determina si hay que cortar el recorrido del laberinto 
 * 		  en el primer intento antes de terminar la estrategia, debido
 * 		  al tiempo restante.
 * @return 0 si todavia no hubo un Trigger, 1 si hubo un Trigger y hay que 
 * 	       terminar el recorrido.
 */
bool firstRunCutoff(void)
{
	log(std::to_string(timeElapsedInSeconds()));
	if (timeElapsedInSeconds() > TIME_LIMIT / 4)
	{
		log("Cutoff Triggered");
		return 1;
	}

	return 0;
}