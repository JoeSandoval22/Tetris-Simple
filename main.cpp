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

int main (int argc, char *argv[]) {
	GameEngine engine;
	engine.run();
	
	return 0;
}

