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
#include <functional>

class monkey
{
public:
	uint64_t inspect_count = 0;
	std::deque<uint64_t> items;
	std::function<uint64_t(uint64_t)> op;
	std::function<bool(uint64_t)> test;
	int true_dest = 0;
	int false_dest = 0;
};

void day11_1()
{
	std::array<monkey, 8> monkeys;

	monkeys[0].items = std::deque<uint64_t>{ 96, 60, 68, 91, 83, 57, 85 };
	monkeys[0].op = [](uint64_t old) { return old * 2; };
	monkeys[0].test = [](uint64_t in) { return (in % 17) == 0; };
	monkeys[0].true_dest = 2;
	monkeys[0].false_dest = 5;

	monkeys[1].items = std::deque<uint64_t>{ 75, 78, 68, 81, 73, 99 };
	monkeys[1].op = [](uint64_t old) { return old + 3; };
	monkeys[1].test = [](uint64_t in) { return (in % 13) == 0; };
	monkeys[1].true_dest = 7;
	monkeys[1].false_dest = 4;

	monkeys[2].items = std::deque<uint64_t>{ 69, 86, 67, 55, 96, 69, 94, 85 };
	monkeys[2].op = [](uint64_t old) { return old + 6; };
	monkeys[2].test = [](uint64_t in) { return (in % 19) == 0; };
	monkeys[2].true_dest = 6;
	monkeys[2].false_dest = 5;

	monkeys[3].items = std::deque<uint64_t>{ 88, 75, 74, 98, 80 };
	monkeys[3].op = [](uint64_t old) { return old + 5; };
	monkeys[3].test = [](uint64_t in) { return (in % 7) == 0; };
	monkeys[3].true_dest = 7;
	monkeys[3].false_dest = 1;

	monkeys[4].items = std::deque<uint64_t>{ 82 };
	monkeys[4].op = [](uint64_t old) { return old + 8; };
	monkeys[4].test = [](uint64_t in) { return (in % 11) == 0; };
	monkeys[4].true_dest = 0;
	monkeys[4].false_dest = 2;

	monkeys[5].items = std::deque<uint64_t>{ 72, 92, 92 };
	monkeys[5].op = [](uint64_t old) { return old * 5; };
	monkeys[5].test = [](uint64_t in) { return (in % 3) == 0; };
	monkeys[5].true_dest = 6;
	monkeys[5].false_dest = 3;

	monkeys[6].items = std::deque<uint64_t>{ 74, 61 };
	monkeys[6].op = [](uint64_t old) { return old * old; };
	monkeys[6].test = [](uint64_t in) { return (in % 2) == 0; };
	monkeys[6].true_dest = 3;
	monkeys[6].false_dest = 1;

	monkeys[7].items = std::deque<uint64_t>{ 76, 86, 83, 55 };
	monkeys[7].op = [](uint64_t old) { return old + 4; };
	monkeys[7].test = [](uint64_t in) { return (in % 5) == 0; };
	monkeys[7].true_dest = 4;
	monkeys[7].false_dest = 0;

	uint64_t m = 17 * 13 * 19 * 7 * 11 * 3 * 2 * 5;

	for (int round = 0; round < 10000; ++round)
	{
		for (auto& monkey : monkeys)
		{
			while (!monkey.items.empty())
			{
				monkey.inspect_count += 1;
				uint64_t item = monkey.items.front();
				monkey.items.pop_front();
				uint64_t after_inspect = monkey.op(item);
				after_inspect = after_inspect % m;
				monkeys[monkey.test(after_inspect) ? monkey.true_dest : monkey.false_dest].items.push_back(after_inspect);
			}
		}
	}

	std::sort(monkeys.begin(), monkeys.end(), [](const auto& a, const auto& b) {
		return a.inspect_count > b.inspect_count;
		});
	const uint64_t bus = monkeys[0].inspect_count * monkeys[1].inspect_count;
}

void day11_2()
{
	std::array<monkey, 4> monkeys;

	monkeys[0].items = std::deque<uint64_t>{ 79, 98 };
	monkeys[0].op = [](uint64_t old) { return old * 19; };
	monkeys[0].test = [](uint64_t in) { return (in % 23) == 0; };
	monkeys[0].true_dest = 2;
	monkeys[0].false_dest = 3;

	monkeys[1].items = std::deque<uint64_t>{ 54, 65, 75, 74 };
	monkeys[1].op = [](uint64_t old) { return old + 6; };
	monkeys[1].test = [](uint64_t in) { return (in % 19) == 0; };
	monkeys[1].true_dest = 2;
	monkeys[1].false_dest = 0;

	monkeys[2].items = std::deque<uint64_t>{ 79, 60, 97 };
	monkeys[2].op = [](uint64_t old) { return old * old; };
	monkeys[2].test = [](uint64_t in) { return (in % 13) == 0; };
	monkeys[2].true_dest = 1;
	monkeys[2].false_dest = 3;

	monkeys[3].items = std::deque<uint64_t>{ 74 };
	monkeys[3].op = [](uint64_t old) { return old + 3; };
	monkeys[3].test = [](uint64_t in) { return (in % 17) == 0; };
	monkeys[3].true_dest = 0;
	monkeys[3].false_dest = 1;

	uint64_t m = 23 * 19 * 13 * 17;

	for (int round = 0; round < 10000; ++round)
	{
		for (auto& monkey : monkeys)
		{
			while (!monkey.items.empty())
			{
				monkey.inspect_count += 1;
				uint64_t item = monkey.items.front();
				monkey.items.pop_front();
				uint64_t after_inspect = monkey.op(item);
				after_inspect = after_inspect % m;
				monkeys[monkey.test(after_inspect) ? monkey.true_dest : monkey.false_dest].items.push_back(after_inspect);
			}
		}
	}

	std::sort(monkeys.begin(), monkeys.end(), [](const auto& a, const auto& b) {
		return a.inspect_count > b.inspect_count;
		});
	const uint64_t bus = monkeys[0].inspect_count * monkeys[1].inspect_count;
}

void day11()
{
	day11_1();
	day11_2();
}
