#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <optional>
#include <array>
#include <algorithm>
#include <map>

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

enum class hand
{
	rock = 1,
	paper = 2,
	scissors = 3
};

enum class outcome
{
	win = 6,
	tie = 3,
	loss = 0
};

struct round_instance
{
	hand theirs;
	hand yours;
};


outcome get_outcome(const round_instance& round)
{
	switch(round.theirs)
	{
	case hand::rock:
		switch(round.yours)
		{
		case hand::rock:
			return outcome::tie;
		case hand::paper:
			return outcome::win;
		case hand::scissors:
			return outcome::loss;
		}
	case hand::paper:
		switch(round.yours)
		{
		case hand::rock:
			return outcome::loss;
		case hand::paper:
			return outcome::tie;
		case hand::scissors:
			return outcome::win;
		}
	case hand::scissors:
		switch(round.yours)
		{
		case hand::rock:
			return outcome::win;
		case hand::paper:
			return outcome::loss;
		case hand::scissors:
			return outcome::tie;
		}
	}

	throw std::exception();
}

int get_score(const round_instance& round)
{
	return static_cast<int>(get_outcome(round)) + static_cast<int>(round.yours);
}

void day2_1()
{
	const std::map<std::string, hand>  theirs_map{
		{ "A", hand::rock },
		{ "B", hand::paper },
		{ "C", hand::scissors }
	};
	const std::map<std::string, hand>  yours_map{
		{ "X", hand::rock },
		{ "Y", hand::paper },
		{ "Z", hand::scissors }
	};

	std::vector<round_instance> rounds;
	std::ifstream file("day2.input");
	std::string line;
	while (std::getline(file, line))
	{
		const auto split = split_string<char>(line, " ");
		rounds.emplace_back(theirs_map.at(std::string(split.at(0))), yours_map.at(std::string(split.at(1))));
	}

	int total_score = 0;
	for(const auto& round : rounds)
	{
		total_score += get_score(round);
	}
	std::cout << total_score << "\n";
}

hand get_hand_needed(hand their_hand, outcome outcome)
{
	switch(their_hand)
	{
	case hand::rock:
		switch(outcome)
		{
		case outcome::tie:
			return hand::rock;
		case outcome::win:
			return hand::paper;
		case outcome::loss:
			return hand::scissors;
		}
	case hand::paper:
		switch(outcome)
		{
		case outcome::tie:
			return hand::paper;
		case outcome::win:
			return hand::scissors;
		case outcome::loss:
			return hand::rock;
		}
	case hand::scissors:
		switch(outcome)
		{
		case outcome::tie:
			return hand::scissors;
		case outcome::win:
			return hand::rock;
		case outcome::loss:
			return hand::paper;
		}
	}

	throw std::exception();	
}

void day2_2()
{
	const std::map<std::string, hand>  theirs_map{
		{ "A", hand::rock },
		{ "B", hand::paper },
		{ "C", hand::scissors }
	};
	const std::map<std::string, outcome>  yours_map{
		{ "X", outcome::loss },
		{ "Y", outcome::tie },
		{ "Z", outcome::win }
	};

	std::vector<round_instance> rounds;
	std::ifstream file("day2.input");
	std::string line;
	while (std::getline(file, line))
	{
		const auto split = split_string<char>(line, " ");
		const hand their_hand = theirs_map.at(std::string(split.at(0)));
		const auto outcome = yours_map.at(std::string(split.at(1)));
		rounds.emplace_back(their_hand, get_hand_needed(their_hand, outcome));
	}

	int total_score = 0;
	for(const auto& round : rounds)
	{
		total_score += get_score(round);
	}
	std::cout << total_score << "\n";
}

void day2()
{
	//day2_1();
	day2_2();
}
