#pragma once
#include "AverageSolver.h"
#include "DictionaryHandler.h"
#include "ProgressBar.h"
#include "Wordle.h"
class Cheat
{
public:
	AverageSolver solver;
	vector<string> answers;
	Cheat(vector<string> dict, vector<string> answers) : solver(dict), answers(answers) {}
	Cheat(vector<string> dict) : Cheat(dict, dict) {}

	void loop();
	
	static void PrintState(char* state) {
		for (int i = 0; i < 5*6; i++)
		{
			std::cout << state[i];
			if (i % 5 == 4) std::cout << std::endl;
		}
	}

private:
	char* GetState();

	int GetLatestResultIndex(char* state);

	string GetLatestResult(char* state, int index);

	void InputAnswer(string answer);
};

