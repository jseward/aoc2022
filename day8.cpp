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

struct tree
{
	int height = 0;
	bool is_visible = false;
};

using tree_grid = std::vector<std::vector<tree>>;

void day8_1()
{
	tree_grid trees;
	std::ifstream file("day8.input");
	std::string line;
	while (std::getline(file, line))
	{
		std::vector<tree> row;
		for (const char c : line)
		{
			std::string cs;
			cs += c;
			row.push_back(tree{ std::atoi(cs.c_str()), false });
		}
		trees.push_back(row);
	}

	for (size_t row_index = 0; row_index < trees.size(); ++row_index)
	{
		auto& row = trees.at(row_index);
		{
			int highest = -1;
			for (auto iter = row.begin(); iter != row.end(); ++iter)
			{
				if (iter->height > highest)
				{
					iter->is_visible = true;
					highest = std::max(highest, iter->height);
				}
			}
		}
		{
			int highest = -1;
			for (auto iter = row.rbegin(); iter != row.rend(); ++iter)
			{
				if (iter->height > highest)
				{
					iter->is_visible = true;
					highest = std::max(highest, iter->height);
				}
			}
		}
	}

	for (size_t column_index = 0; column_index < trees.at(0).size(); ++column_index)
	{
		{
			int highest = -1;
			for (size_t row_index = 0; row_index < trees.size(); ++row_index)
			{
				auto& tree = trees.at(row_index).at(column_index);
				if (tree.height > highest)
				{
					tree.is_visible = true;
					highest = std::max(highest, tree.height);
				}
			}
		}
		{
			int highest = -1;
			for (int row_index = static_cast<int>(trees.size() - 1); row_index >= 0; --row_index)
			{
				auto& tree = trees.at(row_index).at(column_index);
				if (tree.height > highest)
				{
					tree.is_visible = true;
					highest = std::max(highest, tree.height);
				}
			}
		}
	}

	size_t total_count = 0;
	for (const auto& row : trees)
	{
		for (const auto& tree : row)
		{
			if (tree.is_visible)
			{
				total_count += 1;
			}
		}
	}
}

struct coord
{
	size_t x = 0;
	size_t y = 0;
};

int calculate_score(const tree_grid& trees, int x, int y)
{
	std::array<int, 4> blocked;
	blocked.fill(0);

	for (int nx = x + 1; nx < trees.at(0).size(); ++nx)
	{
		blocked.at(0) += 1;
		if (trees.at(y).at(nx).height >= trees.at(y).at(x).height)
		{
			break;
		}
	}

	for (int nx = x - 1; nx >= 0; --nx)
	{
		blocked.at(1) += 1;
		if (trees.at(y).at(nx).height >= trees.at(y).at(x).height)
		{
			break;
		}
	}

	for (int ny = y + 1; ny < trees.size(); ++ny)
	{
		blocked.at(2) += 1;
		if (trees.at(ny).at(x).height >= trees.at(y).at(x).height)
		{
			break;
		}
	}

	for (int ny = y - 1; ny >= 0; --ny)
	{
		blocked.at(3) += 1;
		if (trees.at(ny).at(x).height >= trees.at(y).at(x).height)
		{
			break;
		}
	}

	return blocked.at(0) * blocked.at(1) * blocked.at(2) * blocked.at(3);
}

void day8_2()
{
	tree_grid trees;
	std::ifstream file("day8.input");
	std::string line;
	while (std::getline(file, line))
	{
		std::vector<tree> row;
		for (const char c : line)
		{
			std::string cs;
			cs += c;
			row.push_back(tree{ std::atoi(cs.c_str()), false });
		}
		trees.push_back(row);
	}

	int highest_score = 0;
	for (size_t y = 0; y < trees.size(); ++y)
	{
		for (size_t x = 0; x < trees.size(); ++x)
		{
			const int this_score = calculate_score(trees, static_cast<int>(x), static_cast<int>(y));
			if (this_score > highest_score)
			{
				highest_score = this_score;
			}
		}
	}

	//338776
}

void day8()
{
	day8_1();
	day8_2();
}
