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

bool is_marker(const std::string& buffer, size_t marker_size, size_t end_pos)
{
	std::vector<char> marker(marker_size);
	for(size_t i = end_pos - (marker_size - 1); i <= end_pos; ++i)
	{
		if(std::find(marker.begin(), marker.end(), buffer[i]) != marker.end())
		{
			return false;
		}
		marker.push_back(buffer[i]);
	}
	return true;
}

void day6_1()
{
	std::ifstream file("day6.input");
	std::stringstream input;
	input << file.rdbuf();
	const std::string buffer = input.str();

	std::deque<char> sequence;
	for(size_t i = 3; i < buffer.size(); ++i)
	{
		if(is_marker(buffer, 3, i))
		{
			std::cout << i + 1 << std::endl;
			break;
		}
	}
}

void day6_2()
{
	std::ifstream file("day6.input");
	std::stringstream input;
	input << file.rdbuf();
	const std::string buffer = input.str();

	std::deque<char> sequence;
	for(size_t i = 13; i < buffer.size(); ++i)
	{
		if(is_marker(buffer, 14, i))
		{
			std::cout << i + 1 << std::endl;
			break;
		}
	}
}

void day6()
{
	//day6_1();
	day6_2();
}
