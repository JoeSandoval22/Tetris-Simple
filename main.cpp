#include<iostream>
#include<SFML/Graphics.hpp>
#include "Board.h"
#include "Piece.h"
#include "HoldStack.h"
#include "MovementList.h"
#include "EventQueue.h"
using namespace std;

int main (int argc, char *argv[]) {
	EventQueue queue;
	
	// Insertar eventos desordenados: {eventType, triggerTime, parameter}
	queue.insertSorted({1, 5.0f, 100.0f}); // Evento tipo 1 a los 5.0s
	queue.insertSorted({2, 1.5f, 25.5f});  // Evento tipo 2 a los 1.5s
	queue.insertSorted({3, 10.0f, 1.0f});  // Evento tipo 3 a los 10.0s
	queue.insertSorted({2, 2.0f, 50.0f});  // Evento tipo 2 a los 2.0s
	queue.insertSorted({4, 2.0f, 0.5f});   // Evento tipo 4 a los 2.0s (mismo tiempo, entra después)
	
	// Validar el próximo evento sin sacarlo (peek)
	EventData nextEv;
	if (queue.showNextEvent(nextEv)) {
		std::cout << "Proximo evento en cola (deberia ser a los 1.5s):\n";
		std::cout << "Tipo: " << nextEv.eventType 
			<< " | Tiempo: " << nextEv.triggerTime 
			<< "s | Parametro: " << nextEv.parameter << "\n\n";
	}
	
	std::cout << "Procesando la cola en orden cronologico:\n";
	std::cout << "----------------------------------------\n";
	
	EventData currentEvent;
	while (queue.popFront(currentEvent)) {
		std::cout << "Tiempo: " << currentEvent.triggerTime 
			<< "s | Tipo: " << currentEvent.eventType 
			<< " | Parametro: " << currentEvent.parameter << "\n";
	}
	
	std::cout << "----------------------------------------\n";
	if (queue.isEmpty()) {
		std::cout << "Cola vaciada y memoria liberada correctamente.\n";
	}
	
	return 0;
}

