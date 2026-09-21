#include "ScoreManager.h"

void ScoreManager::quickSort(int low, int high) {
	if(low < high){
		int pi = partition(low, high);
		quickSort(low, pi - 1);
		quickSort(pi + 1, high);
	}
}

int ScoreManager::partition(int low, int high) {
	int pivot = scores[high].score;
	int i = low - 1;
	for(int j = low; j < high; j++){
		if(scores[j].score >= pivot){
			i++;
			ScoreRecord aux = scores[i];
			scores[i] = scores[j];
			scores[j] = aux;
		}
	}
	ScoreRecord aux = scores[i + 1];
	scores[i + 1] = scores[high];
	scores[high] = aux;
	return i + 1;
}

ScoreManager::ScoreManager() {
	for(int i = 0; i < MAX_RECORDS; i++){
		scores[i].playerName = "";
		scores[i].score = 0;
	}
	recordCount = 0;
}

bool ScoreManager::loadFile(const std::string& path) {
	std::ifstream file(path);
	if(!file.is_open()) return false;
	recordCount = 0;
	while(recordCount < MAX_RECORDS && std::getline(file, scores[recordCount].playerName, ':')) {
		file >> scores[recordCount].score;
		file.ignore(); 
		recordCount++;
	}
	file.close();
	return true;
}

bool ScoreManager::saveFile(const std::string& path) {
	std::ofstream file(path);
	if(!file.is_open()) return false;
	for(int i = 0; i < recordCount; i++){
		file << scores[i].playerName << ":" << scores[i].score << "\n";
	}
	file.close();
	return true;
}

void ScoreManager::addScore(const std::string& playerName, int score) {
	if(recordCount < MAX_RECORDS){
		scores[recordCount].playerName = playerName;
		scores[recordCount].score = score;
		recordCount++;
	}
}

void ScoreManager::sortByBubbleSort() {
	for(int i = 0; i < recordCount - 1; i++){
		for(int j = 0; j < recordCount - i - 1; j++){
			if(scores[j].score < scores[j + 1].score){
				ScoreRecord aux = scores[j];
				scores[j] = scores[j + 1];
				scores[j + 1] = aux;
			}
		}
	}
}

void ScoreManager::sortByQuickSort() {
	if(recordCount > 1){
		quickSort(0, recordCount - 1);
	}
}

int ScoreManager::getRecordCount() const {
	return recordCount;
}

ScoreRecord ScoreManager::getRecordAt(int index) const {
	if(index >= 0 && index < recordCount){
		return scores[index];
	}
	return ScoreRecord{"", 0};
}
