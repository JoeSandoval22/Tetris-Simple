#include "HoldStack.h"

HoldStack::HoldStack() {
	top = nullptr;
	usedInTurn = false;
}

HoldStack::~HoldStack(){
	while(!isEmpty()){
		pop();
	}
}

void HoldStack::push(int pieceType) {
	StackNode* newNode = new StackNode(pieceType);
	newNode->next = top;
	top = newNode;
}

int HoldStack::pop() {
	if (isEmpty()) return -1;
	StackNode* current = top;
	int type = current->pieceType;
	top = top->next;
	delete current;
	return type;
}

int HoldStack::peek() const {
	if (isEmpty()) return -1;
	return top->pieceType;
}

bool HoldStack::isEmpty() const {
	return top == nullptr;
}

bool HoldStack::canSwap() const {
	return !usedInTurn;
}

void HoldStack::setUsedInTurn(bool status) {
	usedInTurn = status;
}
