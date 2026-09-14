#include "MovementList.h"

MovementList::MovementList()
{
}

MovementList::~MovementList()
{
}

void MovementList::registerMove(MovePiece data)
{
}

bool MovementList::undo(MovePiece& outMove)
{
	return false;
}

bool MovementList::redo(MovePiece& outMode)
{
	return false;
}

void MovementList::resetToStart()
{
}

bool MovementList::getNextMoveStep(MovePiece& outMove)
{
	return false;
}

void MovementList::clearHistory()
{
}


