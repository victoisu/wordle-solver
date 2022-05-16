#include "ProgressBar.h"
#include <iostream>

std::string ProgressBar::CreateProgressBar(int current, int max)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left - 7;

    double progress = (double)current / max;
    int barcount = int(progress * columns);
    int emptycount = columns - barcount;
    std::string percent =  std::to_string(int(progress * 100)) + "%";
    while (percent.size() < 4) {
        percent = " " + percent;
    }

    std::string bars(barcount, '=');
    std::string empty(emptycount, ' ');

    return std::string(percent + " [" + bars + empty + "]");
}
