#pragma once
#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H
#include <string>
#include <fstream>
const int MAX_RECORDS = 10000;

struct ScoreRecord {
	std::string playerName;
	int score;
};

class ScoreManager
{
private:
	ScoreRecord scores[MAX_RECORDS];
	int recordCount;

	void quickSort(int low, int high);
	int partition(int low, int high);

public:
	ScoreManager();

	bool loadFile(const std::string& path);
	bool saveFile(const std::string& path);
	void addScore(const std::string& playerName, int score);
	void sortByBubbleSort();
	void sortByQuickSort();
	int getRecordCount() const;
	bool substractRecordCount(int substract);
	ScoreRecord getRecordAt(int index) const;
	void registerNewScore(const std::string& name, int newScore);
	void runBenchmark();
};
#endif // !SCOREMANAGER_H
