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

template<class CharT>
std::vector<std::string> split_string(std::basic_string_view<CharT> s, std::basic_string_view<CharT> delimiter)
{
	std::vector<std::string> split_strings;

	size_t begin = 0U;
	size_t end = s.find(delimiter);
	while (end != std::string::npos)
	{
		split_strings.push_back(std::string(std::basic_string_view<CharT>(s.data() + begin, end - begin)));
		begin = end + delimiter.length();
		end = s.find(delimiter, begin);
	}

	if (s.length() - begin > 0)
	{
		split_strings.push_back(std::string(std::basic_string_view<CharT>(s.data() + begin, s.length() - begin)));
	}

	return split_strings;
}

struct instruction
{
	std::string op;
	std::optional<int> value;
};

std::vector<instruction> read_instructions()
{
	std::vector<instruction> instructions;

	std::ifstream file("day10.input");
	std::string line;
	while (std::getline(file, line))
	{
		const auto split = split_string<char>(line, " ");
		instruction i;
		i.op = split.at(0);
		if (i.op == "addx")
		{
			i.value = std::atoi(split.at(1).c_str());
		}
		instructions.push_back(i);
	}
	return instructions;
}

class program
{
public:
	std::deque<int> signal_cycles = {
		20, 60, 100, 140, 180, 220
	};
	std::vector<int> signals;
	int x = 1;
	int cycle = 1;

	void inc_cycle()
	{
		cycle += 1;
		if(!signal_cycles.empty() && cycle == signal_cycles.front())
		{
			signals.push_back(x * cycle);
			signal_cycles.pop_front();
		}
	}

	void run(const std::vector<instruction>& instructions)
	{
		size_t i = 0;
		while (!signal_cycles.empty())
		{
			const auto& ins = instructions.at(i);
			if (ins.op == "noop")
			{
				inc_cycle();
			}
			else if (ins.op == "addx")
			{
				inc_cycle();
				x += ins.value.value();
				inc_cycle();
			}

			++i;
		}
	}
};

void day10_1()
{
	const auto instructions = read_instructions();
	
	program p;
	p.run(instructions);

	int sum = 0;
	for(const int v : p.signals)
	{
		sum += v;
	}
}

class program2
{
public:
	int x = 1;
	int cycle = 1;
	int cursor = 1;

	void inc_cycle()
	{
		cycle += 1;
		
		if(x >= (cursor - 1) && x <= (cursor + 1))
		{
			std::cout << "#";
		}
		else
		{
			std::cout << ".";
		}

		cursor += 1;
		if (cursor > 40)
		{
			cursor = 1;
			std::cout << "\n";
		}
	}

	void run(const std::vector<instruction>& instructions)
	{
		size_t i = 0;
		while (cycle < 240)
		{
			const auto& ins = instructions.at(i);
			if (ins.op == "noop")
			{
				inc_cycle();
			}
			else if (ins.op == "addx")
			{
				inc_cycle();
				x += ins.value.value();
				inc_cycle();
			}

			++i;
		}
	}
};

void day10_2()
{
	const auto instructions = read_instructions();
	
	program2 p;
	p.run(instructions);
}

void day10()
{
	//day10_1();
	day10_2();
}
