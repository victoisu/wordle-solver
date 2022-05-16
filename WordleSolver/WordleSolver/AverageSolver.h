#pragma once
#include "Solver.h"
class AverageSolver :
    public Solver
{
public:
	const vector<string> dict;

	AverageSolver(const vector<string> dictionary) : dict(dictionary) {}

	std::pair<string, double> TopGuessEasy(const vector<string>& remaining);

	std::pair<string, double> TopGuessHard() { return TopGuessEasy(dict); }

	vector<std::pair<string, double>> AllGuessesEasy(const vector<string>& remaining);

	vector<std::pair<string, double>> AllGuessesHard() { return AllGuessesEasy(dict); }

	double Score(const string word) { return Score(word, this->dict); }

	double Score(const string word, const vector<string> remaining);

	virtual vector<string> GetDictionary() const { return dict; };
};

