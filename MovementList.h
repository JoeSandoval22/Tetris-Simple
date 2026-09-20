#pragma once
#ifndef MOVEMENTLIST_H
#define MOVEMENTLIST_H

struct MovePiece {
	int actionType;
	int pieceType;
	int targetX;
	int targetY;
	int rotation;
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
