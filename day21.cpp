#include <iostream>
#include <fstream>
#include <assert.h>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
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

class yell_result
{
public:
	std::string op;
	std::string m0;
	std::string m1;
};

class monkey21
{
public:
	std::string name;
	std::optional<int64_t> yell_number;
	yell_result yell_result;
};

vector<monkey21> read_monkeys()
{
	vector<monkey21> monkeys;

	std::ifstream file("day21.input");
	std::string line;
	while (std::getline(file, line))
	{
		const auto ss = split_string<char>(line, ": ");

		monkey21 m;
		string s;
		string wtf;
		m.name = string(ss.at(0));

		const vector<const char*> ops = {
			" + ",
			" - ",
			" * ",
			" / "
		};
		for (const char* op : ops)
		{
			const auto sr = split_string<char>(ss.at(1), op);
			if (sr.size() == 2)
			{
				m.yell_result.op = string(op);
				m.yell_result.m0 = string(sr.at(0));
				m.yell_result.m1 = string(sr.at(1));
				break;
			}
		}

		if (m.yell_result.op.empty())
		{
			m.yell_number = std::atoi(string(ss.at(1)).c_str());
		}

		monkeys.push_back(m);
	}

	return monkeys;
}

void day21_1()
{
	vector<string> unyelled;

	unordered_map<string, monkey21> monkey_map;
	for (auto& monkey : read_monkeys())
	{
		monkey_map[monkey.name] = monkey;
		if (!monkey.yell_number.has_value())
		{
			unyelled.push_back(monkey.name);
		}
	}

	int n = 0;
	while (!monkey_map["root"].yell_number.has_value())
	{
		auto iter = unyelled.begin();
		while (iter != unyelled.end())
		{
			auto& monkey = monkey_map[*iter];
			if (
				monkey_map[monkey.yell_result.m0].yell_number.has_value() &&
				monkey_map[monkey.yell_result.m1].yell_number.has_value())
			{
				if (monkey.yell_result.op == " + ")
				{
					monkey.yell_number = monkey_map[monkey.yell_result.m0].yell_number.value() + monkey_map[monkey.yell_result.m1].yell_number.value();
				}
				else if (monkey.yell_result.op == " - ")
				{
					monkey.yell_number = monkey_map[monkey.yell_result.m0].yell_number.value() - monkey_map[monkey.yell_result.m1].yell_number.value();
				}
				else if (monkey.yell_result.op == " * ")
				{
					monkey.yell_number = monkey_map[monkey.yell_result.m0].yell_number.value() * monkey_map[monkey.yell_result.m1].yell_number.value();
				}
				else if (monkey.yell_result.op == " / ")
				{
					monkey.yell_number = monkey_map[monkey.yell_result.m0].yell_number.value() / monkey_map[monkey.yell_result.m1].yell_number.value();
				}
				else
				{
					assert(false);
				}

				iter = unyelled.erase(iter);
			}
			else
			{
				++iter;
			}
		}
		++n;
	}

	int64_t root = monkey_map["root"].yell_number.value();
	std::cout << root; // 51928383302238
}

int64_t get_yell_number(const vector<monkey21>& monkeys, string name)
{
	vector<string> unyelled;
	unordered_map<string, monkey21> monkey_map;
	for (auto& monkey : read_monkeys())
	{
		monkey_map[monkey.name] = monkey;
		if (!monkey.yell_number.has_value())
		{
			unyelled.push_back(monkey.name);
		}
	}

	while (!monkey_map[name].yell_number.has_value())
	{
		auto iter = unyelled.begin();
		while (iter != unyelled.end())
		{
			auto& monkey = monkey_map[*iter];
			if (
				monkey_map[monkey.yell_result.m0].yell_number.has_value() &&
				monkey_map[monkey.yell_result.m1].yell_number.has_value())
			{
				if (monkey.yell_result.op == " + ")
				{
					monkey.yell_number = monkey_map[monkey.yell_result.m0].yell_number.value() + monkey_map[monkey.yell_result.m1].yell_number.value();
				}
				else if (monkey.yell_result.op == " - ")
				{
					monkey.yell_number = monkey_map[monkey.yell_result.m0].yell_number.value() - monkey_map[monkey.yell_result.m1].yell_number.value();
				}
				else if (monkey.yell_result.op == " * ")
				{
					monkey.yell_number = monkey_map[monkey.yell_result.m0].yell_number.value() * monkey_map[monkey.yell_result.m1].yell_number.value();
				}
				else if (monkey.yell_result.op == " / ")
				{
					monkey.yell_number = monkey_map[monkey.yell_result.m0].yell_number.value() / monkey_map[monkey.yell_result.m1].yell_number.value();
				}
				else
				{
					assert(false);
				}

				iter = unyelled.erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}

	return monkey_map[name].yell_number.value();
}

void make_eq_rec(const unordered_map<string, monkey21>& monkey_map, string name)
{
	if(name == "humn")
	{
		std::cout << "humn";
	}
	else
	{
		const auto& monkey = monkey_map.at(name);
		if(monkey.yell_number.has_value())
		{
			std::cout << monkey.yell_number.value();
		}
		else
		{
			std::cout << "(";
			make_eq_rec(monkey_map, monkey.yell_result.m0);
			std::cout << monkey.yell_result.op;
			make_eq_rec(monkey_map, monkey.yell_result.m1);
			std::cout << ")";
		}
	}
}

void day21_2()
{
	const auto monkeys = read_monkeys();
	unordered_map<string, monkey21> monkey_map;
	for (auto& monkey : read_monkeys())
	{
		monkey_map[monkey.name] = monkey;
	}

	monkey_map["humn"].yell_number.reset();

	auto& root = monkey_map["root"].yell_result;
	make_eq_rec(monkey_map, root.m0);
	std::cout << " = ";
	make_eq_rec(monkey_map, root.m1);
}

void day21()
{
	//day21_1();
	day21_2();
}
