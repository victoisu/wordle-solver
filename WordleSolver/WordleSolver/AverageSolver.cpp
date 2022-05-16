#include "AverageSolver.h"
#include "CheckResult.h"
#include "DictionaryHandler.h"

std::pair<string, double> AverageSolver::TopGuessEasy(const vector<string>& remaining)
{
	auto res = AllGuessesEasy(remaining);
	return res[0];
}

vector<std::pair<string, double>> AverageSolver::AllGuessesEasy(const vector<string>& remaining)
{
	vector<std::pair<string, double>> result;

#pragma omp parallel for
	for (int i = 0; i < dict.size(); i++)
	{
		//std::cout << i << std::endl;
		auto res = std::make_pair(dict[i], Score(dict[i], remaining));
#pragma omp critical
		result.push_back(res);
	}

	std::sort(result.begin(), result.end(), [](const auto& a, const auto& b) -> bool {
		return a.second < b.second;
		});
	return result;
}

double AverageSolver::Score(const string word, const vector<string> remaining)
{
	int size = remaining.size();
	if (size == 0)
		return 0.0;
	else if (size == 1) 
		return word == remaining[0] ? 0.0 : 1.0;
	double average = 0.0;
	int possibleTable[CheckResult::maxId + 1] = {};

#pragma omp parallel for
	for (int i = 0; i < size; i++)
	{
		CheckResult res;
		string answer = remaining[i];
		if (word != answer) 
		{
			int possibles = 0, resId;
			res = CheckResult::CalculateResult(word, answer);
			resId = res.Id();
#pragma omp critical
			possibles = possibleTable[resId];

			if (possibles == 0) {
				possibles = DictionaryHandler::GetPossibles(remaining, res).size();
#pragma omp critical
				possibleTable[resId] = possibles;
			}

//#pragma omp critical
//			average = average < possibles ? possibles : average;
#pragma omp atomic
			average += double(possibles) / double((size-1));
		}
	}
	double chanceWrong;
	if (std::find(remaining.begin(), remaining.end(), word) != remaining.end())
		chanceWrong = 1.0 - (1.0 / size);
	else
		chanceWrong = 1.0;

	return average * chanceWrong;
}
