#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <optional>
#include <array>
#include <algorithm>

template<typename ContainerT>
typename ContainerT::iterator insert_sorted(ContainerT& container, const typename ContainerT::value_type& item)
{
	return container.insert(std::upper_bound(container.begin(), container.end(), item), item);
}

void day1()
{
	std::vector<std::optional<int>> input;
	std::ifstream file("day1.input");
	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty())
		{
			input.push_back(std::nullopt);
		}
		else
		{
			input.push_back(std::atoi(line.c_str()));
		}
	}

	// part1
	{
		int max_calories = 0;
		int current_calories = 0;
		for (const auto& i : input)
		{
			if (!i.has_value())
			{
				max_calories = std::max(max_calories, current_calories);
				current_calories = 0;
			}
			else
			{
				current_calories += i.value();
			}
		}
		std::cout << max_calories << "\n";
	}

	// part2
	{
		std::vector<int> max_calories;
		int current_calories = 0;
		for (const auto& i : input)
		{
			if (!i.has_value())
			{
				insert_sorted(max_calories, current_calories);
				current_calories = 0;
			}
			else
			{
				current_calories += i.value();
			}
		}

		std::cout << max_calories.at(max_calories.size() - 1) + max_calories.at(max_calories.size() - 2) + max_calories.at(max_calories.size() - 3) << "\n";
	}
}
