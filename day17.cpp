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

vector<vector<coord>> make_shapes()
{
	vector<vector<coord>> shapes;
	shapes.push_back(vector<coord>{coord(0, 0), coord(1, 0), coord(2, 0), coord(3, 0)});
	shapes.push_back(vector<coord>{coord(1, 0), coord(0, 1), coord(1, 1), coord(2, 1), coord(1, 2)});
	shapes.push_back(vector<coord>{coord(0, 0), coord(1, 0), coord(2, 0), coord(2, 1), coord(2, 2)});
	shapes.push_back(vector<coord>{coord(0, 0), coord(0, 1), coord(0, 2), coord(0, 3)});
	shapes.push_back(vector<coord>{coord(0, 0), coord(0, 1), coord(1, 0), coord(1, 1)});
	return shapes;
}

bool is_blocked(int x, int y, const vector<coord>& shape, const std::vector<std::array<bool, 7>>& rows)
{
	for (int i = 0; i < 4; ++i)
	{
		int iy = y + i;
		if (iy < rows.size())
		{
			const auto& row = rows.at(iy);
			for (const auto& sc : shape)
			{
				if (sc.y() == i && row.at(x + sc.x()))
				{
					return true;
				}
			}
		}
	}
	return false;
}

void fill_rows(int x, int y, const vector<coord>& shape, std::vector<std::array<bool, 7>>& rows)
{
	for (int i = 0; i < 4; ++i)
	{
		bool is_within_shape = false;
		for(const auto& sc : shape)
		{
			if(sc.y() == i)
			{
				is_within_shape = true;
			}
		}

		if(is_within_shape)
		{
			int iy = y + i;
			if (iy == rows.size())
			{
				rows.push_back({});
				rows.back().fill(false);
			}

			auto& row = rows.at(iy);
			for (const auto& sc : shape)
			{
				if (sc.y() == i)
				{
					assert(!row.at(x + sc.x()));
					row.at(x + sc.x()) = true;
				}
			}
		}
	}
}

int get_width(const vector<coord>& shape)
{
	int max_x = 0;
	for (const auto& sc : shape)
	{
		max_x = std::max(sc.x(), max_x);
	}
	return max_x + 1;
}

void day17_1()
{
	const auto shapes = make_shapes();
	std::string jets;

	std::ifstream file("day17.input");
	std::string line;
	while (std::getline(file, line))
	{
		jets = line;
	}

	std::vector<std::array<bool, 7>> rows;
	int jet_i = 0;
	int height = 0;
	int num_rocks = 0;
	int shape_i = 0;
	while (num_rocks < 2022)
	{
		num_rocks++;

		const auto& shape = shapes.at(shape_i);
		shape_i = (shape_i + 1) % (int)shapes.size();

		int rock_x = 2;
		int rock_y = (int)rows.size() + 3;

		while (true)
		{
			// push
			const char jet = jets.at(jet_i);
			jet_i = (jet_i + 1) % (int)jets.size();

			if (jet == '<')
			{
				if (rock_x > 0 && !is_blocked(rock_x - 1, rock_y, shape, rows))
				{
					rock_x--;
				}
			}
			else if (jet == '>')
			{
				if (rock_x < (7 - get_width(shape)) && !is_blocked(rock_x + 1, rock_y, shape, rows))
				{
					rock_x++;
				}
			}
			else
			{
				assert(false);
			}

			// down
			if (rock_y > 0 && !is_blocked(rock_x, rock_y - 1, shape, rows))
			{
				rock_y--;
			}
			else
			{
				fill_rows(rock_x, rock_y, shape, rows);
				break;
			}
		}
	}

	std::cout << rows.size();
}

void day17_2()
{
	const auto shapes = make_shapes();
	std::string jets;

	std::ifstream file("day17.input");
	std::string line;
	while (std::getline(file, line))
	{
		jets = line;
	}

	std::vector<std::array<bool, 7>> rows;
	int cycle_rows = 0;
	int cycle_num_rocks = 0;
	int jet_i = 0;
	int height = 0;
	int num_rocks = 0;
	int shape_i = 0;
	while (num_rocks < 2)
	{
		num_rocks++;

		const auto& shape = shapes.at(shape_i);
		shape_i = (shape_i + 1) % (int)shapes.size();

		int rock_x = 2;
		int rock_y = (int)rows.size() + 3;

		while (true)
		{
			// push
			const char jet = jets.at(jet_i);
			jet_i = (jet_i + 1) % (int)jets.size();

			if (jet == '<')
			{
				if (rock_x > 0 && !is_blocked(rock_x - 1, rock_y, shape, rows))
				{
					rock_x--;
				}
			}
			else if (jet == '>')
			{
				if (rock_x < (7 - get_width(shape)) && !is_blocked(rock_x + 1, rock_y, shape, rows))
				{
					rock_x++;
				}
			}
			else
			{
				assert(false);
			}

			// down
			if (rock_y > 0 && !is_blocked(rock_x, rock_y - 1, shape, rows))
			{
				rock_y--;
			}
			else
			{
				fill_rows(rock_x, rock_y, shape, rows);

				std::array<bool, 7> full;
				full.fill(true);
				if(rows.back() == full)
				{
					std::cout << num_rocks - cycle_num_rocks << " - " << rows.size() - cycle_rows << " shape=" << shape_i << " , jet=" << jet_i << "\n";
					cycle_num_rocks = num_rocks;
					cycle_rows = (int)rows.size();
				}

				break;
			}
		}
	}

	// 1741 - 2724
	// 1715 - 2677
	// 1715 - 2677
	// 1715 - 2677   (shape=1 , jet=214)
	// ...
	


	const uint64_t total_num_rocks = 1000000000000;
	const uint64_t total_num_rocks_0 = (total_num_rocks - 1741);

	const uint64_t rocks_per_loop = 1715;
	const uint64_t rows_per_loop = 2677;

	const uint64_t extra_rocks = total_num_rocks_0 % rocks_per_loop; // 1704
	const uint64_t num_loops = ((total_num_rocks_0 + extra_rocks) / rocks_per_loop) - 1; // 583090377

	const uint64_t total_rows_from_loops = num_loops * rows_per_loop; // 1560932939229

	// extra_rows = 2662

	// 2724 + 2662 + 1560932939229 = 1560932944615
}

void day17()
{
	//day17_1();
	day17_2();
}
