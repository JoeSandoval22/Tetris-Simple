#include "Piece.h"
/*
Este método genera bolsas de piezas por medio la función rand() la cual genera ayuda a generar figuar
de forma aleatoria con la cantidad de piezas que restan en la bolsa. También se realiza un intercambio
dentro de la función para lograr que no se repitan piezas cada vez que salen.
*/
void Piece::generateBag() {
	int bag[7] = {0,1,2,3,4,5,6};
	for(int i = 6; i >= 0; i--){
		int j = rand() % (i + 1);
		int aux = bag[i];
		bag[i] = bag[j];
		bag[j] = aux;
	}
	for(int i = 0; i < 7; i++){
		addPiece(bag[i]);
	}
}

Piece::Piece(){
	front = nullptr;
	back = nullptr;
	size = 0;
	generateBag();
	generateBag();
} 

Piece::~Piece() {
	while(!isEmpty()){
		deletePiece();
	}
}

void Piece::addPiece(int pieceType) {
	QueueNode* newNode = new QueueNode(pieceType);
	if(isEmpty()){
		front = newNode;
		back = newNode;
	} else{
		back->next = newNode;
		back = newNode;
	}
	size++;
}
// Este método debe llamarse ya dentro del juego para liberar la memoria
int Piece::deletePiece() {
	if(isEmpty()) return -1;
	QueueNode* current = front;
	int piece = current->pieceType;
	front = front->next;
	if(front == nullptr){
		back = nullptr;
	}
	delete current;
	size--;
	if(size <= 7) {
		generateBag();
	}
	return piece;
}
// Al igual que el anterior hay que llamarlo dentro del juego
int Piece::getPieceAt(int position) const {
	QueueNode* current = front; 
	int count = 0;
	while (current != nullptr) {
		if (count == position) {
			return current->pieceType; 
		}
		current = current->next;
		count++;
	}
	return -1;
}

bool Piece::isEmpty() const {
	return front == nullptr;
}

int Piece::getPieceSize() const {
	return size;
}
