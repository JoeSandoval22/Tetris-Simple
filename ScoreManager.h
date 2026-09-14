#pragma once
#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H
#include <string>

struct ScoreRecord {
	std::string playerName;
	int score;
};

class ScoreManager
{
private:
	ScoreRecord scores[100];
	int recordCount;

	void quikSort(int low, int high);
	int partition(int low, int high);

public:
	ScoreManager();

	bool loadFile(const std::string& path);
	bool saveFile(const std::string& path);
	void addScore(const std::string& playerName, int score);
	void sortByBubbleSort();
	void sortByQuickSort();
	int getRecordCount() const;
	ScoreRecord getRecordAt(int index) const;
};
#endif // !SCOREMANAGER_H
