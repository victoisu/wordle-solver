#include "DoubleAverageSolver.h"
#include "CheckResult.h"
#include "DictionaryHandler.h"

std::pair<string, double> DoubleAverageSolver::TopGuessEasy(const vector<string>& remaining)
{
	return AllGuessesEasy(remaining)[0];
}

vector<std::pair<string, double>> DoubleAverageSolver::AllGuessesEasy(const vector<string>& remaining)
{
	vector<std::pair<string, double>> result;
	progress = 0;
	maxProgress = dict.size() * dict.size() * remaining.size();
	start = std::chrono::system_clock::now();
	std::cout << "0\r";
	std::cout << "a:" << progress << std::endl << "b:" << maxProgress << std::endl;

#pragma omp parallel for
	for (int i = 0; i < dict.size(); i++)
	{
		auto res = std::make_pair(dict[i], Score(dict[i], remaining));
#pragma omp critical
		{
			result.push_back(res);
		}
	}

	std::cout << std::endl;

	std::sort(result.begin(), result.end(), [](const auto& a, const auto& b) -> bool {
		return a.second < b.second;
		});
	std::cout << "a:" << progress << std::endl << "b:" << maxProgress << std::endl;
	return result;
}

double DoubleAverageSolver::Score(const string word, const vector<string> remaining)
{
	int size = remaining.size();
	if (size == 0)
		return 0.0;
	else if (size == 1)
		return word == remaining[0] ? 0.0 : 1.0;

	double chanceWrong;
	if (std::find(remaining.begin(), remaining.end(), word) != remaining.end())
		chanceWrong = 1.0 / size;
	else
		chanceWrong = 1.0;

	int* possibleTable = new int[CheckResult::maxId * CheckResult::maxId + 1]{};

	const int dictSize = dict.size();
	vector<double> averages(dictSize, 0.0);

#pragma omp parallel for
	for (int i = 0; i < size; i++)
	{
		string answer = remaining[i];
		auto res1 = CheckResult::CalculateResult(word, answer);
		auto possibles = DictionaryHandler::GetPossibles(remaining, res1);
		int possible;
		for (int j = 0; j < dictSize; j++)
		{
			auto word2 = dict[j];
			
			if (word == answer) {
				possible = 0;
			}
			else if (word2 == answer) {
				possible = 1;
			}
			else {
				auto res2 = CheckResult::CalculateResult(word2, answer);
				int combinedId = (res1.Id() - 1) * CheckResult::maxId + res2.Id();
#pragma omp critical
				possible = possibleTable[combinedId];
				if (possible == 0) {
					possible = DictionaryHandler::GetPossibles(possibles, res2).size();
#pragma omp critical
					possibleTable[combinedId] = possible;
				}
			}

#pragma omp critical
			{
				averages[j] += possible / double(size);
				if (progress++ % 1235233 == 0) {
					std::chrono::duration<float> timeDiff = std::chrono::system_clock::now() - start;
					std::cout << double(progress) / double(maxProgress) << " " << (timeDiff.count() / progress) * (maxProgress - progress) << " seconds left" << "                                    \r";
				}
			}
		}
	}

	delete[] possibleTable;
	double average = *std::min_element(averages.begin(), averages.end());
	return average * chanceWrong;
}