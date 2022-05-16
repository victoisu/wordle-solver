#pragma once
#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "CheckResult.h"
#include "DictionaryHandler.h"
using std::string;
using std::vector;

class Solver
{
public:
	virtual std::pair<string, double> TopGuessEasy(const vector<string> &remaining) = 0;

	virtual std::pair<string, double> TopGuessHard() = 0;

	virtual vector<std::pair<string, double>> AllGuessesEasy(const vector<string> &remaining) = 0;

	virtual vector<std::pair<string, double>> AllGuessesHard() = 0;

	virtual double Score(const string word) = 0;

	virtual double Score(const string word, const vector<string> remaining) = 0;

	virtual vector<string> GetDictionary() const = 0;

	void ManualPlayGraded(vector<string> answers) {
		bool find = false;

		while (true) {
			if (find)
				std::cout << "Remaining: " << answers.size() << std::endl
				<< Solver::ResultString(6, AllGuessesEasy(answers)) << std::endl;
			find = true;
			std::string word, ans;
			std::cout << "Guess:  ";
			std::cin >> word;
			if (word == "2") return;
			std::cout << "Result: ";
			std::cin >> ans;
			if (ans == "2" || ans == "22222") return;
			auto res = CheckResult(word, ans);
			answers = DictionaryHandler::GetPossibles(answers, res);
		}
	}

	static void SolveKnown(string answer, Solver* solver, string initialGuess="") {
		std::cout << "Trying to guess: " << answer << std::endl;
		auto dict = solver->GetDictionary();
		//std::cout << dict.size() << " remaining\t ";
		if (initialGuess == "")
			initialGuess = solver->TopGuessHard().first;
		//std::cout << dict.size() << " remaining\t " << "Guessing: " << initialGuess << std::endl;
		SolveRecursive(answer, solver, dict, initialGuess);
	}

	static void SolveUnknown(Solver* solver, string initialGuess) {
		auto remaining = solver->GetDictionary();
		
		SolveUnknown(solver, initialGuess, remaining);
	}

	static void SolveUnknown(Solver* solver, string initialGuess, vector<string> answers) 
	{
		if (initialGuess == "") {
			auto guess = solver->AllGuessesEasy(answers);
			initialGuess = guess[0].first;
			std::cout << Solver::ResultString(6, guess) << std::endl;
		}

		string ans;
		while (true) {
			std::cout << "Guess:  " << initialGuess << ". " << answers.size() << " remaining." << std::endl << "Answer: ";
			std::cin >> ans;
			if (ans == "22222" || ans == "2") break;
			auto res = CheckResult(initialGuess, ans);
			answers = DictionaryHandler::GetPossibles(answers, res);
			if (answers.size() == 1) {
				initialGuess = answers[0];
			}
			else if (answers.size() == 0)
			{
				std::cout << "Not found?" << std::endl;
				return;
			}
			else
			{
				auto guess = solver->AllGuessesEasy(answers);
				std::cout << Solver::ResultString(10, guess) << std::endl;
				initialGuess = guess[0].first;
			}
		}
	}

	static string ResultString(int n, vector<std::pair<string, double>> res) {
		int size = res.size() > n ? n : res.size();
		std::stringstream ss;
		int i = 0;
		while(true)
		{
			ss << "[" << res[i].first << ": " << res[i].second << "]";
			if (++i == size) break;
			ss << ",";
		}
		return ss.str();
	}
private:
	static void SolveRecursive(string answer, Solver* solver, vector<string> remaining, string guess) {
		if (remaining.size() == 0)
			return;
		std::cout << remaining.size() << " remaining\t " << "Guessing: " << guess << std::endl;
		auto check = CheckResult::CalculateResult(guess, answer);
		auto possibles = DictionaryHandler::GetPossibles(remaining, check);
		//if (possibles.size() > 0)/*
		//	std::cout << possibles.size() << " remaining\t ";*/
		auto newGuess = solver->TopGuessEasy(possibles);
		SolveRecursive(answer, solver, possibles, newGuess.first);
	}
};

