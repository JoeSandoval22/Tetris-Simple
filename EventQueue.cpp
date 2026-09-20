#include "EventQueue.h"

EventQueue::EventQueue() {
	front = nullptr;
}

EventQueue::~EventQueue() {
	clearEvents();
}
/*
Este método agrega eventos siguiendo la regla de que el que tiene un tiempo menor en ocurrir va antes
da igual si existe otro que se haya creado primero, si este no va a ocurrir antes del que el juego crea
entonces se ordena agregando el más pronto antes.
*/
void EventQueue::insertSorted(EventData event) {
	EventNode* newNode = new EventNode(event);
	if(isEmpty() || event.triggerTime < front->data.triggerTime){
		newNode->next = front;
		front = newNode;
		return;
	}
	EventNode* current = front;
	while(current->next != nullptr && current->next->data.triggerTime <= event.triggerTime){
		current = current->next;
	}
	newNode->next = current->next;
	current->next = newNode;
}
/*
Elimina el evento que ya ocurrió
*/
bool EventQueue::popFront(EventData& outEvent) {
	if(isEmpty()) return false;
	outEvent = front->data;
	EventNode* toDelete = front;
	front = front->next;
	delete toDelete;
	return true;
}
/*
Muestra el próximo evento en ocurrir en base al tiempo en que fue agregado
*/
bool EventQueue::showNextEvent(EventData& outEvent) const {
	if(isEmpty()) return false;
	outEvent = front->data;
	return true;
}

bool EventQueue::isEmpty() const {
	return front == nullptr;
}
//Limpia de forma total la cola de eventos en el momento que el juego termine.
void EventQueue::clearEvents() {
	while(!isEmpty()){
		EventNode* toDelete = front;
		front = front->next;
		delete toDelete;
	}
}
