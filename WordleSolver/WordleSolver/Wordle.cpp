#include "Wordle.h"
#include <random>

void Wordle::PlayGameKnowingAnswer(std::vector<std::string> dict, std::string answer, std::string startingGuess, bool hard)
{
	std::cout << "Trying to guess " << answer << std::endl;
	Minimax::Solve(dict, answer, startingGuess, hard);
}

void Wordle::RandomMinimaxGame(std::vector<std::string> dict, std::string startingGuess, bool hard) {
	int index = rand() % dict.size();
	auto answer = dict[index];

	PlayGameKnowingAnswer(dict, answer, startingGuess, hard);
}

void Wordle::ManualPlayKnown(std::vector<std::string> dict, std::string answer) {
	std::string response;
	while (dict.size() > 1) 
	{
		std::cout << dict.size() << " possible.\n";
		std::cin >> response;
		auto result = CheckResult::CalculateResult(response, answer);
		std::cout << result.result << std::endl;
		dict = DictionaryHandler::GetPossibles(dict, result);
	}
}

void Wordle::ManualPlayUnknown(std::vector<std::string> dict, std::string startingGuess, bool hard, std::pair<std::string, double>(*func)(std::vector<std::string>, std::vector<std::string>))
{
	double score = -1.0;
	if (startingGuess == "") {
		auto top = func(dict, dict);
		startingGuess = top.first;
		score = top.second;
	}

	std::string answer;
	auto remaining = dict;
	while (true) {
		std::cout << remaining.size() << " remaining. Score: " << std::to_string(score).substr(0, 10) << ". " << DictionaryHandler::FirstN(remaining, 10) << std::endl << "Top guess: " << startingGuess << std::endl << "           ";
		std::cin >> answer;
		if (answer == "22222") break;
		auto res = CheckResult(startingGuess, answer);
		remaining = DictionaryHandler::GetPossibles(remaining, res);
		if (remaining.size() == 1) {
			startingGuess = remaining[0];
			score = 1.0;
		}
		else if (remaining.size() == 0) 
		{
			std::cout << "Not found?" << std::endl;
			return;
		}
		else
		{
			auto guess = func(dict, remaining);
			startingGuess = guess.first;
			score = guess.second;
		}
	}
}

void Wordle::ManualPlayMinimaxUnknown(std::vector<std::string> dict, std::string startingGuess, bool hard) 
{
	ManualPlayUnknown(dict, startingGuess, hard, Minimax::TopEasyMode);
}

void Wordle::ManualPlayAverageUnknown(std::vector<std::string> dict, std::string startingGuess, bool hard)
{
	ManualPlayUnknown(dict, startingGuess, hard, Average::TopEasyMode);
}

void Wordle::ManualPlayUnknown(std::vector<std::string> dict, std::vector<std::string> answers)
{
	while (true) {
		std::string word, ans;
		std::cout << "Guess:  ";
		std::cin >> word;
		if (word == "2") return;
		std::cout << "Result: ";
		std::cin >> ans;
		if (ans == "2" || ans == "22222") return;
		auto res = CheckResult(word, ans);
		answers = DictionaryHandler::GetPossibles(answers, res);
		std::cout << "Remaining: " << answers.size() << std::endl << DictionaryHandler::FirstN(answers, 6) << std::endl;
	}
}

