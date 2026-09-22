#pragma once
#ifndef GAMEENGINE_H
#define GAMEENGINE_H
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Piece.h"
#include "HoldStack.h"
#include "MovementList.h"
#include "EventQueue.h"
#include "ScoreManager.h"
#include "TetrominoShape.h"

enum class GameState {
	MENU,
    PLAYING,
	PAUSE,
    GAME_OVER,
	REPLAY
};

class GameEngine {
private:
	sf::RenderWindow window;

	Board board;
	Piece piece;
	HoldStack holdStack;
	MovementList movList;
	EventQueue eventQueue;
	ScoreManager scoreManager;
	GameState currentState;

	int currentPieceType;
	int currentX;
	int currentY;
	int currentRotation;
	int score;
	float gameTime;
	float dropTimer;
	float dropInterval;
	bool isGameOver;
	bool isPaused;

	void processInput();
	void update(float deltaTime);
	void render();
	
	bool isValidPosition(int pieceType, int rotation, int newX, int newY) const;
	void lockPiece();
	void spawnNewPiece();
	void holdCurrentPiece();
	
	bool isTimeExpired() const;
	
	void handlePlayInput(sf::Keyboard::Key key);
	void handleReplayInput(sf::Keyboard::Key key);

public:
	GameEngine();
	~GameEngine();

	void run();
};
#endif // !GAMEENGINE_H
