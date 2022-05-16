#include "Runner.h"

static const int THREADS = 6;
static const int WORDS = 0;
static const char* DICTIONARYNAME = "twl.txt";
#include <iomanip>
#include "Solver.h"
#include "AverageSolver.h"
#include "DoubleAverageSolver.h"
#include "Cheat.h"
#include "MaximumGreens.h"

int main(int argc, char **argv)
{
	// Top word for basic average is "lares"
	// Top word for minimax is "serai"
	srand(time(NULL));
	int threads = THREADS > 0 ? THREADS : omp_get_max_threads();
	omp_set_nested(1);
	omp_set_num_threads(threads);

	std::vector<std::string> dict = DictionaryHandler::GetDictionary(DICTIONARYNAME), sample;
	std::vector<std::string> answers = DictionaryHandler::GetDictionary("answers.txt");
	//std::sample(answers.begin(), answers.end(), std::back_inserter(sample), answers.size(), std::mt19937{ std::random_device{}() });
	//answers = sample;
	//answers.resize(100);

	AverageSolver average = AverageSolver(dict);
	DoubleAverageSolver dsolver = DoubleAverageSolver(dict);
	//const string answer = dict[rand() % dict.size()];
	//const string answer = "breed";
	//while (true) {
		//string answer = dict[rand() % dict.size()];
	//	try {
			//Solver::SolveUnknown(&average, "house", dict);
			//Solver::SolveKnown(answer, &average, "raise");
			//Wordle::ManualPlayUnknown(dict);
	//average.TopGuessEasy(answers);
	//std::cout << MaximumGreens::Score("serai", "ducot", dict) << std::endl;
	MaximumGreens::FindMaxPair(dict, dict);
	//		average.ManualPlayGraded(dict);
	//		std::cout << "===========\n";
	//		break;
	//	}
	//	catch(const std::exception& e)
	//	{

	//	}
	//}
	//Solver::SolveKnown(answer, &dsolver, "");

	//Cheat cheater(dict);
	//cheater.loop();

	std::cout << std::endl;
	system("PAUSE");
}
