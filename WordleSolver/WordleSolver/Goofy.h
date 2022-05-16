#pragma once
#include <vector>
#include <string>

class Goofy
{
public:
	static int Solve(std::vector<std::string> dict, std::string answer, std::string guess, bool hard);

	static bool SolveMaxGray(std::vector<std::string> dict, std::string answer, std::vector<std::string>& out);

	static std::vector<std::pair<std::string, double>> BlindGoForGray(std::vector<std::string> dict, bool hard, std::vector<std::string> remaining);

private:
	static int CharacterToIndex(char c) {
		return c - 'a';
	}

	static std::vector<std::string> RecursiveGraySearch(std::vector<std::string> dict, std::vector<std::string> remaining, std::string answer, int quota, int depth);

	static std::vector<std::vector<double>> GreenRatios(std::vector<std::string> dict);

	static std::vector<std::pair<std::string, double>> ValidWordsScored(std::vector<std::string> dict, std::string answer, std::vector<std::string> remaining, int quota);

	static std::vector<std::pair<std::string, int>> ValidWordsQuota(std::vector<std::string> dict, std::string answer, int quota);
};

