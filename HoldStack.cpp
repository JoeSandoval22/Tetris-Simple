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
//Agrega nodos (piezas) a la pila utilizando el push() clásico de las pilas
void HoldStack::push(int pieceType) {
	StackNode* newNode = new StackNode(pieceType);
	newNode->next = top;
	top = newNode;
}
//Elimina una pieza usando un pop() de pilas clásico
int HoldStack::pop() {
	if (isEmpty()) return -1;
	StackNode* current = top;
	int type = current->pieceType;
	top = top->next;
	delete current;
	return type;
}
//Muestra el tope de la pila usando el peek() clásico de las pilas
int HoldStack::peek() const {
	if (isEmpty()) return -1;
	return top->pieceType;
}

bool HoldStack::isEmpty() const {
	return top == nullptr;
}
//Evita que se pueda cambiar de pieza más de una vez por turno
bool HoldStack::canSwap() const {
	return !usedInTurn;
}

void HoldStack::setUsedInTurn(bool status) {
	usedInTurn = status;
}

