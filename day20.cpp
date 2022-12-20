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

template<typename ContainerT, typename Pr>
auto find_index_if(const ContainerT& container, Pr predicate) -> std::optional<typename ContainerT::size_type>
{
	for (typename ContainerT::size_type i = 0; i < container.size(); ++i)
	{
		if (predicate(container.at(i)))
		{
			return i;
		}
	}
	return {};
}

template<typename ContainerT, typename ValueT>
auto find_index(const ContainerT& container, const ValueT& value) -> std::optional<typename ContainerT::size_type>
{
	return find_index_if(container, [&value](const ValueT& v) { return value == v; });
}

template<typename ContainerT> void erase_at_index(ContainerT& dst, typename ContainerT::size_type index)
{
	dst.erase(std::begin(dst) + static_cast<typename ContainerT::difference_type>(index));
}

template<typename ContainerT>
void insert_at_index(ContainerT& dst, typename ContainerT::size_type index, const typename ContainerT::value_type& value)
{
	dst.insert(std::begin(dst) + static_cast<typename ContainerT::difference_type>(index), value);
}

void day20_1()
{
	vector<std::unique_ptr<int>> initial_numbers;

	std::ifstream file("day20.input");
	std::string line;
	while (std::getline(file, line))
	{
		initial_numbers.push_back(std::make_unique<int>(std::atoi(line.c_str())));
	}

	vector<int*> numbers;
	for (auto& n : initial_numbers)
	{
		numbers.push_back(n.get());
	}

	for (int j = 0; j < initial_numbers.size(); ++j)
	{
		int* in = initial_numbers.at(j).get();
		int i = (int)find_index(numbers, in).value();
		int* n = numbers.at(i);
		assert(n == in);
		int off = *n;
		erase_at_index(numbers, i);

		int next = i + off;
		while (off < 0 && next <= 0)
		{
			next = (int)numbers.size() + next;
		}

		next = next % numbers.size();

		insert_at_index(numbers, next, n);
	}

	int base = (int)find_index_if(numbers, [](int* x) { return *x == 0; }).value();
	int n0 = *numbers.at((base + 1000) % numbers.size());
	int n1 = *numbers.at((base + 2000) % numbers.size());
	int n2 = *numbers.at((base + 3000) % numbers.size());
	int sum = n0 + n1 + n2;
	//13883
}

void day20_2()
{
	const int64_t decrypt_key = 811589153;

	vector<std::unique_ptr<int64_t>> initial_numbers;

	std::ifstream file("day20.input");
	std::string line;
	while (std::getline(file, line))
	{
		initial_numbers.push_back(std::make_unique<int64_t>(std::atoi(line.c_str()) * decrypt_key));
	}

	vector<int64_t*> numbers;
	for (auto& n : initial_numbers)
	{
		numbers.push_back(n.get());
	}

	for (int r = 0; r < 10; ++r)
	{
		//for (int j = 0; j < numbers.size(); ++j)
		//{
		//	std::cout << *numbers.at(j) << ",";
		//}
		//std::cout << "\n";

		for (int j = 0; j < initial_numbers.size(); ++j)
		{
			int64_t* in = initial_numbers.at(j).get();
			int i = (int)find_index(numbers, in).value();
			int64_t* n = numbers.at(i);
			assert(n == in);
			int64_t off = *n;
			erase_at_index(numbers, i);

			int64_t next = i + off;
			while (off < 0 && next <= 0)
			{
				int64_t m = std::max<int64_t>(1, std::abs(next) / (int64_t)numbers.size());
				next += (m * (int64_t)numbers.size());
			}

			next = next % numbers.size();

			insert_at_index(numbers, next, n);
		}
	}

	int base = (int)find_index_if(numbers, [](int64_t* x) { return *x == 0; }).value();
	int64_t n0 = *numbers.at((base + 1000) % numbers.size());
	int64_t n1 = *numbers.at((base + 2000) % numbers.size());
	int64_t n2 = *numbers.at((base + 3000) % numbers.size());
	int64_t sum = n0 + n1 + n2;
	//1431661695
	//1623178306 (too low)
	std::cout << "\n" << sum;
}

void day20()
{
	//day20_1();
	day20_2();
}
