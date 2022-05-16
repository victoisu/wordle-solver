#include "Cheat.h"
#include <chrono>
#include <thread>

void Cheat::loop()
{
	const string start = "lares";
	int last = -2;
	string current = start;
	vector<string> remaining = solver.dict;
	std::cout << "Get game going, then continue.";
	std::getchar();
	std::cout << "Starting!!! Get focused!!!\n";
	std::this_thread::sleep_for(std::chrono::seconds(3));
	//InputAnswer("ready");
	while (true) {
		auto state = GetState();
		if (state[0] == 'W') return;
		//Cheat::PrintState(state);
		int resultIndex = GetLatestResultIndex(state);
		if (last == resultIndex) continue;

		if (resultIndex == -1 || resultIndex < last) {
			string res = GetLatestResult(state, 0);
			if (res == "EEEEE") {
				current = start;
				remaining = solver.dict;
				std::cout << "==========\n" << current << std::endl;
				InputAnswer(current);
			}
			else 
			{
				std::cout << "Here:" << res << std::endl;
				continue;
			}
		}
		else if (resultIndex >= 0)
		{
			string result = GetLatestResult(state, resultIndex);
			std::cout << resultIndex << " " << result << std::endl;
			if (resultIndex == 25 || result == "22222") {
				continue;
			}
			CheckResult check(current, result);
			remaining = DictionaryHandler::GetPossibles(remaining, check);
			if (remaining.size() == 0) continue;
			auto solved = solver.AllGuessesEasy(remaining);
			current = solved[0].first;
			std::cout << current << std::endl;
			InputAnswer(current);
		}
		last = resultIndex;
	}
}

char* Cheat::GetState()
{
	char result[5 * 6];
	const int top = 588;
	const int left = 1110;
	const int spacing = 74;

	// Green:   46/216/ 60
	// Yellow: 214/190/  0
	// None:   115/ 93/247
	// Empty:  167/113/248
	// Won if 1170, 140 R value = 65

	HDC dc = GetDC(NULL);
	COLORREF win = GetPixel(dc, 1170, 140);
	int winred = GetRValue(win);
	if (winred == 65) {
		result[0] = 'W';
		return result;
	}
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			int x = left + i * spacing, y = top + j * spacing;
			int index = i + j * 5;
			COLORREF color = GetPixel(dc, x, y);
			int red = GetRValue(color);
			switch (red) {
			case 46:
				result[index] = '2';
				break;
			case 214:
				result[index] = '1';
				break;
			case 155:
				result[index] = '0';
				break;
			case 167:
				result[index] = 'E';
				break;
			default:
				result[index] = 'U';
				break;
			}
		}
	}
	ReleaseDC(NULL, dc);

	return result;
}

int Cheat::GetLatestResultIndex(char* state)
{
	int i = 0;
	for (; i < 30; i += 5) {
		for (int j = 0; j < 5; j++)
		{
			char current = state[i + j];
			if (current == 'U' || current == 'W') {
				return -1;
			}
			else if (current == 'E') {
				return i > 0 ? i - 5 : -1;
			}
		}
	}
	return i;
}

string Cheat::GetLatestResult(char* state, int index)
{

	return string(state + index, state + index + 5);
}

void Cheat::InputAnswer(string answer)
{
	//INPUT ip;
	//// Set up a generic keyboard event.
	//ip.type = INPUT_KEYBOARD;
	//ip.ki.wScan = 0; // hardware scan code for key
	//ip.ki.time = 0;
	//ip.ki.dwExtraInfo = 0;

	//// Press the "A" key
	//ip.ki.wVk = 0x41; // virtual-key code for the "a" key
	//ip.ki.dwFlags = 0; // 0 for key press
	//SendInput(1, &ip, sizeof(INPUT));

	INPUT input;
	INPUT inputs[2];
	for (int i = 0; i < 5; i++)
	{
		char current = answer[i];
		int ascii = 0x41 + current - 'a';
		input.type = INPUT_KEYBOARD;
		input.ki.wScan = 0;
		input.ki.time = 0;
		input.ki.dwExtraInfo = 0;

		input.ki.wVk = ascii;
		input.ki.dwFlags = 0;
		inputs[0] = input;
		input.ki.dwFlags = KEYEVENTF_KEYUP;
		inputs[1] = input;
		SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
		std::this_thread::sleep_for(std::chrono::milliseconds(34));
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(34));
	INPUT input2;
	input2.type = INPUT_KEYBOARD;
	input2.ki.wScan = MapVirtualKey(VK_RETURN, MAPVK_VK_TO_VSC);
	input2.ki.time = 0;
	input2.ki.dwExtraInfo = 0;

	input2.ki.wVk = VK_RETURN;
	input2.ki.dwFlags = 0;

	SendInput(1, &input2, sizeof(INPUT));
	std::this_thread::sleep_for(std::chrono::milliseconds(34));
	input2.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input2, sizeof(INPUT));
}
