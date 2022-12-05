#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <optional>
#include <array>
#include <algorithm>
#include <map>
#include <deque>
#include <regex>

struct op
{
	int move = 0;
	int from = 0;
	int to = 0;
};

void day5_1()
{
	std::array<std::deque<char>, 9> stacks;

	std::ifstream file("day5.input");
	std::string line;
	while (std::getline(file, line))
	{
		if (line.starts_with(" 1"))
		{
			break;
		}

		for (size_t i = 0; i < stacks.size(); ++i)
		{
			size_t ci = 1 + (i * 4);
			if (ci < line.size() && line.at(ci) != ' ')
			{
				stacks.at(i).push_front(line.at(ci));
			}
		}
	}

	std::vector<op> ops;
	std::regex regex("move (\\d+) from (\\d+) to (\\d+)");
	while (std::getline(file, line))
	{
		std::smatch sm;
		if (std::regex_search(line, sm, regex)) {
			ops.push_back(op{ std::atoi(sm[1].str().c_str()), std::atoi(sm[2].str().c_str()), std::atoi(sm[3].str().c_str()) });
		}
	}

	for(const auto op : ops)
	{
		for(int i = 0; i < op.move; ++i)
		{
			const char c = stacks.at(op.from - 1).back();
			stacks.at(op.from - 1).pop_back();
			stacks.at(op.to - 1).push_back(c);
		}
	}

	for(const auto& stack : stacks)
	{
		std::cout << stack.back();
	}
	std::cout << "\n";
}

void day5_2()
{
	std::array<std::deque<char>, 9> stacks;

	std::ifstream file("day5.input");
	std::string line;
	while (std::getline(file, line))
	{
		if (line.starts_with(" 1"))
		{
			break;
		}

		for (size_t i = 0; i < stacks.size(); ++i)
		{
			size_t ci = 1 + (i * 4);
			if (ci < line.size() && line.at(ci) != ' ')
			{
				stacks.at(i).push_front(line.at(ci));
			}
		}
	}

	std::vector<op> ops;
	std::regex regex("move (\\d+) from (\\d+) to (\\d+)");
	while (std::getline(file, line))
	{
		std::smatch sm;
		if (std::regex_search(line, sm, regex)) {
			ops.push_back(op{ std::atoi(sm[1].str().c_str()), std::atoi(sm[2].str().c_str()), std::atoi(sm[3].str().c_str()) });
		}
	}

	for(const auto op : ops)
	{
		std::vector<char> moved;
		for(int i = 0; i < op.move; ++i)
		{
			moved.push_back(stacks.at(op.from - 1).back());
			stacks.at(op.from - 1).pop_back();
		}
		for(auto ri = moved.rbegin(); ri != moved.rend(); ++ri)
		{
			stacks.at(op.to - 1).push_back(*ri);
		}
	}

	for(const auto& stack : stacks)
	{
		std::cout << stack.back();
	}
	std::cout << "\n";
}

void day5()
{
	//day5_1();
	day5_2();
}
