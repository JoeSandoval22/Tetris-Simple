#include "Board.h"

/*
Se encarga de volver a construir las filas que el jugador haya destruido, el tope siempre es dinámico, 
por lo que pueden romperse varias filas a la vez y que se vuelvan a crear hasta el tope real.
*/
void Board::insertEmptyRowAtTop() {
	RowNode* newNode = new RowNode();
	newNode->next = head;
	head = newNode;
	rowCounter++;
}

//Verifica que una fila esté completamente llena tomando en cuenta que ninguna celda tengo 0 como valor.
bool Board::isACompleteRow(RowNode* row) const {
	if (row == nullptr) return false;
	for (int i = 0; i < 10; i++) {
		if (row->cells[i] == 0) {
			return false;
		}
	}
	return true;
}

Board::Board() {
	head = nullptr;
	rowCounter = 0;
	for (int i = 0; i < 20; i++) {
		insertEmptyRowAtTop();
	}
}

Board::~Board() {
	clearBoard();
}

//Barre por completo con el tablero, este método ayuda a liberar memoria
void Board::clearBoard() {
    RowNode* current = head;
	while (current != nullptr) {
		head = head->next;
		delete current;	
	}
	rowCounter = 0;
}

//Obtiene el índice de una fila en específico
RowNode* Board::getRowIndex(int index) const {
	if (index < 0 || index >= rowCounter) return nullptr;
	RowNode* current = head;
	for (int i = 0; i < index; i++) {
		current = current->next;
	}
	return current;
}

//Limpia una única fila siempre y cuando se cumpla que toda está llena.
int Board::clearCompleteRows() {
	
}

//Verifica que una posición ya está llena por otro bloque
bool Board::isCellOccupied(int row, int column) const {
	if (row < 0 || row >= rowCounter || column < 0 || column >= 10) return false;
	RowNode* current = getRowIndex(row);
	if (current != nullptr && current->cells[column] != 0) {
		return true;
	}
	else {
		return false;
	}
}

//Asigna las posiciones de un bloque en cuanto choca o llega al fondo indicando que esas celdad ya están ocupadas.
void Board::setCell(int row, int column, int value) {
	if (row < 0 || row >= rowCounter || column < 0 || column >= 10) return;
	RowNode* current = getRowIndex(row);
	if (current != nullptr) {
		current->cells[column] = value;
	}

}
