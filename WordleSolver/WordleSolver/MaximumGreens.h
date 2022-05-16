#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include "CheckResult.h"
using std::string;
using std::vector;
class MaximumGreens
{
public:
	static std::pair<string, string> FindMaxPair(const vector<string>& dict, const vector<string>& answers) {
		int size = dict.size();
		int max = -1;
		string _first = "";
		string _second = "";
		int count = 0;
#pragma omp parallel for
		for (int i = 0; i < size; i++)
		{
#pragma omp parallel for
			for (int j = 0; j < size; j++)
			{
				string first = dict[i];
				string second = dict[j];
				int score = Score(first, second, answers);
#pragma omp critical
				{
					count++;
					if (count % 12131 == 0)
						std::cout << count << "/" << size * size << "\r";
					if (score > max) {
						max = score;
						_first = first;
						_second = second;
						std::cout << first << "/" << second << "=" << score << std::endl;
					}
				}
			}
		}

		return std::make_pair(_first, _second);
	}

	static int Score(string first, string second, const vector<string> &answers) {
		vector<int> res;
#pragma omp parallel for
		for (int i = 0; i < answers.size(); i++)
		{
			auto check1 = CheckResult::CalculateResult(first, answers[i]);
			auto check2 = CheckResult::CalculateResult(second, answers[i]);

			int greens = 0;
			for (int i = 0; i < 5; i++)
			{
				if (check1.result[i] == '2')
					greens++;
			}

#pragma omp critical
			res.push_back(greens);
		}

		std::sort(res.begin(), res.end(), [](const auto& a, const auto& b) -> bool {
			return a < b;
			});

		return res[answers.size()/2];
	}
};

