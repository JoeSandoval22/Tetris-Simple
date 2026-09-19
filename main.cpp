#include<iostream>
#include<SFML/Graphics.hpp>
#include "Board.h"
#include "Piece.h"
using namespace std;

int main (int argc, char *argv[]) {
	srand(time(0));
	
	Piece cola;
	
	std::cout << "Tamano inicial (deben ser 14): " << cola.getPieceSize() << "\n";
	
	std::cout << "Primeras 3 piezas en la fila:\n";
	for(int i = 1; i <= 3; i++) {
		std::cout << "Pos " << i << ": " << cola.getPieceAt(i) << "\n";
	}
	
	std::cout << "\nSimulando extraccion de 8 piezas (recarga automatica):\n";
	for(int i = 0; i < 8; i++) {
		int p = cola.deletePiece();
		std::cout << "Sacrificada pieza: " << p << " | Quedan: " << cola.getPieceSize() << "\n";
	}
	
	return 0;
}

