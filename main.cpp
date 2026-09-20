#include<iostream>
#include<SFML/Graphics.hpp>
#include "Board.h"
#include "Piece.h"
#include "HoldStack.h"
#include "MovementList.h"
using namespace std;

int main (int argc, char *argv[]) {
	MovementList history;
	MovePiece moveOut;
	
	// 1. Registro inicial de movimientos
	history.registerMove({1, 1, 4, 0, 0});
	history.registerMove({1, 2, 5, 0, 90});
	history.registerMove({1, 3, 6, 0, 180});
	
	std::cout << "--- 1. Probando Undo ---" << std::endl;
	while (history.undo(moveOut)) {
		std::cout << "Deshecho -> Pieza: " << moveOut.pieceType 
			<< ", Pos X: " << moveOut.targetX 
			<< ", Pos Y: " << moveOut.targetY << std::endl;
	}
	
	std::cout << "\n--- 2. Probando Redo ---" << std::endl;
	while (history.redo(moveOut)) {
		std::cout << "Rehecho -> Pieza: " << moveOut.pieceType 
			<< ", Pos X: " << moveOut.targetX 
			<< ", Pos Y: " << moveOut.targetY << std::endl;
	}
	
	// 3. Probando el truncamiento del historial (rama alternativa)
	std::cout << "\n--- 3. Probando Truncamiento por Nuevo Movimiento ---" << std::endl;
	history.undo(moveOut); // Retrocede a pieza 2
	history.undo(moveOut); // Retrocede a pieza 1
	std::cout << "Se realizaron dos undos. Registrando un movimiento nuevo (Pieza 99)..." << std::endl;
	history.registerMove({1, 99, 7, 0, 270}); // Debe borrar el futuro (piezas 2 y 3 anteriores)
	
	std::cout << "Intentando hacer redo tras el nuevo registro:" << std::endl;
	if (!history.redo(moveOut)) {
		std::cout << "Redo bloqueado correctamente (historial futuro borrado con éxito)." << std::endl;
	}
	
	// 4. Probando recorrido desde el inicio (resetToStart y getNextMoveStep)
	std::cout << "\n--- 4. Probando Replay (resetToStart y getNextMoveStep) ---" << std::endl;
	history.resetToStart();
	while (history.getNextMoveStep(moveOut)) {
		std::cout << "Paso registrado -> Pieza: " << moveOut.pieceType 
			<< ", Pos X: " << moveOut.targetX << std::endl;
	}
	
	return 0;
}

