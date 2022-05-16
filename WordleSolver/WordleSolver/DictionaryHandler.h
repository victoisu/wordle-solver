#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include "CheckResult.h"
using std::string;
using std::vector;

class DictionaryHandler
{
public:
	static vector<string> GetDictionary(string fileName);

	static vector<string> SortByPopular(vector<string> toSort, vector<string> popular);

	static vector<string> GetPossibles(const vector<string> &dict, const CheckResult &checkResult);
	
	static string FirstN(vector<string> dict, int n) {
		int size = dict.size();
		int length = size > n ? n : dict.size();
		std::stringstream stream;
		for (int i = 0; i < length; i++)
		{
			stream << dict[i];
			if (i != length - 1) {
				stream << ", ";
			}
		}

		if (size > length)
			stream << "...";

		return stream.str();
	}
	
};

