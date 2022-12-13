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

struct coord : public std::pair<int, int>
{
	coord(int x, int y) : std::pair<int, int>(x, y) {}
	int& x() { return first; }
	int& y() { return second; }
};

void day12_1()
{
	std::map<coord, int> height_map;
	coord curr(0, 0);
	coord dest(0, 0);
	std::vector<coord> starts;

	std::ifstream file("day12.input");
	std::string line;
	int y = 0;
	while (std::getline(file, line))
	{
		int x = 0;
		for (const char c : line)
		{
			char actual_c = c;
			if (c == 'S')
			{
				curr = coord(x, y);
				actual_c = 'a';
			}
			else if (c == 'E')
			{
				dest = coord(x, y);
				actual_c = 'z';
			}

			const int height = static_cast<int>(actual_c) - static_cast<int>('a');
			assert(height >= 0);
			height_map[coord(x, y)] = height;
			if(height == 0)
			{
				starts.push_back(coord(x, y));
			}
			x += 1;
		}
		y += 1;
	}

	std::vector<int> steps;

	for (const auto& start : starts)
	{
		std::deque<std::pair<coord, int>> queue;
		std::vector<coord> visited;

		const auto add_to_queue = [&](coord from, coord to, int steps)
		{
			if (std::find(visited.begin(), visited.end(), to) != visited.end())
			{
				return;
			}

			if (height_map.find(to) != height_map.end())
			{
				if ((height_map.at(from) + 1) >= height_map.at(to))
				{
					queue.push_back(std::pair<coord, int>(to, steps));
					visited.push_back(to);
				}
			}
		};

		const auto add_all_to_queue = [&](coord from, int steps)
		{
			add_to_queue(from, coord(from.x(), from.y() + 1), steps);
			add_to_queue(from, coord(from.x(), from.y() - 1), steps);
			add_to_queue(from, coord(from.x() + 1, from.y()), steps);
			add_to_queue(from, coord(from.x() - 1, from.y()), steps);
		};

		add_all_to_queue(start, 1);
		while (!queue.empty())
		{
			const auto con = queue.front();
			queue.pop_front();
			if (con.first == dest)
			{
				steps.push_back(con.second);
				break;
			}
			add_all_to_queue(con.first, con.second + 1);
		}
	}

	std::sort(steps.begin(), steps.end());
}

void day12()
{
	day12_1();
	//day12_2();
}
