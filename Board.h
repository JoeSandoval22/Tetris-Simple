#pragma once
#ifndef BOARD_H
#define BOARD_H

struct RowNode {
	int cells[10];
	RowNode* next;

	RowNode() : next(nullptr) {
		for (int i = 0; i < 10; i++) {
			cells[i] = 0;
		}
	}
};

class Board {
private:
	RowNode* head;
	int rowCounter;

	void insertEmptyRowAtTop();
	bool isACompleteRow(RowNode* row) const;

public:
	Board();
	~Board();

	void clearBoard();
	RowNode* getRowIndex(int index) const;
	int clearCompleteRows();
	bool isCellOccupied(int row, int column) const;
	void setCell(int row, int column, int value);
};
#endif // !BOARD_H
