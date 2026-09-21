#include<iostream>
#include<SFML/Graphics.hpp>
#include <string>
#include "Board.h"
#include "Piece.h"
#include "HoldStack.h"
#include "MovementList.h"
#include "EventQueue.h"
#include "ScoreManager.h"
using namespace std;

void showScores(const ScoreManager& manager, const std::string& title) {
	std::cout << "\n=== " << title << " ===" << std::endl;
	int count = manager.getRecordCount();
	if (count == 0) {
		std::cout << "No hay registros cargados." << std::endl;
		return;
	}
	for (int i = 0; i < count; i++) {
		ScoreRecord rec = manager.getRecordAt(i);
		std::cout << i + 1 << ". " << rec.playerName << " -> " << rec.score << " pts\n";
	}
}

int main (int argc, char *argv[]) {
	ScoreManager sm;
	std::string filepath = "highscores.txt";
	
	// 1. Inserción de varios usuarios (con y sin espacios)
	std::cout << "Insertando registros de prueba...\n";
	sm.addScore("Carlos Gomez", 1450);
	sm.addScore("Ana Ruiz", 3800);
	sm.addScore("Pedro", 920);
	sm.addScore("Maria Jose", 4500);
	sm.addScore("Luis_99", 2100);
	sm.addScore("Sofia Castro", 3100);
	sm.addScore("David", 500);
	
	showScores(sm, "1. Datos Originales (Sin Ordenar)");
	
	// 2. Prueba de BubbleSort
	sm.sortByBubbleSort();
	showScores(sm, "2. Ordenados con BubbleSort (Descendente)");
	
	// 3. Guardar en archivo plano
	if (sm.saveFile(filepath)) {
		std::cout << "\n[OK] Archivo guardado correctamente con formato 'Nombre:Puntaje'.\n";
	} else {
		std::cout << "\n[ERROR] No se pudo guardar el archivo.\n";
	}
	
	// 4. Probar lectura desde archivo e inserción adicional en una nueva instancia
	ScoreManager sm2;
	if (sm2.loadFile(filepath)) {
		std::cout << "[OK] Datos cargados exitosamente en una nueva instancia.\n";
	} else {
		std::cout << "[ERROR] No se pudo leer el archivo.\n";
	}
	
	// Agregar un jugador nuevo después de cargar
	sm2.addScore("Elena Torres", 2900);
	showScores(sm2, "3. Datos Cargados + Nuevo Registro Desordenado");
	
	// 5. Prueba de QuickSort
	sm2.sortByQuickSort();
	showScores(sm2, "4. Ordenados con QuickSort (Descendente)");
	
	return 0;
}

