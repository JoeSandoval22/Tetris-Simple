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
	DoubleNode* prev;
	DoubleNode* next;

	DoubleNode(MovePiece data) : move(data), prev(nullptr), next(nullptr){}
};

class MovementList
{
private:
	DoubleNode* head;
	DoubleNode* tail;
	DoubleNode* actual;

public:
	MovementList();
	~MovementList();

	void registerMove(MovePiece data);
	bool undo(MovePiece& outMove);
	bool redo(MovePiece& outMode);
	void resetToStart();
	bool getNextMoveStep(MovePiece& outMove);
	void clearHistory();
};
#endif // !MOVEMENTLIST_H
