#include "Average.h"
#include <chrono>
#include <iomanip>
#include "Runner.h"

std::pair<std::string, double> Average::TopGuess(std::vector<std::string> dict)
{
	return AllGuesses(dict, true, std::vector<std::string>())[0];
}

std::vector<std::pair<std::string, double>> Average::AllGuesses(std::vector<std::string> dict)
{
	return AllGuesses(dict, true, std::vector<std::string>());
}

std::pair<std::string, double> Average::TopEasyMode(std::vector<std::string> dict, std::vector<std::string> remaining)
{
	return AllGuesses(dict, false, remaining)[0];
}

std::pair<std::string, double> Average::TopDouble(std::vector<std::string> dict, std::vector<std::string> remaining)
{
	auto a = AllDouble(dict, remaining);
	std::cout << Runner::ResultString(a) << std::endl;
	return AllDouble(dict, remaining)[0];
}

std::vector<std::pair<std::string, double>> Average::AllDouble(std::vector<std::string> dict, std::vector<std::string> remaining)
{
	int size = dict.size(), rsize = remaining.size();
	std::vector<std::pair<std::string, double>> result(size);
	int progress = 0;
	double max_prog = (double)size * (double)size;
	if (rsize == 1)
		return std::vector<std::pair<std::string, double>>(1, std::make_pair(remaining[0], 1.0));

	auto start = std::chrono::system_clock::now();

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cursorPosition = { 0, 0 };
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	if (GetConsoleScreenBufferInfo(handle, &cbsi))
	{
		cursorPosition = cbsi.dwCursorPosition;
	}
	else
	{
		std::cout << "something bad!" << std::endl;
	}
	

#pragma omp parallel for
	for (int i = 0; i < size; i++)
	{
		int *possibleTable = new int[CheckResult::maxId * CheckResult::maxId + 1] {};
		auto guess1 = dict[i];
		double average = 0.0;

		for (int j = 0; j < size; j++)
		{
			int possibles;
			auto answer = dict[j];
			auto res1 = CheckResult::CalculateResult(guess1, answer);
			auto possibles1 = DictionaryHandler::GetPossibles(remaining, res1);

			for (int k = 0; k < rsize; k++)
			{
				auto guess2 = dict[k];

				if (guess1 == answer) 
				{
					possibles = 0;
				}
				else if (guess2 == answer)
				{
					possibles = 1;
				}
				else
				{
					auto res2 = CheckResult::CalculateResult(guess2, answer);
					int combinedId = (res1.Id() - 1) * CheckResult::maxId + res2.Id();
					if (possibleTable[combinedId] == 0)
						possibleTable[combinedId] = DictionaryHandler::GetPossibles(possibles1, res2).size();
					possibles = possibleTable[combinedId];
				}

				average += (double)possibles/((double)rsize * (double)rsize * (double)size * (double)size);
			}
#pragma omp critical
			{
				progress++;
				if (progress % 127 == 0) {
					std::chrono::duration<float> timeDiff = std::chrono::system_clock::now() - start;
					std::cout << std::setprecision(4) << std::setw(30) << progress / max_prog << ", " << size << " " << (timeDiff.count() / progress) * (max_prog - progress) << " seconds left";
					SetConsoleCursorPosition(handle, cursorPosition);
				}
			}
		}
		delete[] possibleTable;

		result[i] = std::make_pair(guess1, average);
	}
	std::cout << "                                                                                                    ";
	SetConsoleCursorPosition(handle, cursorPosition);

	std::sort(result.begin(), result.end(), [](const auto& a, const auto& b) -> bool {
		return a.second < b.second;
		});

	return result;
}

int Average::Solve(std::vector<std::string> dict, std::string answer, std::string guess, bool hard)
{
	auto remaining = dict;
	int iterations = 1;
	std::cout << "Remaining: " << remaining.size() << " ";
	if (guess == "")
		guess = AllGuesses(dict, hard, dict)[0].first;

	while (true) {
		std::cout << "Guess: " << guess << std::endl;
		if (guess == answer) {
			return iterations;
		}

		iterations++;
		auto result = CheckResult::CalculateResult(guess, answer);
		remaining = DictionaryHandler::GetPossibles(remaining, result);
		std::cout << "Remaining: " << remaining.size() << " ";
		guess = AllGuesses(hard ? remaining : dict, hard, remaining)[0].first;
	}

	return -1;
}

int Average::SolveDouble(std::vector<std::string> dict, std::string answer, std::string guess)
{
	auto remaining = dict;
	int iterations = 1;
	std::cout << "Remaining: " << remaining.size() << " ";
	if (guess == "")
		guess = TopDouble(dict, remaining).first;

	while (true) {
		std::cout << "Guess: " << guess << std::endl;
		if (guess == answer) {
			return iterations;
		}

		iterations++;
		auto result = CheckResult::CalculateResult(guess, answer);
		remaining = DictionaryHandler::GetPossibles(remaining, result);
		std::cout << "Remaining: " << remaining.size() << " ";
		guess = TopDouble(dict, remaining).first;
	}

	return -1;
}

std::vector<std::pair<std::string, double>> Average::AllGuesses(std::vector<std::string> dict, bool hard, std::vector<std::string> remaining)
{
	//std::cout << ProgressBar::CreateProgressBar(0, 1) << "\r";
	remaining = hard ? dict : remaining;
	int size = dict.size();
	int rsize = remaining.size();
	std::vector<std::pair<std::string, double>> result(size);
	double lowest = 2.0;

	int progress = 0;
	int max_prog = size * size;
	if (rsize == 1)
		return std::vector<std::pair<std::string, double>>(1, std::make_pair(remaining[0], 1.0));


#pragma omp parallel for
	for (int i = 0; i < size; i++)
	{
		std::string guess = dict[i];
		std::string answer;
		CheckResult res;
		int count = 0, possibles = 0, resId;
		double average = 0.0;
		double lowest_possible;
		int possibleTable[CheckResult::maxId + 1] = {};

		for (int j = 0; j < rsize; j++)
		{
			answer = remaining[j];
			if (guess == answer) {
				possibles = 0;
			}
			else {
				res = CheckResult::CalculateResult(guess, answer);
				resId = res.Id();
				if (possibleTable[resId] == 0) {
					possibleTable[resId] = DictionaryHandler::GetPossibles(remaining, res).size();
				}
				possibles = possibleTable[resId];
			}

			average += double(possibles)/(double(rsize) * double(size));

//#pragma omp atomic
//			progress++;
//			if (progress % 127 == 0) {
//#pragma omp critical
//				std::cout << ProgressBar::CreateProgressBar(progress, size*rsize) << "\r";
//			}
		}

		result[i] = std::make_pair(guess, 1.0 - average);
	}
	//std::cout << ProgressBar::CreateProgressBar(1, 1) << std::endl;

	std::sort(result.begin(), result.end(), [](const auto& a, const auto& b) -> bool {
		return a.second > b.second;
		});

	return result;
}
