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

using coord = std::pair<int, int>;

coord next_h(coord in, char dir, int dist)
{
	switch (dir)
	{
	case 'D':
		return coord(in.first, in.second - dist);
	case 'U':
		return coord(in.first, in.second + dist);
	case 'L':
		return coord(in.first - 1, in.second);
	case 'R':
		return coord(in.first + 1, in.second);
	}

	throw std::exception();
}

coord next_t(coord h, coord t)
{
	if (h.first == t.first)
	{
		int dist = std::abs(h.second - t.second);
		if (dist > 1)
		{
			if (h.second > t.second)
			{
				return coord(t.first, t.second + 1);
			}
			return coord(t.first, t.second - 1);
		}
	}
	else if (h.second == t.second)
	{
		int dist = std::abs(h.first - t.first);
		if (dist > 1)
		{
			if (h.first > t.first)
			{
				return coord(t.first + 1, t.second);
			}
			return coord(t.first - 1, t.second);
		}
	}
	else
	{
		int delta_x = t.first - h.first;
		int delta_y = t.second - h.second;
		if (std::abs(delta_x) > 1 || std::abs(delta_y) > 1)
		{
			if (std::abs(delta_x) > std::abs(delta_y))
			{
				return coord(t.first + (delta_x < 0 ? 1 : -1), h.second);
			}
			else if (std::abs(delta_x) < std::abs(delta_y))
			{
				return coord(h.first, t.second + (delta_y < 0 ? 1 : -1));
			}
			return coord(t.first + (delta_x < 0 ? 1 : -1), t.second + (delta_y < 0 ? 1 : -1));
		}
	}

	return t;
}

void day9_1()
{
	coord h(0, 0);
	coord t(0, 0);

	std::map<coord, bool> visited;
	visited[t] = true;

	std::ifstream file("day9.input");
	std::string line;
	while (std::getline(file, line))
	{
		const auto parts = split_string<char>(line, " ");
		const char dir = parts.at(0).at(0);
		const int distance = std::atoi(std::string(parts.at(1)).c_str());

		for (int i = 0; i < distance; ++i)
		{
			h = next_h(h, dir, 1);
			t = next_t(h, t);
			visited[t] = true;
		}
	}

	const size_t v = visited.size(); // 3143
}

void day9_2()
{
	std::array<coord, 10> coords;

	std::map<coord, bool> visited;
	visited[coords.back()] = true;

	std::ifstream file("day9.input");
	std::string line;
	size_t count = 0;
	while (std::getline(file, line))
	{
		const auto parts = split_string<char>(line, " ");
		const char dir = parts.at(0).at(0);
		const int distance = std::atoi(std::string(parts.at(1)).c_str());

		for (int i = 0; i < distance; ++i)
		{
			coords.at(0) = next_h(coords.at(0), dir, 1);
			for(int j = 1; j < 10; ++j)
			{
				coords.at(j) = next_t(coords.at(j - 1), coords.at(j));
			}
			visited[coords.back()] = true;
			count += 1;
		}
	}

	const size_t v = visited.size();
}

void day9()
{
	//day9_1();
	day9_2();
}
