#pragma once
#ifndef GAMEENGINE_H
#define GAMEENGINE_H
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Piece.h"
#include "HoldStack.h"
#include  "MovementList.h"
#include "EventQueue.h"
#include "ScoreManager.h"

class GameEngine {
private:
	sf::RenderWindow window;

	Board board;
	Piece piece;
	HoldStack holdStack;
	MovementList movList;
	EventQueue eventQueue;
	ScoreManager scoreManager;

	int currentPieceType;
	int currentX;
	int currentY;
	int currentRotarion;
	int score;
	float gameTime;
	float dropTimer;
	float dropInterval;
	bool isGameOver;
	bool isPaused;

	void processInput();
	void update(float deltaTime);
	void render();
	bool showCollision(int posX, int posY, int newRotation) const;
	void lockPiece();
	void spawnNewPiece();

public:
	GameEngine();
	~GameEngine();

	void run();
};
#endif // !GAMEENGINE_H
