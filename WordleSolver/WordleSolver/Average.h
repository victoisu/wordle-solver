#pragma once
#include <utility>
#include <string>
#include <vector>
#include <iostream>
#include "ProgressBar.h"
#include "DictionaryHandler.h"


class Average
{
public:

	static std::pair<std::string, double> TopGuess(std::vector<std::string> dict);

	static std::vector<std::pair<std::string, double>> AllGuesses(std::vector<std::string> dict);

	static std::pair<std::string, double> TopEasyMode(std::vector<std::string> dict, std::vector<std::string> remaining);

	static std::pair<std::string, double> TopDouble(std::vector<std::string> dict, std::vector<std::string> remaining);

	static std::vector<std::pair<std::string, double>> AllDouble(std::vector<std::string> dict, std::vector<std::string> remaining);

	static int Solve(std::vector<std::string> dict, std::string answer, std::string guess, bool hard);

	static int SolveDouble(std::vector<std::string> dict, std::string answer, std::string guess);

private:
	static std::vector<std::pair<std::string, double>> AllGuesses(std::vector<std::string> dict, bool hard, std::vector<std::string> remaining);

};