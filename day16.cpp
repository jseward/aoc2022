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

struct room
{
	std::string name;
	int rate = 0;
	std::vector<std::string> links;
	bool closed = true;
	std::map<room*, std::vector<room*>> paths;
	int score = 0;
};

std::map<std::string, std::unique_ptr<room>> rooms;

void set_paths_recursive(room* this_room, std::vector<room*> path)
{
	room* other = path.back();
	if (this_room->paths.find(other) == this_room->paths.end())
	{
		this_room->paths[other] = path;
		for (const auto& other_link : other->links)
		{
			std::vector<room*> other_path = path;
			other_path.push_back(rooms[other_link].get());
			set_paths_recursive(this_room, other_path);
		}
	}
}

room* get_next_room(room* from, int best_rate, int max_path, int minutes_remaining, room* exclude)
{
	std::vector<room*> pot_rooms;
	for (auto& r : rooms)
	{
		room* pot_room = r.second.get();
		if (pot_room != exclude)
		{
			bool ok = true;
			if (exclude != nullptr)
			{
				//ok = (from->paths[pot_room].size() < exclude->paths[pot_room].size());
			}

			if (ok)
			{
				//if (r.second->closed && (r.second->rate >= (best_rate - max_path)))
				if (pot_room->closed && (pot_room->rate > 8))
				{
					pot_rooms.push_back(pot_room);
				}
			}
		}
	}
	if (pot_rooms.empty())
	{
		for (auto& r : rooms)
		{
			room* pot_room = r.second.get();
			if (pot_room != exclude && pot_room->closed)
			{
				pot_rooms.push_back(pot_room);
			}
		}
	}


	room* next_room = nullptr;
	if (!pot_rooms.empty())
	{
		std::sort(pot_rooms.begin(), pot_rooms.end());
		int next_rooms_score = 0;
		do {

			int score = 0;
			room* curr = from;
			int curr_minutes_remaining = minutes_remaining;
			for (room* r : pot_rooms)
			{
				curr_minutes_remaining -= (int)curr->paths[r].size();
				curr = r;
				score += (curr_minutes_remaining * curr->rate);
				curr_minutes_remaining -= 1;
				if (curr_minutes_remaining <= 0)
				{
					break;
				}
				if (exclude != nullptr)
				{
					break;
				}
			}

			if (score > next_rooms_score)
			{
				next_room = pot_rooms.front();
				next_rooms_score = score;
			}
		} while (std::next_permutation(pot_rooms.begin(), pot_rooms.end()));
	}
	return next_room;
}

void day16_1()
{
	int best_rate = 0;

	std::ifstream file("day16.input");
	std::string line;
	while (std::getline(file, line))
	{
		const auto ss = split_string<char>(line, ";");
		std::string ss0 = std::string(ss.at(0));
		std::string ss1 = std::string(ss.at(1));
		const auto rr = split_string<char>(ss0, "=");

		auto room = std::make_unique<::room>();
		std::string room_name = std::string(rr.at(0));
		room->name = room_name;
		room->rate = std::atoi(std::string(rr.at(1)).c_str());
		best_rate = std::max(best_rate, room->rate);

		const auto other_rooms = split_string<char>(ss1, ",");
		for (const auto other_room : other_rooms)
		{
			room->links.push_back(std::string(other_room));
		}

		rooms[room_name] = std::move(room);
	}

	int max_path = 0;
	for (auto& r : rooms)
	{
		std::deque<std::vector<room*>> q;
		for (const auto& link : r.second->links)
		{
			q.push_back(std::vector<room*>{rooms[link].get()});
		}
		while (!q.empty())
		{
			std::vector<room*> p = q.front();
			q.pop_front();
			r.second->paths[p.back()] = p;
			max_path = std::max((int)p.size(), max_path);
			for (const auto& other : p.back()->links)
			{
				room* other_room = rooms[other].get();
				if (r.second->paths.find(other_room) == r.second->paths.end())
				{
					std::vector<room*> p2 = p;
					p2.push_back(other_room);
					q.push_back(p2);
				}
			}
		}
	}

	room* curr = rooms["AA"].get();
	room* dest = nullptr;
	int total_released = 0;
	int release_rate = 0;
	for (int minute = 0; minute < 30; ++minute)
	{
		std::cout << minute << "\n";
		int minutes_remaining = 30 - minute;

		total_released += release_rate;

		if (dest == nullptr)
		{
			dest = get_next_room(curr, best_rate, max_path, minutes_remaining, nullptr);
		}

		if (dest != nullptr)
		{
			if (curr == dest)
			{
				curr->closed = false;
				release_rate += curr->rate;
				dest = nullptr;
			}
			else
			{
				curr = curr->paths[dest].front();
			}
		}
	}

	std::cout << total_released << "\n"; //1796
}

void day16_2()
{
	int best_rate = 0;

	std::ifstream file("day16.input");
	std::string line;
	while (std::getline(file, line))
	{
		const auto ss = split_string<char>(line, ";");
		std::string ss0 = std::string(ss.at(0));
		std::string ss1 = std::string(ss.at(1));
		const auto rr = split_string<char>(ss0, "=");

		auto room = std::make_unique<::room>();
		std::string room_name = std::string(rr.at(0));
		room->name = room_name;
		room->rate = std::atoi(std::string(rr.at(1)).c_str());
		best_rate = std::max(best_rate, room->rate);

		const auto other_rooms = split_string<char>(ss1, ",");
		for (const auto other_room : other_rooms)
		{
			room->links.push_back(std::string(other_room));
		}

		rooms[room_name] = std::move(room);
	}

	int max_path = 0;
	for (auto& r : rooms)
	{
		std::deque<std::vector<room*>> q;
		for (const auto& link : r.second->links)
		{
			q.push_back(std::vector<room*>{rooms[link].get()});
		}
		while (!q.empty())
		{
			std::vector<room*> p = q.front();
			q.pop_front();
			r.second->paths[p.back()] = p;
			max_path = std::max((int)p.size(), max_path);
			for (const auto& other : p.back()->links)
			{
				room* other_room = rooms[other].get();
				if (r.second->paths.find(other_room) == r.second->paths.end())
				{
					std::vector<room*> p2 = p;
					p2.push_back(other_room);
					q.push_back(p2);
				}
			}
		}
	}

	room* curr0 = rooms["AA"].get();
	room* curr1 = rooms["AA"].get();
	room* dest0 = nullptr;
	room* dest1 = nullptr;
	int total_released = 0;
	int release_rate = 0;
	for (int minute = 0; minute < 26; ++minute)
	{
		std::cout << minute << "\n";
		int minutes_remaining = 26 - minute;

		total_released += release_rate;

		if (dest0 == nullptr)
		{
			dest0 = get_next_room(curr0, best_rate, max_path, minutes_remaining, dest1);
		}

		if (dest1 == nullptr)
		{
			dest1 = get_next_room(curr1, best_rate, max_path, minutes_remaining, dest0);
		}

		if (dest0 != nullptr)
		{
			if (curr0 == dest0)
			{
				curr0->closed = false;
				release_rate += curr0->rate;
				dest0 = nullptr;
			}
			else
			{
				curr0 = curr0->paths[dest0].front();
			}
		}

		if (dest1 != nullptr)
		{
			if (curr1 == dest1)
			{
				curr1->closed = false;
				release_rate += curr1->rate;
				dest1 = nullptr;
			}
			else
			{
				curr1 = curr1->paths[dest1].front();
			}
		}
	}

	std::cout << total_released << "\n"; //1734
}

void day16()
{
	//day16_1();
	day16_2();
}
