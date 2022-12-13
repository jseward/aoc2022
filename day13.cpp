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

struct packet
{
	std::optional<int> value;
	std::vector<packet> packets;

	packet() {}
	packet(int v) : value(v) {}
};

packet read_packet(std::string& s)
{
	if(!s.starts_with("["))
	{
		const auto end_pos = s.find_first_of(",]");
		assert(end_pos != std::string::npos);
		if(end_pos == 0)
		{
			return packet();
		}		
		const auto vs = s.substr(0, end_pos);
		s = s.substr(end_pos);
		return packet(std::atoi(vs.c_str()));
	}

	packet p;
	while(true)
	{
		s = s.substr(1);
		p.packets.push_back(read_packet(s));
		if(s.starts_with(","))
		{
			continue;
		}
		if(s.starts_with("]"))
		{
			s = s.substr(1);
			break;
		}
		assert(false);
	}
	return p;
}

int compare_packets(const packet& left, const packet& right)
{
	if(left.value.has_value() && right.value.has_value())
	{
		if(left.value.value() < right.value.value())
		{
			return -1;
		}

		if(left.value.value() > right.value.value())
		{
			return +1;
		}

		return 0;
	}

	if(left.value.has_value() && !right.value.has_value())
	{
		packet lp;
		lp.packets.push_back(left);
		return compare_packets(lp, right);
	}

	if(!left.value.has_value() && right.value.has_value())
	{
		packet rp;
		rp.packets.push_back(right);
		return compare_packets(left, rp);
	}

	size_t i = 0;
	while(i < left.packets.size())
	{
		if(i >= right.packets.size())
		{
			return +1;
		}

		const int inner_compare = compare_packets(left.packets.at(i), right.packets.at(i));
		if(inner_compare != 0)
		{
			return inner_compare;
		}

		++i;
	}

	if(i < right.packets.size())
	{
		return -1;
	}

	return 0;
}

void day13_1()
{
	std::vector<int> right_orders;
	int index = 1;

	std::ifstream file("day13.input");
	std::string line;
	while (std::getline(file, line))
	{
		auto left = read_packet(line);
		std::getline(file, line);
		auto right = read_packet(line);
		std::getline(file, line);

		if(compare_packets(left, right) <= 0)
		{
			right_orders.push_back(index);
		}

		index += 1;
	}

	int sum = 0;
	for(const int ro: right_orders)
	{
		sum += ro;
	}

	// 5632 (too low)
	// 6118 (too high)
	// 5659!
}

void day13_2()
{
	std::ifstream file("day13.input");
	std::string line;
	while (std::getline(file, line))
	{
		auto left = read_packet(line);
		std::getline(file, line);
		auto right = read_packet(line);
		std::getline(file, line);

		if(compare_packets(left, right) <= 0)
		{
			right_orders.push_back(index);
		}

		index += 1;
	}
}

void day13()
{
	day13_1();
	day13_2();
}
