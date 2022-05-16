#pragma once
#include "Solver.h"
#include <chrono>

class DoubleAverageSolver : public Solver
{
public:
	const vector<string> dict;
	long long progress = 0;
	long long maxProgress = 0;
	std::chrono::system_clock::time_point start;

	DoubleAverageSolver(const vector<string> dictionary) : dict(dictionary) {}

	std::pair<string, double> TopGuessEasy(const vector<string>& remaining);

	std::pair<string, double> TopGuessHard() { return TopGuessEasy(dict); }

	vector<std::pair<string, double>> AllGuessesEasy(const vector<string>& remaining);

	vector<std::pair<string, double>> AllGuessesHard() { return AllGuessesEasy(dict); }

	double Score(const string word) { return Score(word, this->dict); }

	double Score(const string word, const vector<string> remaining);

	virtual vector<string> GetDictionary() const { return dict; };
};

