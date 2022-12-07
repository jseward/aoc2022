#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <optional>
#include <array>
#include <algorithm>
#include <map>
#include <deque>
#include <sstream>

void day8_1()
{
	std::vector<std::string> lines;
	std::ifstream file("day8.input");
	std::string line;
	while (std::getline(file, line))
	{
		lines.push_back(line);
	}
}

void day8_2()
{
}

void day8()
{
	day8_1();
	day8_2();
}
