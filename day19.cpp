#include <iostream>
#include <regex>
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

enum asset
{
	ore = 0,
	clay = 1,
	obsidian = 2,
	geode = 3
};

struct robot
{
	std::array<int, 4> cost;

	robot()
	{
		cost.fill(0);
	}

	void set_cost(asset a, int v)
	{
		cost.at(a) = v;
	}
};

struct blueprint
{
	std::array<robot, 4> robots;
	std::array<int, 4> max_costs;

	blueprint()
	{
		max_costs.fill(0);
	}

	void update_max_costs()
	{
		for (int i = 0; i < 4; ++i)
		{
			for(int j = 0; j < 4; ++j)
			{
				max_costs.at(i) = std::max(max_costs.at(i), robots.at(j).cost.at(i));
			}
		}
	}
};

struct state
{
	int minute = 0;
	array<int, 4> num_robots;
	array<int, 4> num_assets;

	state()
	{
		num_robots.fill(0);
		num_assets.fill(0);
	}

	void purchase(int robot, const std::array<int, 4>& cost)
	{
		num_robots.at(robot) += 1;
		for (int i = 0; i < 4; ++i)
		{
			num_assets.at(i) -= cost.at(i);
		}
	}

	void collect()
	{
		for (int i = 0; i < 4; ++i)
		{
			num_assets.at(i) += num_robots.at(i);
		}
	}
};

bool can_afford(const std::array<int, 4>& curr_assets, const std::array<int, 4>& cost)
{
	for (int i = 0; i < 4; ++i)
	{
		if (curr_assets.at(i) < cost.at(i))
		{
			return false;
		}
	}
	return true;
}

int get_score(const blueprint& blueprint, int max_minutes)
{
	state initial_s;
	initial_s.minute = 1;
	initial_s.num_robots.at(asset::ore) = 1;

	int most_geodes = 0;

	vector<state> states;
	states.push_back(initial_s);
	while (!states.empty())
	{
		auto s = states.back();
		states.pop_back();

		if (s.minute > max_minutes)
		{
			if (s.num_assets.at(asset::geode) > most_geodes)
			{
				most_geodes = s.num_assets.at(asset::geode);
			}
		}
		else
		{
			bool can_ever_exceed_most =
				(s.num_assets.at(asset::geode) + (max_minutes - s.minute)) > most_geodes;
			if (can_ever_exceed_most)
			{
				std::vector<int> needed_robots;
				for (int i = 0; i < 4; ++i)
				{
					if (s.num_robots.at(i) < blueprint.max_costs.at(i) || (i == asset::geode))
					{
						needed_robots.push_back(i);
					}
				}

				for (int robot : needed_robots)
				{
					const auto& cost = blueprint.robots.at(robot).cost;
					if (can_afford(s.num_assets, cost))
					{
						state ns = s;
						ns.collect();
						ns.purchase(robot, blueprint.robots.at(robot).cost);
						ns.minute += 1;
						states.push_back(ns);
					}
					else
					{
						int minutes_needed = 0;
						for (int i = 0; i < 4; ++i)
						{
							if (cost.at(i) > 0)
							{
								const int needed = cost.at(i) - s.num_assets.at(i);
								if(needed > 0)
								{
									const int minutes = (int)std::ceilf(static_cast<float>(needed) / static_cast<float>(cost.at(i)));
									minutes_needed = std::max(minutes_needed, minutes);
								}
							}
						}
						state ns = s;
						ns.collect();
						ns.minute += minutes_needed;
						states.push_back(ns);
					}
				}
			}
		}
	}

	return most_geodes;
}

void day19_1()
{
	vector<blueprint> blueprints;

	std::ifstream file("day19.input");
	std::string line;
	std::regex regex("Blueprint (\\d+): Each ore robot costs (\\d+) ore. Each clay robot costs (\\d+) ore. Each obsidian robot costs (\\d+) ore and (\\d+) clay. Each geode robot costs (\\d+) ore and (\\d+) obsidian.");
	while (std::getline(file, line))
	{
		std::smatch sm;
		if (std::regex_search(line, sm, regex)) {
			blueprint b;
			b.robots.at(asset::ore).set_cost(asset::ore, std::atoi(sm[2].str().c_str()));
			b.robots.at(asset::clay).set_cost(asset::ore, std::atoi(sm[3].str().c_str()));
			b.robots.at(asset::obsidian).set_cost(asset::ore, std::atoi(sm[4].str().c_str()));
			b.robots.at(asset::obsidian).set_cost(asset::clay, std::atoi(sm[5].str().c_str()));
			b.robots.at(asset::geode).set_cost(asset::ore, std::atoi(sm[6].str().c_str()));
			b.robots.at(asset::geode).set_cost(asset::obsidian, std::atoi(sm[7].str().c_str()));
			b.update_max_costs();
			blueprints.push_back(b);
		}
	}

	int total_quality = 0;
	for (int i = 0; i < blueprints.size(); ++i)
	{
		std::cout << "+";
		total_quality += ((i + 1) * get_score(blueprints.at(i), 24));
	}

	std::cout << "\n" << total_quality; //1599
}

void day19_2()
{
	vector<blueprint> blueprints;

	std::ifstream file("day19.input");
	std::string line;
	std::regex regex("Blueprint (\\d+): Each ore robot costs (\\d+) ore. Each clay robot costs (\\d+) ore. Each obsidian robot costs (\\d+) ore and (\\d+) clay. Each geode robot costs (\\d+) ore and (\\d+) obsidian.");
	while (std::getline(file, line))
	{
		std::smatch sm;
		if (std::regex_search(line, sm, regex)) {
			blueprint b;
			b.robots.at(asset::ore).set_cost(asset::ore, std::atoi(sm[2].str().c_str()));
			b.robots.at(asset::clay).set_cost(asset::ore, std::atoi(sm[3].str().c_str()));
			b.robots.at(asset::obsidian).set_cost(asset::ore, std::atoi(sm[4].str().c_str()));
			b.robots.at(asset::obsidian).set_cost(asset::clay, std::atoi(sm[5].str().c_str()));
			b.robots.at(asset::geode).set_cost(asset::ore, std::atoi(sm[6].str().c_str()));
			b.robots.at(asset::geode).set_cost(asset::obsidian, std::atoi(sm[7].str().c_str()));
			b.update_max_costs();
			blueprints.push_back(b);
		}
	}

	int total = 1;
	for (int i = 0; i < std::min(3, (int)blueprints.size()); ++i)
	{
		std::cout << "+";
		total *= get_score(blueprints.at(i), 32);
	}

	std::cout << "\n" << total;
}

void day19()
{
	//day19_1();
	day19_2();
}
