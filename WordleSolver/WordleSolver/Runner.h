#pragma once
#include <omp.h>
#include <chrono>
#include <random>
#include "DictionaryHandler.h"
#include "ProgressBar.h"
#include "Wordle.h"
#include "Minimax.h"
#include "Average.h"
#include "Goofy.h"
class Runner
{
public:
	static void GetTopWordMinimax(const char* dictionaryName = "twl.txt", int maxWords = 0) {

		auto start = std::chrono::system_clock::now();
		std::vector<std::string> dict = DictionaryHandler::GetDictionary(dictionaryName);
		if (maxWords > 0) dict.resize(maxWords);
		std::cout << "Generating top guess out of " << dict.size() << " words with " << omp_get_num_threads() << " threads." << std::endl;
		auto top = Minimax::TopEasyMode(dict, dict);

		auto end = std::chrono::system_clock::now();
		std::chrono::duration<float> diff = end - start;
		std::cout << "Finished " << dict.size() << " words in " << diff.count() << " seconds.\n";
		std::cout << "Top guess: " << top.first << std::endl << "Eliminates " << top.second * 100 << "% of words using Minimax.";
	}


	static void GetAllGuessesMinimax(const char* dictionaryName = "twl.txt", int maxWords = 0) {

		auto start = std::chrono::system_clock::now();
		std::vector<std::string> dict = DictionaryHandler::GetDictionary(dictionaryName);
		if (maxWords > 0) dict.resize(maxWords);
		std::cout << "Generating all guesses out of " << dict.size() << " words with " << omp_get_num_threads() << " threads using Minimax." << std::endl;

		auto guesses = Minimax::AllGuesses(dict);
		auto top = guesses[0];
		for (int i = 0; i < dict.size(); i++)
		{
			std::cout << "[\"" << guesses[i].first << "\"" << ": " << guesses[i].second << "], ";
		}
		std::cout << std::endl;

		auto end = std::chrono::system_clock::now();
		std::chrono::duration<float> diff = end - start;
		std::cout << "Finished " << dict.size() << " words in " << diff.count() << " seconds.\n";
		std::cout << "Top guess: " << top.first << std::endl << "Eliminates " << top.second * 100 << "% of words.";
	}

	static void GetTopWordAverage(const char* dictionaryName = "twl.txt", int maxWords = 0) {
		auto start = std::chrono::system_clock::now();
		std::vector<std::string> dict = DictionaryHandler::GetDictionary(dictionaryName);
		if (maxWords > 0) dict.resize(maxWords);
		std::cout << "Generating top guess out of " << dict.size() << " words with " << omp_get_num_threads() << " threads using Average." << std::endl;
		auto top = Average::TopEasyMode(dict, dict);

		auto end = std::chrono::system_clock::now();
		std::chrono::duration<float> diff = end - start;
		std::cout << "Finished " << dict.size() << " words in " << diff.count() << " seconds.\n";
		std::cout << "Top guess: " << top.first << std::endl << "Eliminates " << top.second * 100 << "% of words.";
	}

	static void GetAllGuessesAverage(const char* dictionaryName = "twl.txt", int maxWords = 0) {
		auto start = std::chrono::system_clock::now();
		std::vector<std::string> dict = DictionaryHandler::GetDictionary(dictionaryName);
		if (maxWords > 0) dict.resize(maxWords);
		std::cout << "Generating all guesses out of " << dict.size() << " words with " << omp_get_num_threads() << " threads using Average." << std::endl;

		auto guesses = Average::AllGuesses(dict);
		auto top = guesses[0];
		for (int i = 0; i < dict.size(); i++)
		{
			std::cout << "[\"" << guesses[i].first << "\"" << ": " << guesses[i].second << "], ";
		}
		std::cout << std::endl;

		auto end = std::chrono::system_clock::now();
		std::chrono::duration<float> diff = end - start;
		std::cout << "Finished " << dict.size() << " words in " << diff.count() << " seconds.\n";
		std::cout << "Top guess: " << top.first << std::endl << "Eliminates " << top.second * 100 << "% of words on average.";
	}

	static void Goofy(std::vector<std::string> dict) {
		//Goofy::Solve(dict, "tangy", "oxbow", true);

		//auto res = Goofy::AvoidGreen(dict, false, dict);
		//res.resize(10);
		//std::cout << ResultString(res) << std::endl;

		std::vector<std::string> sample = dict;

		for (int k = 0; k < 1000; k++)
		{
			//sample = {};
			//std::sample(dict.begin(), dict.end(), std::back_inserter(sample), 400, std::mt19937{ std::random_device{}() });
			std::string answer = sample[rand() % sample.size()];
			std::vector<std::string> res;
			bool found = Goofy::SolveMaxGray(sample, "tangy", res);
			if (!found)
				continue;

			for (int i = 0; i < res.size(); i++)
			{
				std::cout << res[i] << " ";
			}
			std::cout << "\n================" << std::endl;
		}
	}

	static std::string ResultString(std::vector<std::pair<std::string, double>> guesses ) {
		std::stringstream stream;
		int mini = min(guesses.size(), 10);
		for (int i = 0; i < mini; i++)
		{
			stream << "[\"" << guesses[i].first << "\"" << ": " << guesses[i].second << "], ";
		}
		return stream.str();
	}
};

