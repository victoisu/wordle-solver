#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include <numeric>
#include "Checkresult.h"
#include "Minimax.h"
#include "Average.h"
#include "DictionaryHandler.h"
class Wordle
{
public:
	static void PlayGameKnowingAnswer(std::vector<std::string> dict, std::string answer, std::string startingGuess, bool hard);

	static void RandomMinimaxGame(std::vector<std::string> dict, std::string startingGuess, bool hard);

	static void ManualPlayMinimaxUnknown(std::vector<std::string> dict, std::string startingGuess, bool hard);

	static void ManualPlayAverageUnknown(std::vector<std::string> dict, std::string startingGuess, bool hard);

	static void ManualPlayKnown(std::vector<std::string> dict, std::string answer);

	static void ManualPlayUnknown(std::vector<std::string> dict) { ManualPlayUnknown(dict, dict); }

	static void ManualPlayUnknown(std::vector<std::string> dict, std::vector<std::string> answers);

	static void Compete(int trials, int words);

	static void FullAnalyze(int words);

private:
	static void ManualPlayUnknown(std::vector<std::string> dict, std::string startingGuess, bool hard, std::pair<std::string, double>(*func)(std::vector<std::string>, std::vector<std::string>));
};

