#include "Minimax.h"
#include <iostream>
#include "CheckResult.h"
#include "ProgressBar.h"
#include <algorithm>
#include "DictionaryHandler.h"

std::pair<std::string, double> Minimax::TopGuess(std::vector<std::string> dict)
{
	return AllGuesses(dict, true, false, std::vector<std::string>())[0];
}

std::vector<std::pair<std::string, double>> Minimax::AllGuesses(std::vector<std::string> dict)
{
	//std::cout << "Generating guesses for " << dict.size() << " words." << std::endl;
	return AllGuesses(dict, false, false, std::vector<std::string>());
}

std::pair<std::string, double> Minimax::TopEasyMode(std::vector<std::string> dict, std::vector<std::string> remaining)
{
	return AllGuesses(dict, true, false, remaining)[0];
}

int Minimax::Solve(std::vector<std::string> dict, std::string answer, std::string guess, bool hard)
{
	auto remaining = dict;
	int iterations = 1;
	if (guess == "")
		guess = AllGuesses(dict, true, hard, dict)[0].first;

	while (true) {
		std::cout << guess;
		if (guess == answer) {
			return iterations;
		}

		iterations++;
		auto result = CheckResult::CalculateResult(guess, answer);
		remaining = DictionaryHandler::GetPossibles(remaining, result);
		std::cout << " " << remaining.size() << std::endl;
		guess = remaining.size() == 1 ? remaining[0] : AllGuesses(hard ? remaining : dict, true, hard, remaining)[0].first;
	}

	return -1;
}

std::vector<std::pair<std::string, double>> Minimax::AllGuesses(std::vector<std::string> dict, bool prune, bool hard, std::vector<std::string> remaining)
{
	//std::cout << ProgressBar::CreateProgressBar(0, 1) << "\r";
	remaining = hard ? dict : remaining;
	int size = dict.size();
	int rsize = remaining.size();
	std::vector<std::pair<std::string, double>> result(size);
	int overall_minimax = INT_MAX;
	int progress = 0;
	if (rsize == 1)
		return std::vector<std::pair<std::string, double>>(1, std::make_pair(remaining[0], 1.0));

#pragma omp parallel for
	for (int i = 0; i < size; i++)
	{
		std::string guess = dict[i];
		std::string answer;
		int possibles, resId;
		int max = 0;
		bool bigger = false;
		CheckResult res;
		int possibleTable[CheckResult::maxId + 1] = {};
		for (int j = 0; j < rsize; j++)
		{
			answer = remaining[j];
			res = CheckResult::CalculateResult(guess, answer);
			resId = res.Id();

#pragma omp critical 
			{
				if (possibleTable[resId] == 0) {
					possibleTable[resId] = DictionaryHandler::GetPossibles(hard ? dict : remaining, res).size();
				}
				possibles = possibleTable[resId];
			}


			max = possibles > max ? possibles : max;


//#pragma omp atomic
//			progress++;
//
//			if (progress % 20 == 0) {
//#pragma omp critical
//				std::cout << ProgressBar::CreateProgressBar(progress, size*rsize) << "\r";
//			}

			if (prune) {
				bigger = overall_minimax < max;

				if (bigger) break;
			}
		}

		max = bigger ? INT_MAX : max;
		if (!bigger) {
#pragma omp critical
			overall_minimax = max;
		}
		result[i] = std::make_pair(guess, 1.0 - (max / (double)size));
	}
	//std::cout << ProgressBar::CreateProgressBar(1, 1) << std::endl;

	std::sort(result.begin(), result.end(), [](const auto& a, const auto& b) -> bool {
		return a.second > b.second;
		});

	return result;
}
