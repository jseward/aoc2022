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
	initial_s.minute = 0;
	initial_s.num_robots.at(asset::ore) = 1;

	int most_geodes = 0;

	vector<state> states;
	states.reserve(999999999);
	states.push_back(initial_s);
	while (!states.empty())
	{
		auto s = states.back();
		states.pop_back();

		s.minute += 1;
		if (s.minute > max_minutes)
		{
			if (s.num_assets.at(asset::geode) > most_geodes)
			{
				most_geodes = s.num_assets.at(asset::geode);
			}
		}
		else
		{
			std::vector<int> robot_priorities;
			for (int robot = 0; robot < 4; ++robot)
			{
				if (can_afford(s.num_assets, blueprint.robots.at(robot).cost))
				{
					if (robot > 1)
					{
						robot_priorities.clear();
					}

					robot_priorities.push_back(robot);
				}
			}

			for (int robot : robot_priorities)
			{
				state ns = s;
				ns.collect();
				ns.purchase(robot, blueprint.robots.at(robot).cost);
				states.push_back(ns);
			}

			s.collect();
			states.push_back(s);
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
