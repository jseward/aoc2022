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

enum class coord_state
{
	empty = 0,
	rock,
	sand
};

bool fall_sand(std::map<coord, coord_state>& m, int max_y, coord begin)
{
	coord curr = begin;
	while(true)
	{
		if(curr.y() + 1 >= max_y)
		{
			m[curr] = coord_state::sand;
			return false;
		}

		const auto s = curr.add(coord(0, 1));
		const auto l = curr.add(coord(-1, 1));
		const auto r = curr.add(coord(1, 1));

		if(m[s] == coord_state::empty)
		{
			curr = s;
		}
		else if(m[l] == coord_state::empty)
		{
			curr = l;
		}
		else if(m[r] == coord_state::empty)
		{
			curr = r;
		}
		else
		{
			m[curr] = coord_state::sand;
			return (curr == begin);
		}

		//if(curr.y() > max_y)
		//{
		//	return true;
		//}
	}

	return false;
}

void day14_1()
{
	std::map<coord, coord_state> m;
	int max_y = 0;

	std::ifstream file("day14.input");
	std::string line;
	while (std::getline(file, line))
	{
		const auto coord_strings = split_string<char>(line, " -> ");
		std::vector<coord> coords;
		for(const auto coord_string : coord_strings)
		{
			const auto split_cs = split_string<char>(coord_string, ",");
			int x = std::atoi(std::string(split_cs.at(0)).c_str());
			int y = std::atoi(std::string(split_cs.at(1)).c_str());			
			max_y = std::max(max_y, y);
			coords.push_back(coord(x, y));
		}

		for(size_t i = 0; i < coords.size() - 1; ++i)
		{
			const auto a = coords.at(i);
			const auto b = coords.at(i + 1);
			if(a.x() == b.x())
			{
				int delta = b.y() - a.y();
				for(int i = 0; i <= std::abs(delta); ++i)
				{
					const int sign = (delta > 0) ? +1 : -1;
					const coord c = coord(a.x(), a.y() + (i * sign));
					m[c] = coord_state::rock;
				}
			}
			else if(a.y() == b.y())
			{
				int delta = b.x() - a.x();
				for(int i = 0; i <= std::abs(delta); ++i)
				{
					const int sign = (delta > 0) ? +1 : -1;
					const coord c = coord(a.x() + (i * sign), a.y());
					m[c] = coord_state::rock;
				}
			}
			else
			{
				assert(false);
			}
		}
	}

	size_t units = 0;
	while(true)
	{
		units += 1;
		if(fall_sand(m, max_y + 2, coord(500, 0)))
		{
			break;
		}
	}

	// 625
}

void day14_2()
{
	// hacked up part1 instead
}

void day14()
{
	day14_1();
	day14_2();
}
