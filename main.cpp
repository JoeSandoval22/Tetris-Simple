#include<iostream>
#include<SFML/Graphics.hpp>
#include "Board.h"
#include "Piece.h"
#include "HoldStack.h"
using namespace std;

int main (int argc, char *argv[]) {
	HoldStack hold;
	int piezaActual = 1; // Representa una pieza, ej: 1 = 'I'
	int piezaSiguienteDeCola = 2; // 2 = 'T'
	
	std::cout << "--- TURNO 1 ---\n";
	std::cout << "Pieza activa: " << piezaActual << "\n";
	
	// 1. Primer uso: Hold está vacío
	if (hold.canSwap()) {
		std::cout << "Accion: Mandar pieza al Hold.\n";
		hold.push(piezaActual);
		hold.setUsedInTurn(true); // Se bloquea por este turno
		
		// Como el Hold estaba vacío, simulamos que sacamos una de la cola principal
		piezaActual = piezaSiguienteDeCola; 
		std::cout << "Hold actual: " << hold.peek() << " | Nueva pieza activa: " << piezaActual << "\n";
	}
	
	// 2. Intento de trampa: Tratar de hacer Hold otra vez antes de que la pieza toque suelo
	std::cout << "\nIntentando usar Hold otra vez...\n";
	if (!hold.canSwap()) {
		std::cout << "[BLOQUEADO] El juego no te deja cambiar dos veces en el mismo turno.\n";
	}
	
	// 3. Fin del turno: La pieza colisiona con el tablero
	std::cout << "\n--- FIN DEL TURNO 1 (La pieza choca) ---\n";
	hold.setUsedInTurn(false); // Reseteamos la bandera
	
	// Empieza turno 2 con una nueva pieza de la cola
	piezaActual = 3; // 3 = 'L'
	
	std::cout << "\n--- TURNO 2 ---\n";
	std::cout << "Pieza activa: " << piezaActual << "\n";
	
	// 4. Segundo uso: Ya hay una pieza en Hold (la 1)
	if (hold.canSwap()) {
		std::cout << "Accion: Intercambiar pieza actual con el Hold.\n";
		
		int piezaQueSale = hold.pop(); // Saco la 1
		hold.push(piezaActual);        // Meto la 3
		hold.setUsedInTurn(true);      // Bloqueo de nuevo
		
		piezaActual = piezaQueSale;    // La pieza activa pasa a ser la que salió del Hold
		
		std::cout << "Hold actual: " << hold.peek() << " | Nueva pieza activa: " << piezaActual << "\n";
	}
	
	return 0;
}

