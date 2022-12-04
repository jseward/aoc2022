#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <optional>
#include <array>
#include <algorithm>
#include <map>

template<class CharT>
std::vector<std::basic_string_view<CharT>> split_string(std::basic_string_view<CharT> s, std::basic_string_view<CharT> delimiter)
{
	std::vector<std::basic_string_view<CharT>> split_strings;

	size_t begin = 0U;
	size_t end = s.find(delimiter);
	while (end != std::string::npos)
	{
		split_strings.push_back(std::basic_string_view<CharT>(s.data() + begin, end - begin));
		begin = end + delimiter.length();
		end = s.find(delimiter, begin);
	}

	if (s.length() - begin > 0)
	{
		split_strings.push_back(std::basic_string_view<CharT>(s.data() + begin, s.length() - begin));
	}

	return split_strings;
}

void day4_1()
{
	size_t count = 0;
	std::ifstream file("day4.input");
	std::string line;
	while (std::getline(file, line))
	{
		const auto split = split_string<char>(line, ",");
		const auto r0s = split_string<char>(split.at(0), "-");
		const auto r1s = split_string<char>(split.at(1), "-");
		const auto r0 = std::pair<int, int>(std::atoi(std::string(r0s.at(0)).c_str()), std::atoi(std::string(r0s.at(1)).c_str()));
		const auto r1 = std::pair<int, int>(std::atoi(std::string(r1s.at(0)).c_str()), std::atoi(std::string(r1s.at(1)).c_str()));
		if(r0.first <= r1.first && r0.second >= r1.second)
		{
			count += 1;
		}
		else if(r1.first <= r0.first && r1.second >= r0.second)
		{
			count += 1;
		}
	}

	std::cout << count << "\n";
}

void day4_2()
{
	size_t count = 0;
	std::ifstream file("day4.input");
	std::string line;
	while (std::getline(file, line))
	{
		const auto split = split_string<char>(line, ",");
		const auto r0s = split_string<char>(split.at(0), "-");
		const auto r1s = split_string<char>(split.at(1), "-");
		const auto r0 = std::pair<int, int>(std::atoi(std::string(r0s.at(0)).c_str()), std::atoi(std::string(r0s.at(1)).c_str()));
		const auto r1 = std::pair<int, int>(std::atoi(std::string(r1s.at(0)).c_str()), std::atoi(std::string(r1s.at(1)).c_str()));
		if(r0.second < r1.first || r0.first > r1.second || r1.second < r0.first || r1.first > r0.second)
		{
			// no overlap
		}
		else
		{
			count += 1;
		}
	}

	std::cout << count << "\n";
}

void day4()
{
	//day4_1();
	day4_2();
}
