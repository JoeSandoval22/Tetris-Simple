#include<iostream>
#include<SFML/Graphics.hpp>
#include <string>
#include "Board.h"
#include "Piece.h"
#include "HoldStack.h"
#include "MovementList.h"
#include "EventQueue.h"
#include "ScoreManager.h"
#include "GameEngine.h"
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
	GameEngine engine;
	engine.run();
	
	return 0;
}

