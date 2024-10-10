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

typedef struct {
    
    int heuristica;
    int x;
    int y;
    int pesoDeMovimiento;
    Nodo_t* padre;
} Nodo_t;

int main(int argc, char* argv[]) {
    /*
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
        
    }*/
}



/*
* En la primera llamada le pasamos el
* 
* 
* 
* 
*/ 
bool esMismoNodo(Nodo_t& n1, Nodo_t& n2) {
    return (n1.x == n2.x && n1.y == n2.y);
}
bool nodoValido(Nodo_t& n) {
    return n.x >= 0 && n.x <= 16 && n.y >= 0 && n.y <= 16;
}
void heuristica(Nodo_t casilla){
    
    //Primer cuadrante
    if (casilla.x >= 8 && casilla.y >= 8) return casilla.x - 8 + casilla.y - 8;

    //Segundo cuadrante
    else if (casilla.x <= 7 && casilla.y >= 8) return 7 - casilla.x + casilla.y - 8;

    //Tercer cuadrante
    else if (casilla.x <= 7 && casilla.y <= 7) return 7 - casilla.x + 7 - casilla.y;

    //Cuarto cuadrante
    else if (casilla.x = > 8 && casilla.y <= 7) return casilla.x - 8 + 7 - casilla.y;
}
struct compararHeuristica {
    bool operator()(Nodo_t n1, Nodo_t n2) {
        return n1.heuristica + n1.pesoDeMovimiento > n1.pesoDeMovimiento + n2.heuristica;
    }

};
enum directions {
    top,
    left,
    right,
    bottom
};
typedef struct {
    int x;
    int y;
} Lados;
bool hayObstaculo(short int lado, short int direccionNave) {
    if (direccionNave == top) {

        if (lado == right) return API::wallRight();
        else if (lado == left) return API::wallLeft();
        else if (lado == top) return API::wallFront();

    else if (direccionNave == left) {

        if (lado == top) return API::wallRight();
        else if (lado == bottom) return API::wallLeft();
        else if (lado == left) return API::wallFront();
    }
    else if (direccionNave == right) {

        if (lado == bottom) return API::wallRight();
        else if (lado == top) return API::wallLeft();
        else if (lado == right) return API::wallFront();
    }
    else if (direccionNave == bottom) {

        if (lado == left) return API::wallRight();
        else if (lado == right) return API::wallLeft();
        else if (lado == bottom) return API::wallFront();
    }
}
    #include <iostream>
#include <vector>
#include <queue>
#include <cmath>

// Definición de un nodo
struct Nodo_t {
    int x, y;
    int heuristica;
    int pesoDeMovimiento;
    Nodo_t* padre = nullptr;  // Nodo padre para reconstruir el camino
};

// Comparador para la priority_queue (basado en la heurística)
struct compararHeuristica {
    bool operator()(const Nodo_t& a, const Nodo_t& b) {
        return (a.heuristica + a.pesoDeMovimiento) > (b.heuristica + b.pesoDeMovimiento);
    }
};

// Funciones auxiliares (defínelas según tus necesidades)
bool esMismoNodo(const Nodo_t& a, const Nodo_t& b) {
    return a.x == b.x && a.y == b.y;
}

bool nodoValido(const Nodo_t& nodo) {
    // Verifica que el nodo esté dentro de los límites del mapa
    return (nodo.x >= 0 && nodo.x < 16 && nodo.y >= 0 && nodo.y < 16);  // Ejemplo para un mapa de 16x16
}

bool hayObstaculo(int direccion, int direccionNave) {
    // Implementa la lógica para detectar obstáculos según la dirección
    return false;
}

int heuristica(const Nodo_t& nodo) {
    // Calcula la heurística (distancia estimada hasta el objetivo)
    return std::abs(nodo.x - 15) + std::abs(nodo.y - 15);  // Ejemplo: heurística Manhattan hacia el punto (15, 15)
}

    
void aStar(void) {
    
    short int direccionNave = top;
    short int lados[4] = {top, bottom,right, left};
    // Nodos por explorar
    
    std::priority_queue<Nodo_t, std::vector<Nodo_t>, compararHeuristica> nodosAbiertos;
    // Nodos ya explorados
    std::vector<Nodo_t> nodosCerrados;
    
    Nodo_t nodoInicial;
    nodoInicial.x = 0;
    nodoInicial.y = 0;
    nodoInicial.heuristica = heuristica(nodoInicial);
    nodoInicial.pesoDeMovimiento = 0;
    nodosAbiertos.push_back(nodoInicial);
    while (!nodosAbiertos.empty()) {
        Nodo_t nodoMenorHeuristca = nodosAbiertos.top();
        nodosCerrados.push_back(nodoMenorHeuristca);
        //nodosAbiertos.pop();
        
        Nodo_t posiblesDirecciones[4];
        // Miro hacia arriba
        posiblesDirecciones[0].x = nodoMenorHeuristca.x + 0;
        posiblesDirecciones[0].y = nodoMenorHeuristca.y + 1;
        // Miro hacia el abajo
        posiblesDirecciones[1].x = nodoMenorHeuristca.x + 0;
        posiblesDirecciones[1].y = nodoMenorHeuristca.y - 1;
        // Miro hacia la derecha
        posiblesDirecciones[2].x = nodoMenorHeuristca.x + 1;
        posiblesDirecciones[2].y = nodoMenorHeuristca.y + 0;
        // Miro hacia la izquierda
        posiblesDirecciones[3].x = nodoMenorHeuristca.x + -1;
        posiblesDirecciones[3].y = nodoMenorHeuristca.y + 0;
        for (auto direccion : posiblesDirecciones) {
            bool yaCerrado = false;
            int i = 0;
           
            for (auto nodoCerrado : nodosCerrados) {
                if (esMismoNodo(nodoCerrado, direccion)) {
                    yaCerrado;
                    break;
                }
            }
            
            if (yaCerrado || !nodoValido(direccion) || hayObstaculo(lados[i], direccionNave)) continue;
            i++;
            bool nodoYaAbierto = 0;
            for (auto nodoAbierto : nodosAbiertos) {
                if (esMismoNodo(nodoYaAbierto)) { 
                    nodoYaAbierto = 1 
                };
                
            }
            if (!nodoYaAbierto) {
                direccion.heuristica = heuristica(direccion);
                direccion.pesoDeMovimiento = nodoMenorHeuristica.pesoDeMovimiento + 1;
                direccion.padre = &(nodosCerrados.back());
                nodosAbiertos.push_back(direccion);
            }
            
        }
       
        
    }
}
*/
/*
import heapq

# Definimos una clase para los nodos
class Nodo :
    def __init__(self, pos, g, h, padre = None) :
    self.pos = pos   # posición del nodo(x, y)
    self.g = g       # costo g(camino recorrido)
    self.h = h       # costo h(heurística)
    self.padre = padre  # el nodo desde el cual llegamos a este nodo

    def f(self) :
    return self.g + self.h  # costo total f = g + h

    # Comparación entre nodos por f
    def __lt__(self, other) :
    return self.f() < other.f()

    # Función heurística, por ejemplo, distancia de Manhattan
    def heuristica(pos_actual, pos_final) :
    return abs(pos_actual[0] - pos_final[0]) + abs(pos_actual[1] - pos_final[1])

    # Algoritmo A *
    def a_star(mapa, inicio, final) :
    # Lista abierta(nodos por explorar) y cerrada(nodos ya explorados)
    abierta = []
    cerrada = set()

    # Nodo inicial
    nodo_inicio = Nodo(inicio, 0, heuristica(inicio, final))
    heapq.heappush(abierta, nodo_inicio)

    while abierta:
nodo_actual = heapq.heappop(abierta)  # Nodo con el menor f

# Si llegamos al nodo final, reconstruimos el camino
if nodo_actual.pos == final:
camino = []
while nodo_actual :
    camino.append(nodo_actual.pos)
    nodo_actual = nodo_actual.padre
    return camino[:: - 1]  # Devolvemos el camino en orden inverso

    cerrada.add(nodo_actual.pos)

    # Recorremos vecinos(arriba, abajo, izquierda, derecha)
    vecinos = [(0, 1), (0, -1), (1, 0), (-1, 0)]
    for dx, dy in vecinos :
vecino_pos = (nodo_actual.pos[0] + dx, nodo_actual.pos[1] + dy)

# Si el vecino está fuera de los límites o es un obstáculo, lo ignoramos
if not (0 <= vecino_pos[0] < len(mapa) and 0 <= vecino_pos[1] < len(mapa[0])) or mapa[vecino_pos[0]][vecino_pos[1]] == 1:
continue

# Si ya lo exploramos, lo ignoramos
if vecino_pos in cerrada :
continue

# Nodo vecino
g_nuevo = nodo_actual.g + 1  # Suponemos que el costo de moverse es 1
vecino = Nodo(vecino_pos, g_nuevo, heuristica(vecino_pos, final), nodo_actual)

# Si el vecino ya está en la lista abierta y tiene un costo mayor, lo actualizamos
if vecino not in abierta or g_nuevo < vecino.g:
heapq.heappush(abierta, vecino)

return None  # No hay camino

# Ejemplo de uso
mapa = [
    [0, 1, 0, 0, 0],
        [0, 1, 0, 1, 0],
        [0, 0, 0, 1, 0],
        [0, 1, 0, 0, 0],
        [0, 0, 0, 1, 0]
]

inicio = (0, 0)
final = (4, 4)

camino = a_star(mapa, inicio, final)
print("Camino:", camino)
*/

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