void Wordle::Compete(int trials, int words)
{
	std::vector<std::string> dict = DictionaryHandler::GetDictionary("twl.txt"), sample;
	words = words == 0 ? dict.size() : words;
	std::sample(dict.begin(), dict.end(), std::back_inserter(sample), words, std::mt19937{ std::random_device{}() });
	dict = sample;

	trials = trials > words ? words : trials;
	std::vector<int> indices(words), tests;
	std::iota(indices.begin(), indices.end(), 0);
	std::sample(indices.begin(), indices.end(), std::back_inserter(tests), trials, std::mt19937{ std::random_device{}() });

	std::cout << "Competing with a " << dict.size() << " word dictionary over a " << trials << " trial competition." << std::endl;
	auto topMini = Minimax::TopEasyMode(dict, dict);
	std::cout << "Top minimax starter: " << topMini.first << ". Score: " << topMini.second << std::endl;
	auto topAverage = Average::TopEasyMode(dict, dict);
	std::cout << "Top average starter: " << topAverage.first << ". Score: " << topAverage.second << std::endl;
	int miniItScore = 0;
	int miniTimeScore = 0;
	int averageItScore = 0;
	int averageTimeScore = 0;
	int miniMax = 0;
	int averageMax = 0;
	std::string miniBig, averageBig;


	system("PAUSE");
	for (int i = 0; i < trials; i++)
	{
		int index = rand() % dict.size();
		std::string answer = dict[tests[i]];
		//std::cout << "Trying " << answer << std::endl << "Minimax...\r";
		auto start = std::chrono::system_clock::now();
		int miniIts = Minimax::Solve(dict, answer, topMini.first, false);
		//int miniIts = Minimax::Solve(dict, answer, "serai", false);
		//std::cout << "Average...\r";
		auto mid = std::chrono::system_clock::now();
		int averageIts = Average::Solve(dict, answer, topAverage.first, false);
		//int averageIts = Average::Solve(dict, answer, "lares", false);
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<float> diffMini = mid - start;
		std::chrono::duration<float> diffAverage = end - mid;

		//std::cout << "Mini:    " << miniIts << " iterations. Time: " << diffMini << std::endl;
		//std::cout << "Average: " << averageIts << " iterations. Time: " << diffAverage << std::endl;
		
		if (i % 127 == 0) {
			std::cout << i << "/" << trials << "\r";
		}

		if (miniIts > miniMax) {
			miniBig = answer;
			miniMax = miniIts;
		}

		if (averageIts > averageMax) {
			averageBig = answer;
			averageMax = averageIts;
		}

		if (miniIts > averageIts) {
			averageItScore++;
		}
		else if (averageIts > miniIts) {
			miniItScore++;
		}

		if (diffAverage > diffMini) {
			averageTimeScore++;
		}
		else if (diffMini > diffAverage) {
			miniTimeScore++;
		}
	}
	std::cout << "\nFinal score:\nIterations: Mini " << miniItScore << " - " << averageItScore << " Average\nTime:            " 
		<< miniTimeScore << " - " << averageTimeScore << "\nMax mini:    " << miniMax << " " << miniBig << "\nMax average: " << averageMax << " " << averageBig << std::endl;
}

void Wordle::FullAnalyze(int words)
{
	std::vector<std::string> dict = DictionaryHandler::GetDictionary("twl.txt"), sample;
	words = words == 0 ? dict.size() : words;
	std::sample(dict.begin(), dict.end(), std::back_inserter(sample), words, std::mt19937{ std::random_device{}() });
	dict = sample;

	std::cout << "Calculating top words...\n";
	auto topMini = Minimax::TopEasyMode(dict, dict).first, topAverage = Average::TopEasyMode(dict, dict).first;
	//std::string topMini = "serai", topAverage = "lares";
	std::ofstream output;
	output.open("output.txt", std::ios::out | std::ios::trunc);


	for (int i = 0; i < words; i++)
	{
		auto answer = dict[i];

		auto start = std::chrono::system_clock::now();
		int mini = Minimax::Solve(dict, answer, topMini, false);
		auto mid = std::chrono::system_clock::now();
		int average = Average::Solve(dict, answer, topAverage, false);
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<float> diffMini = mid - start;
		std::chrono::duration<float> diffAverage = end - mid;

		std::cout << i << ": " << answer << "\t" << mini << "\t" << average << "\t" << diffMini.count() << "\t" << diffAverage.count() << std::endl;
		output << answer << "\t" << mini << "\t" << average << "\t" << diffMini.count() << "\t" << diffAverage.count() << std::endl;
	}
	std::cout << topMini << "\t" << topAverage << std::endl;
	output.close();
}
