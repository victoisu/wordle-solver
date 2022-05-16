#pragma once
#include <utility>
#include <vector>
#include <string>
class Minimax
{
public:

	static std::pair<std::string, double> TopGuess(std::vector<std::string> dict);

	static std::vector<std::pair<std::string, double>> AllGuesses(std::vector<std::string> dict);

	static std::pair<std::string, double> TopEasyMode(std::vector<std::string> dict, std::vector<std::string> remaining);

	static int Solve(std::vector<std::string> dict, std::string answer, std::string guess, bool hard);

private:
	static std::vector<std::pair<std::string, double>> AllGuesses(std::vector<std::string> dict, bool prune, bool hard, std::vector<std::string> remaining);
};