#pragma once
#include <string>
#include <map>
class CheckResult
{
public:
	std::string word;
	std::string result;
	int greens = 0;
	int yellows = 0;
	static const int maxId = (3 * 3 * 3 * 3 * 3);

	CheckResult() : greens(0), yellows(0) {}

	CheckResult(std::string word, std::string result) : word(word), result(result) {
		greens = 0;
		yellows = 0;
		for (int i = 0; i < 5; i++)
		{
			switch (result[i]) {
			case '1':
				yellows++;
				break;
			case '2':
				greens++;
				break;
			}
		}
	}

	CheckResult(std::string word, std::string result, int greens, int yellows) : word(word), result(result), greens(greens), yellows(yellows) {}

	static CheckResult CalculateResult(std::string guess, std::string answer);

	bool IsPossible(std::string other) const;

	int Id() {
		if (id == -1) {
			id = 1 + ((result[0] - '0') * 1)
				+ ((result[1] - '0') * 3)
				+ ((result[2] - '0') * 3*3)
				+ ((result[3] - '0') * 3*3*3)
				+ ((result[4] - '0') * 3*3*3*3);
		}
		return id;
	}
private:
	int id = -1;
	static std::map<std::pair<std::string, std::string>, std::string> memoized;
	static const bool first = true;
};
