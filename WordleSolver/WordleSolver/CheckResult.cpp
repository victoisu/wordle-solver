#include "CheckResult.h"


std::map<std::pair<std::string, std::string>, std::string> CheckResult::memoized = {};

CheckResult CheckResult::CalculateResult(std::string guess, std::string answer)
{
	auto pair = std::make_pair(guess, answer);
	if (CheckResult::memoized.contains(pair)) {
		return CheckResult(guess, memoized[pair]);
	}

	std::string initial = guess;
	int greens = 0, yellows = 0;

	for (int i = 0; i < 5; i++)
	{
		if (guess[i] == answer[i]) {
			guess[i] = '2';
			answer[i] = '2';
			greens++;
		}
	}

	for (int i = 0; i < 5; i++)
	{
		if (guess[i] == '2') continue;

		for (int j = 0; j < 5; j++)
		{
			if (i != j && guess[i] == answer[j]) {
				guess[i] = '1';
				answer[j] = '1';
				yellows++;
				break;
			}
		}

		if (guess[i] != '1') {
			guess[i] = '0';
		}
	}

	for (int i = 0; i < 5; i++)
	{
		if (guess[i] != '1' && guess[i] != '2') {
			guess[i] = '0';
		}
	}

	CheckResult::memoized[pair] = guess;

	return CheckResult(guess, memoized[pair], greens, yellows);
}

bool CheckResult::IsPossible(std::string other) const
{
	std::string init_self = word;

	// Greens
	for (int i = 0; i < 5; i++)
	{
		bool green = result[i] == '2';
		bool equal = init_self[i] == other[i];
		if (green != equal) {
			return false;
		} 

		if (green && equal) {
			other[i] = '2';
			init_self[i] = '2';
		}
	}

	// Yellows
	for (int i = 0; i < 5; i++)
	{
		if (result[i] != '1') continue;

		bool present = false;
		for (int j = 0; j < 5; j++)
		{
			if (init_self[i] == other[j]) {
				other[j] = '1';
				present = true;
				break;
			}
		}

		if (!present) {
			return false;
		}
	}

	// Grays
	for (int i = 0; i < 5; i++)
	{
		if (result[i] != '0') continue;

		bool present = false;
		for (int j = 0; j < 5; j++)
		{
			if (init_self[i] == other[j]) {
				return false;
			}
		}
	}

	return true;
}
