#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <optional>
#include <array>
#include <algorithm>
#include <map>

int get_priority(char c)
{
	if(c >= 'a' && c <= 'z')
	{
		return static_cast<int>(c - 'a');
	}

	if(c >= 'A' && c <= 'Z')
	{
		return 26 + static_cast<int>(c - 'A');
	}

	throw std::exception();
}

void day3_1()
{
	std::vector<int> duplicates;

	std::ifstream file("day3.input");
	std::string line;
	while (std::getline(file, line))
	{
		std::array<bool, 52> c0{false};
		size_t half = line.length() / 2;
		for(size_t i = 0; i < half; ++i)
		{
			c0.at(get_priority(line.at(i))) = true;
		}
		for(size_t i = half; i < line.length(); ++i)
		{
			const char c = line.at(i);
			const int p = get_priority(c);
			if(c0.at(p))
			{
				duplicates.push_back(p);
				break;
			}
		}
	}

	int acc = 0;
	for(const int dup : duplicates)
	{
		acc += (dup + 1);
	}
	std::cout << acc << "\n";
}

void day3_2()
{
	std::vector<int> duplicates;
	
	std::array<std::array<bool, 52>, 3> bags;
	for(auto& bag : bags)
	{
		bag.fill(false);
	}

	size_t group_index = 0;

	std::ifstream file("day3.input");
	std::string line;
	while (std::getline(file, line))
	{
		for(const char c : line)
		{
			const int p = get_priority(c);
			if(group_index == 0)
			{
				bags.at(group_index).at(p) = true;
			}
			else if(bags.at(group_index - 1).at(p))
			{
				bags.at(group_index).at(p) = true;
			}
		}
		group_index += 1;
		if(group_index == 3)
		{
			for(size_t i = 0; i < bags.back().size(); ++i)
			{
				if(bags.back().at(i))
				{
					duplicates.push_back(static_cast<int>(i));
				}
			}

			for(auto& bag : bags)
			{
				bag.fill(false);
			}

			group_index = 0;
		}
	}

	int acc = 0;
	for(const int dup : duplicates)
	{
		acc += (dup + 1);
	}
	std::cout << acc << "\n";
}

void day3()
{
	day3_1();
	day3_2();
}
