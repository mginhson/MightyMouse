# Level 4
## Consideraciones
- Nombre del Grupo: La Maquina De Hacer Pajaros.
- Se utiliz� el algoritmo Floodfill para lograr que el raton llegue del inicio del laberinto al centro y viceversa.
- Se logro girar de la manera m�s eficiente en cuanto a cantidad de rotaciones al momento de doblar.
- Al llegar al centro del laberinto, se exploran las 4 posiciones centrales a fin de obtener todas las posibles entradas al laberinto.
- Se resaltan las paredes y se muestra el valor de floodfill de las celdas para verificar que el algoritmo funciona correctamente.
- La estrategia que elegimos fue encarar cada intento de una forma distinta:
  En el primer intento, con FloodFill intentamos llegar al centro lo más pronto posible.
  Habiendo llegado al centro, el tiempo restante que le quede antes de llegar a la marca de tiempo
  delimitada por firstRunCutoff() lo utilizará para intentar llegar a las esquinas, en pos de 
  recorrer la mayor cantidad del laberinto posible. Al cumplirse este corte, el ratón luego vuelve al
  inicio, y en el segundo y tercer intento tiene como objetivo ir directamente la centro de la manera más rápida posible. Generalmente, el tercer intento es marginalmente mejor que el segundo, ya que en el segundo puede que aún se recorran y reconozcan algunas celdas nuevas.
  
- Adjuntamos logotipo del grupo.
