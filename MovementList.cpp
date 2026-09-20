#include "MovementList.h"

MovementList::MovementList() {
	head = nullptr;
	tail = nullptr;
	current = nullptr;
}

MovementList::~MovementList() {
	clearHistory();
}

void MovementList::registerMove(MovePiece data) {
	DoubleNode* newNode = new DoubleNode(data);
	if(current != tail){
		DoubleNode* toDelete = nullptr;
		
		if(current == nullptr){
			toDelete = head;
			head = nullptr;
			tail = nullptr;
		} else{
			toDelete = current->next;
			current->next = nullptr;
			tail = current;
		}
		
		while(toDelete != nullptr){
			DoubleNode* aux = toDelete->next;
			delete toDelete;
			toDelete = aux;
		}
	}
	if(head == nullptr){
		head = newNode;
		tail = newNode;
		current = newNode;
	} else {
		tail->next = newNode;
		newNode->previous = tail;
		tail = newNode;
		current = tail;
	}
}

bool MovementList::undo(MovePiece& outMove) {
	if(current == nullptr || current->previous == nullptr) return false;
	outMove.actionType = current->previous->move.actionType;
	outMove.pieceType = current->previous->move.pieceType;
	outMove.targetX = current->previous->move.targetX;
	outMove.targetY = current->previous->move.targetY;
	outMove.rotation = current->previous->move.rotation;
	current = current->previous;
	return true;
}

bool MovementList::redo(MovePiece& outMove) {
	if(current == nullptr || current->next == nullptr) return false;
	outMove.actionType = current->next->move.actionType;
	outMove.pieceType = current->next->move.pieceType;
	outMove.targetX = current->next->move.targetX;
	outMove.targetY = current->next->move.targetY;
	outMove.rotation = current->next->move.rotation;
	current = current->next;
	return true;
}

void MovementList::resetToStart() {
	current = head;
}

bool MovementList::getNextMoveStep(MovePiece& outMove) {
	if(current == nullptr) return false;
	outMove = current->move;
	current = current->next;
	return true;
}

void MovementList::clearHistory() {
	DoubleNode* toDelete = head;
	while(toDelete != nullptr){
		DoubleNode* nextNode = toDelete->next;
		delete toDelete;
		toDelete = nextNode;
	}
	head = nullptr;
	tail = nullptr;
	current = nullptr;
}


