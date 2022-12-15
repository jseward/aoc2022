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
#include <set>
#include "bloom_filter.h"

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

coord parse_coord(std::string_view s)
{
	const auto cs = split_string<char>(s, ",");
	return coord(std::atoi(std::string(cs.at(0)).c_str()), std::atoi(std::string(cs.at(1)).c_str()));
}

int get_man_distance(coord a, coord b)
{
	return std::abs(a.x() - b.x()) + std::abs(a.y() - b.y());
}

void day15_1()
{
	const int answer_y = 2000000;
	//const int answer_y = 10;

	std::set<int> xs;
	std::set<int> beacons;
	std::map<coord, bool> m;

	std::ifstream file("day15.input");
	//std::ifstream file("day15_example.input");
	std::string line;
	while (std::getline(file, line))
	{
		const auto sbs = split_string<char>(line, ":");
		coord sensor = parse_coord(sbs.at(0));
		coord beacon = parse_coord(sbs.at(1));
		const int distance = get_man_distance(sensor, beacon);
		const int distance_to_answer_y = std::abs(sensor.y() - answer_y);
		if (distance_to_answer_y <= distance)
		{
			int x_distance = distance - distance_to_answer_y;
			for (int x = -x_distance; x <= x_distance; ++x)
			{
				//m[sensor.add(coord(x, y))] = true;
				xs.insert(sensor.x() + x);
			}
		}

		if (beacon.y() == answer_y)
		{
			beacons.insert(beacon.x());
		}
	}

	std::cout << (xs.size() - beacons.size());

	//4725496
}

void day15_2_bloom()
{
	const unsigned long max_v = 4000000;
	//const int max_v = 20;

	bloom_parameters parameters;
	parameters.projected_element_count = max_v * max_v;
	parameters.false_positive_probability = 0.000000000001;
	parameters.compute_optimal_parameters();
	bloom_filter filter(parameters);

	std::ifstream file("day15.input");
	//std::ifstream file("day15_example.input");
	std::string line;
	while (std::getline(file, line))
	{
		std::cout << line << "\n";
		const auto sbs = split_string<char>(line, ":");
		coord sensor = parse_coord(sbs.at(0));
		coord beacon = parse_coord(sbs.at(1));
		const int distance = get_man_distance(sensor, beacon);

		int min_x_offset = std::max(-sensor.x(), -distance);
		int max_x_offset = std::min(static_cast<int>(max_v - sensor.x()), distance);
		for (int x_offset = min_x_offset; x_offset <= max_x_offset; ++x_offset)
		{
			int x = sensor.x() + x_offset;
			if (x >= 0 && x <= max_v)
			{
				int y_distance = distance - std::abs(x_offset);
				int min_y_offset = std::max(-sensor.y(), -y_distance);
				int max_y_offset = std::min(static_cast<int>(max_v - sensor.y()), y_distance);
				for (int y_offset = min_y_offset; y_offset <= max_y_offset; ++y_offset)
				{
					int y = sensor.y() + y_offset;
					if (y >= 0 && y <= max_v)
					{
						filter.insert(coord(x, y));
					}
				}
			}
		}
	}

	std::cout << "buildcoords" << "\n";
	std::vector<coord> coords;
	for (int x = 0; x <= max_v; ++x)
	{
		for (int y = 0; y <= max_v; ++y)
		{
			coords.push_back(coord(x, y));
		}
	}

	std::cout << "containsall" << "\n";
	const auto iter = filter.contains_all(coords.begin(), coords.end());
	if (iter != coords.end())
	{
		std::cout << iter->x() << "," << iter->y();
		const unsigned long seq = (iter->x() * max_v) + iter->y();
		std::cout << seq;
	}
}

struct row
{
	bool full = false;
	std::vector<std::pair<int, int>> segments;
};

void day15_2()
{
	const int max_v = 4000000;
	//const int max_v = 20;

	std::map<int, std::unique_ptr<row>> rows;
	for (int i = 0; i <= max_v; ++i)
	{
		rows[i] = std::make_unique<row>();
	}

	//std::ifstream file("day15_example.input");
	std::ifstream file("day15.input");

	std::string line;
	while (std::getline(file, line))
	{
		std::cout << line << "\n";
		const auto sbs = split_string<char>(line, ":");
		coord sensor = parse_coord(sbs.at(0));
		coord beacon = parse_coord(sbs.at(1));
		const int distance = get_man_distance(sensor, beacon);

		int min_y_offset = std::max(-sensor.y(), -distance);
		int max_y_offset = std::min(static_cast<int>(max_v - sensor.y()), distance);
		for (int y_offset = min_y_offset; y_offset <= max_y_offset; ++y_offset)
		{
			int y = sensor.y() + y_offset;
			auto& row = rows[y];
			if (!row->full)
			{
				int x_distance = distance - std::abs(y_offset);
				int min_x = std::max(0, sensor.x() - x_distance);
				int max_x = std::min(max_v, sensor.x() + x_distance);
				if (min_x <= 0 && max_x >= max_v)
				{
					row->full = true;
				}
				else
				{
					bool added_to_segments = false;
					for (auto& segment : row->segments)
					{
						if(min_x >= segment.first && max_x <= segment.second)
						{
							added_to_segments = true;
							break;
						}
						else if(min_x <= segment.first && max_x >= segment.second)
						{
							segment.first = min_x;
							segment.second = max_x;
							added_to_segments = true;
							break;
						}
						else if (min_x <= segment.first && min_x >= segment.first)
						{
							segment.first = std::min(min_x, segment.first);
							added_to_segments = true;
							break;
						}
						else if (max_x >= segment.second && min_x <= segment.second)
						{
							segment.second = std::max(max_x, segment.second);
							added_to_segments = true;
							break;
						}
					}

					if (!added_to_segments)
					{
						row->segments.push_back(std::make_pair(min_x, max_x));
					}
				}
			}
		}
	}

	std::cout << "ANALYZE!" << "\n";

	for (int i = 0; i <= max_v; ++i)
	{
		const auto& row = rows[i];
		bool full = row->full;
		if (!full)
		{
			std::sort(row->segments.begin(), row->segments.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
			int x = 0;
			for (const auto& segment : row->segments)
			{
				if (segment.first <= 0 && segment.second >= max_v)
				{
					full = true;
					break;
				}

				if((x + 1) >= segment.first)
				{
					x = std::max(x, segment.second);
				}
				else
				{
					full = false;
					break;
				}
			}

			if(x >= max_v)
			{
				full = true;
			}
		}

		if (!full)
		{
			std::cout << "ROW(" << i << ") ";
			for (const auto& segment : row->segments)
			{
				std::cout << "[" << segment.first << ":" << segment.second << "]";
			}
			std::cout << "\n";
		}
	}

	//ROW(3042458) [0:2223394][2126374:3012820][3012822:4000000]
	//(3012821*4000000) + 3042458
	//12051287042458
}

void day15()
{
	//day15_1();
	day15_2();
}
