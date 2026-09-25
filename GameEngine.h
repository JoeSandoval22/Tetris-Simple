#pragma once
#ifndef GAMEENGINE_H
#define GAMEENGINE_H
#include <SFML/Graphics.hpp>
#include <optional>
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
	sf::Font font;

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
	bool fontLoaded;

	void processInput();
	void update(float deltaTime);
	void render();
	void renderNextPieces(sf::RectangleShape& cellShape);
	void renderHoldPiece(sf::RectangleShape& cellShape);
	void renderScore();
	
	bool isValidPosition(int pieceType, int rotation, int newX, int newY) const;
	void lockPiece();
	void spawnNewPiece();
	void holdCurrentPiece();
	
	bool isTimeExpired() const;
	
	bool timeBonusEvent(int rowsCleared);
	bool punishmentEvent(int rowsReached);
	bool rapidDropEvent();
	
	void handlePlayInput(sf::Keyboard::Key key);
	void handlePausedInput(sf::Keyboard::Key key);
	void handleReplayInput(sf::Keyboard::Key key);

public:
	GameEngine();
	~GameEngine();

	void run();
};
#endif // !GAMEENGINE_H
