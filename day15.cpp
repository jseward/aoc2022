#include <iostream>
#include <fstream>
#include <assert.h>
#include <string>
#include <vector>
#include <optional>
#include <array>
#include <algorithm>
#include <map>
#include <deque>
#include <sstream>

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

struct coord : public std::pair<int, int>
{
	coord(int x, int y) : std::pair<int, int>(x, y) {}
	int& x() { return first; }
	int& y() { return second; }
	const int& x() const { return first; }
	const int& y() const { return second; }
	coord add(const coord& rhs) const
	{
		return coord(x() + rhs.x(), y() + rhs.y());
	};
};

void day15_1()
{
	std::ifstream file("day15.input");
	std::string line;
	while (std::getline(file, line))
	{
	}
}

void day15_2()
{
}

void day15()
{
	day15_1();
	day15_2();
}
