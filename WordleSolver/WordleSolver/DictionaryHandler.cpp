#include "DictionaryHandler.h"
#include "ProgressBar.h"

std::vector<std::string> DictionaryHandler::GetDictionary(std::string fileName)
{
	std::vector<std::string> words;
	std::ifstream dictFile(fileName);
	std::string line;
	while (std::getline(dictFile, line)) {
		if (line.length() == 5) {
			words.push_back(line.substr(0, 5));
		}
	}
	return words;
}

std::vector<std::string> DictionaryHandler::SortByPopular(std::vector<std::string> toSort, std::vector<std::string> popular)
{
	std::vector<std::string> res;

	for (int i = 0; i < toSort.size(); i++)
	{
		for (int j = 0; j < popular.size(); j++)
		{
			if (toSort[i] == popular[j]) {
				res.push_back(toSort[i]);
				break;
			}
		}
	}

	return res;
}

std::vector<std::string> DictionaryHandler::GetPossibles(const std::vector<std::string> &dict, const CheckResult &checkResult)
{
	std::vector<std::string> words;
	std::string word;
	for (int i = 0; i < dict.size(); i++)
	{
		word = dict[i];
		if (word != checkResult.word && checkResult.IsPossible(word)) {
			words.push_back(word);
		}
	}

	return words;
}