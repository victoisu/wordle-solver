#include "Goofy.h"
#include "CheckResult.h"
#include "DictionaryHandler.h"
#include "Runner.h"
#include <algorithm>

long bignumber = 0L;

int Goofy::Solve(std::vector<std::string> dict, std::string answer, std::string guess, bool hard)
{
	auto remaining = dict;
	int iterations = 1;
	std::cout << remaining.size() << " ";
	if (guess == "")
		guess = BlindGoForGray(dict, hard, dict)[0].first;

	while (true) {
		std::cout << guess << std::endl;
		if (guess == answer) {
			return iterations;
		}

		iterations++;
		auto result = CheckResult::CalculateResult(guess, answer);
		remaining = DictionaryHandler::GetPossibles(remaining, result);
		std::cout << remaining.size() << " ";
		auto fullGuess = BlindGoForGray(hard ? remaining : dict, hard, remaining)[0];
		guess = fullGuess.first;
		std::cout << fullGuess.second << " ";
	}

	return -1;
}

bool Goofy::SolveMaxGray(std::vector<std::string> dict, std::string answer, std::vector<std::string> &out)
{
	std::cout << "\nGuessing: " << answer << std::endl;
	int quota = 0;
	while (true) {
		bignumber = 0L;
		auto a = ValidWordsQuota(dict, answer, quota).size();
		std::cout << "Quota: " << quota << std::endl;
		std::cout << "Valid words: " << a << std::endl;
		std::cout << "Max searches: " << a*a*a*a*a << std::endl;
		auto res = RecursiveGraySearch(dict, dict, answer, quota, 5);
		if (res[0] != ".") {
			out = res;
			return true;
		}

		if (quota >= 10)
			return false;

		quota++;
	}

	return false;
}

std::vector<std::string> Goofy::RecursiveGraySearch(std::vector<std::string> dict, std::vector<std::string> remaining, std::string answer, int quota, int depth) 
{
#pragma omp atomic
		bignumber++;
	std::vector<std::string> out = { "." };

	if (remaining.size() == 1 && remaining[0] == answer)
		return remaining;

	if (depth == 0)
		return out;

	auto valid = ValidWordsQuota(dict, answer, quota);
	std::vector<int> indices(valid.size());
	std::iota(indices.begin(), indices.end(), 0);
	std::shuffle(indices.begin(), indices.end(), std::mt19937{ std::random_device{}() });

#pragma omp parallel for
	for (int i = 0; i < indices.size(); i++)
	{
		if (bignumber > 1000000) {
			out = { "." };
			break;
		}
		auto attempt = valid[indices[i]];
		auto res = CheckResult::CalculateResult(attempt.first, answer);
		auto possibles = DictionaryHandler::GetPossibles(remaining, res);

		if (possibles.size() >= remaining.size())
			continue;

		auto result = RecursiveGraySearch(dict, possibles, answer, attempt.second, depth - 1);
		if (result[0] != ".") 
		{
			result.push_back(attempt.first);
#pragma omp critical
			out = result;
			break;
		}
	}

	return out;
}

std::vector<std::pair<std::string, double>> Goofy::BlindGoForGray(std::vector<std::string> dict, bool hard, std::vector<std::string> remaining)
{
	remaining = hard ? dict : remaining;
	int size = dict.size();
	int rsize = remaining.size();
	std::vector<std::pair<std::string, double>> result(size);
	double lowest = 2.0;

	int progress = 0;
	int max_prog = size * size;
	if (rsize == 1)
		return std::vector<std::pair<std::string, double>>(1, std::make_pair(remaining[0], 1.0));

	auto ratios = GreenRatios(remaining);

#pragma omp parallel for
	for (int i = 0; i < size; i++)
	{
		std::string guess = dict[i];
		std::string answer;
		CheckResult res;
		int count = 0, greens = 0, resId, possibles;
		double averageGreens = 0.0, averagePossible = 0.0;
		int possibleTable[CheckResult::maxId + 1] = {};

		for (int j = 0; j < rsize; j++)
		{
			answer = remaining[j];
			res = CheckResult::CalculateResult(guess, answer);
			//if (guess == answer) {
			//	possibles = 1;
			//}
			//else {
			//	resId = res.Id();
			//	if (possibleTable[resId] == 0) {
			//		possibleTable[resId] = DictionaryHandler::GetPossibles(hard ? dict : remaining, res).size();
			//	}
			//	possibles = possibleTable[resId];
			//}

			greens = (res.greens == 0 && res.yellows == 0) ? 0 : 1;

			averageGreens += double(greens) / (double(rsize));
			//averagePossible += double(possibles) / (double(rsize) * double(size));

//#pragma omp atomic
//			progress++;
//			if (progress % 127 == 0) {
//#pragma omp critical
//				std::cout << (double)progress/(double)max_prog << "\r";
//			}
		}

		result[i] = std::make_pair(guess, averageGreens);
	}

	std::sort(result.begin(), result.end(), [](const auto& a, const auto& b) -> bool {
		return a.second < b.second;
		});

	return result;
}

std::vector<std::vector<double>> Goofy::GreenRatios(std::vector<std::string> dict)
{
	std::vector<std::vector<double>> res(5, std::vector<double>(26, 0.0));

	for (int i = 0; i < dict.size(); i++)
	{
		std::string word = dict[i];
		for (int j = 0; j < 5; j++)
		{
			int index = CharacterToIndex(word[j]);
			res[j][index] += (1.0 / dict.size());
		}
	}

	return res;
}

std::vector<std::pair<std::string, double>> Goofy::ValidWordsScored(std::vector<std::string> dict, std::string answer, std::vector<std::string> remaining, int quota)
{
	int size = dict.size();
	int rsize = remaining.size();
	std::vector<std::pair<std::string, double>> result;
	int possibleTable[CheckResult::maxId + 1] = {};
#pragma omp parallel for
	for (int i = 0; i < size; i++)
	{
		std::string guess = dict[i];
		int count = 0, greens = 0, resId, possibles;
		CheckResult res = CheckResult::CalculateResult(guess, answer);

		if (res.greens + res.yellows <= quota)
		{
			resId = res.Id();
			if (possibleTable[resId] == 0) {
				possibleTable[resId] = DictionaryHandler::GetPossibles(remaining, res).size();
			}
			possibles = possibleTable[resId];
#pragma omp critical
			result.push_back(std::make_pair(guess, possibles));
		}
	}

	std::sort(result.begin(), result.end(), [](const auto& a, const auto& b) -> bool {
		return a.second < b.second;
		});

	return result;
}

std::vector<std::pair<std::string, int>> Goofy::ValidWordsQuota(std::vector<std::string> dict, std::string answer, int quota)
{
	std::vector<std::pair<std::string, int>> result;
#pragma omp parallel for
	for (int i = 0; i < dict.size(); i++)
	{
		auto guess = dict[i];
		auto res = CheckResult::CalculateResult(guess, answer);
		if (res.greens + res.yellows <= quota) {
#pragma omp critical
			result.push_back(std::make_pair(guess, quota - (res.greens + res.yellows)));
		}
	}

	return result;
}
