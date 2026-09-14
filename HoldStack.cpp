#include "HoldStack.h"

HoldStack::HoldStack()
{
}

HoldStack::~HoldStack()
{
}

void HoldStack::push(int pieceType) {
	StackNode* newNode = new StackNode(pieceType);
	newNode->next = top;
	top = newNode;
}

int HoldStack::pop() {
	if (isEmpty()) return 0;
	top = top->next;
	int type = top->pieceType;
	delete top;
	return type;
}

int HoldStack::peek() const {
	if (isEmpty()) return 0;
	return top->pieceType;
}

bool HoldStack::isEmpty() const
{
	return top == nullptr;
}

bool HoldStack::canSwap() const
{
	return false;
}

void HoldStack::setUsedInTurn(bool status)
{
}
