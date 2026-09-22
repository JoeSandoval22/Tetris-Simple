#pragma once
#ifndef MOVEMENTLIST_H
#define MOVEMENTLIST_H
#include <iostream>
#include <string>

struct MovePiece {
	std::string actionType;
	int pieceType;
	int targetX;
	int targetY;
	int rotation;
	
	MovePiece(std::string newActionType, int newPieceType, int newTargetX,int newTargetY, int newRotation) : actionType(newActionType), 
		pieceType(newPieceType), targetX(newTargetX), targetY(newTargetY), rotation(newRotation){}
	
	MovePiece() : actionType(""), pieceType(0), targetX(0), targetY(0), rotation(0) {}
};

struct DoubleNode {
	MovePiece move;
	DoubleNode* previous;
	DoubleNode* next;

	DoubleNode(MovePiece data) : move(data), previous(nullptr), next(nullptr){}
};

class MovementList {
private:
	DoubleNode* head;
	DoubleNode* tail;
	DoubleNode* current;
public:
	MovementList();
	~MovementList();

	void registerMove(MovePiece data);
	bool undo(MovePiece& outMove);
	bool redo(MovePiece& outMove);
	void resetToStart();
	bool getNextMoveStep(MovePiece& outMove);
	void clearHistory();
};
#endif // !MOVEMENTLIST_H
