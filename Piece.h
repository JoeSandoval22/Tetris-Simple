#pragma once
#ifndef PIECE_H
#define PIECE_H

struct QueueNode {
	int pieceType;
	QueueNode* next;

	QueueNode(int type): pieceType(type),next(nullptr){}
};

class Piece
{
private: 
	QueueNode* front;
	QueueNode* back; 
	int size;

	void generateBag();

public:
	Piece();
	~Piece();

	void addPiece(int pieceType);
	int deletePiece();
	int showPiece(int position);
	bool isEmpty() const;
	int getPieceSize() const;

};
#endif // !PIECE
