#pragma once
#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

struct EventData {
	int eventType;
	float triggerTime;
	float parameter;
};

struct EventNode {
	EventData data;
	EventNode* next;

	EventNode(EventData event) : data(event), next(nullptr){}
};

class EventQueue
{
private:
	EventNode* front;

public:
	EventQueue();
	~EventQueue();

	void insertSorted(EventData event);
	bool popFront(EventData& outEvent);
	bool showNextEvent(float currentTiem, EventData& outEvent);
	bool isEmpty() const;
	void clearEvents();
};
#endif // !EVENTQUEUE_H