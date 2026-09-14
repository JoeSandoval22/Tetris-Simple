#pragma once
#ifndef HOLDSTACK_H
#define HOLDSTACK_H

struct StackNode {
	int pieceType;
	StackNode* next;

	StackNode(int type): pieceType(type),next(nullptr){}
};

class HoldStack
{
private:
	StackNode* top;
	bool usedInTurn;

public: 
	HoldStack();
	~HoldStack();

	void push(int pieceType);
	int pop();
	int peek() const;
	bool isEmpty() const;
	bool canSwap() const;
	void setUsedInTurn(bool status);
};
#endif // !HOLDSTACK_H
