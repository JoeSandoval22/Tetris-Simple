#include<iostream>
#include<SFML/Graphics.hpp>
#include "Board.h"
using namespace std;

int main (int argc, char *argv[]) {
	Board board;
	
	cout << "=== PRUEBA MULTI-FILA (TETRIS) EN EL TABLERO ===" << endl;
	
	// 1. Llenamos 4 filas consecutivas (de la 16 a la 19)
	for (int row = 16; row <= 19; row++) {
		for (int col = 0; col < 10; col++) {
			board.setCell(row, col, 1);
		}
	}
	
	// 2. Verificamos que las filas estén ocupadas antes de limpiar
	cout << "Comprobando celdas antes de la limpieza:" << endl;
	cout << " - Fila 16 (16,0): " << (board.isCellOccupied(16, 0) ? "Ocupada" : "Libre") << endl;
	cout << " - Fila 19 (19,0): " << (board.isCellOccupied(19, 0) ? "Ocupada" : "Libre") << endl;
	
	// 3. Ejecutamos la limpieza masiva
	int limpiadas = board.clearCompleteRows();
	
	cout << "\nFilas limpiadas reportadas (Esperado 4): " << limpiadas << endl;
	
	// 4. Verificamos que todo ese bloque de filas haya quedado libre
	cout << "\nComprobando celdas despues de la limpieza:" << endl;
	cout << " - Fila 16 (16,0): " << (board.isCellOccupied(16, 0) ? "Ocupada" : "Libre") << endl;
	cout << " - Fila 19 (19,0): " << (board.isCellOccupied(19, 0) ? "Ocupada" : "Libre") << endl;
	
	return 0;
}

