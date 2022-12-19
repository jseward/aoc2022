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

using namespace std;

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

struct cube
{
	int x;
	int y;
	int z;

	bool operator==(const cube& rhs) const
	{
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}
};

void day18_1()
{
	vector<cube> offsets{
		cube{+1, 0, 0},
		cube{-1, 0, 0},
		cube{0, +1, 0},
		cube{0, -1, 0},
		cube{0, 0, +1},
		cube{0, 0, -1}
	};

	vector<cube> cubes;

	std::ifstream file("day18.input");
	std::string line;
	while (std::getline(file, line))
	{
		const auto ps = split_string<char>(line, ",");
		cubes.push_back(cube{
			std::atoi(std::string(ps.at(0)).c_str()),
			std::atoi(std::string(ps.at(1)).c_str()),
			std::atoi(std::string(ps.at(2)).c_str()) });
	}

	size_t num_open_sides = 0;
	for (const auto& cube : cubes)
	{
		for (const auto& offset : offsets)
		{
			::cube test_cube = cube;
			test_cube.x += offset.x;
			test_cube.y += offset.y;
			test_cube.z += offset.z;

			if (std::find(cubes.begin(), cubes.end(), test_cube) == cubes.end())
			{
				num_open_sides++;
			}
		}
	}
}

void day18_2()
{
	vector<cube> offsets{
		cube{+1, 0, 0},
		cube{-1, 0, 0},
		cube{0, +1, 0},
		cube{0, -1, 0},
		cube{0, 0, +1},
		cube{0, 0, -1}
	};

	vector<cube> cubes;

	int min_x = 100000;
	int max_x = 0;
	int min_y = 100000;
	int max_y = 0;
	int min_z = 100000;
	int max_z = 0;

	std::ifstream file("day18.input");
	std::string line;
	while (std::getline(file, line))
	{
		const auto ps = split_string<char>(line, ",");
		cubes.push_back(cube{
			std::atoi(std::string(ps.at(0)).c_str()),
			std::atoi(std::string(ps.at(1)).c_str()),
			std::atoi(std::string(ps.at(2)).c_str()) });

		min_x = std::min(min_x, cubes.back().x);
		max_x = std::max(max_x, cubes.back().x);
		min_y = std::min(min_y, cubes.back().y);
		max_y = std::max(max_y, cubes.back().y);
		min_z = std::min(min_z, cubes.back().z);
		max_z = std::max(max_z, cubes.back().z);
	}

	vector<cube> open_cubes;

	for (int x = min_x; x <= max_x; ++x)
	{
		for (int y = min_y; y <= max_y; ++y)
		{
			for (int z = min_z; z <= max_z; ++z)
			{
				::cube c{ x, y, z };
				if (std::find(cubes.begin(), cubes.end(), c) == cubes.end() &&
					std::find(open_cubes.begin(), open_cubes.end(), c) == open_cubes.end())
				{
					vector<cube> t_cubes;
					deque<cube> q;
					q.push_back(c);
					while (!q.empty())
					{
						const cube t = q.front();
						q.pop_front();
						t_cubes.push_back(t);

						if (t.x < min_x || t.x > max_x || t.y < min_y || t.y > max_y || t.z < min_z || t.z > max_z)
						{
							open_cubes.insert(open_cubes.end(), t_cubes.begin(), t_cubes.end());
							t_cubes.clear();
							break;
						}
						else if (std::find(cubes.begin(), cubes.end(), t) == cubes.end())
						{
							for (const auto& offset : offsets)
							{
								cube nq = t;
								nq.x += offset.x;
								nq.y += offset.y;
								nq.z += offset.z;
								if (
									std::find(q.begin(), q.end(), nq) == q.end() &&
									std::find(cubes.begin(), cubes.end(), nq) == cubes.end() &&
									std::find(t_cubes.begin(), t_cubes.end(), nq) == t_cubes.end())
								{
									q.push_back(nq);
								}
							}
						}
					}

					cubes.insert(cubes.end(), t_cubes.begin(), t_cubes.end());
				}
			}
		}
	}

	size_t num_open_sides = 0;
	for (const auto& cube : cubes)
	{
		std::cout << "+";

		for (const auto& offset : offsets)
		{
			::cube test_cube = cube;
			test_cube.x += offset.x;
			test_cube.y += offset.y;
			test_cube.z += offset.z;

			if (std::find(cubes.begin(), cubes.end(), test_cube) == cubes.end())
			{
				num_open_sides++;
			}
		}
	}

	//2524
}

void day18()
{
	//day18_1();
	day18_2();
}
