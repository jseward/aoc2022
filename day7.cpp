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

struct file
{
	std::string name;
	size_t size;
};

struct dir
{
	std::string name;
	dir* parent = nullptr;
	std::vector<file> files;
	std::vector<std::unique_ptr<dir>> sub_dirs;
	size_t total_size = 0;

public:
	void update_total_size_recursive(std::vector<dir*>& at_most, size_t tolerance)
	{
		for (const auto& file : files)
		{
			total_size += file.size;
		}

		for (auto& sub_dir : sub_dirs)
		{
			sub_dir->update_total_size_recursive(at_most, tolerance);
			total_size += sub_dir->total_size;
		}

		if (total_size <= tolerance)
		{
			at_most.push_back(this);
		}
	}

	void get_to_free_recursive(std::vector<dir*>& to_free, size_t tolerance)
	{
		if (total_size >= tolerance)
		{
			to_free.push_back(this);
		}

		for (auto& sub_dir : sub_dirs)
		{
			sub_dir->get_to_free_recursive(to_free, tolerance);
		}
	}
};

void day7_1()
{
	std::vector<std::string> lines;
	std::ifstream file("day7.input");
	std::string line;
	while (std::getline(file, line))
	{
		lines.push_back(line);
	}

	std::unique_ptr<dir> root = std::make_unique<dir>();
	dir* current_dir = root.get();

	size_t i = 0;
	while (i < lines.size())
	{
		const auto& line = lines.at(i);
		if (line.starts_with("$ cd"))
		{
			std::string to = line.substr(5);
			if (to.starts_with("/"))
			{
				current_dir = root.get();
			}
			else if (to.starts_with(".."))
			{
				current_dir = current_dir->parent;
			}
			else
			{
				auto sub_dir_iter = std::find_if(current_dir->sub_dirs.begin(), current_dir->sub_dirs.end(), [to](const auto& sub_dir) { return sub_dir->name == to; });
				if (sub_dir_iter != current_dir->sub_dirs.end())
				{
					current_dir = (*sub_dir_iter).get();
				}
				else
				{
					current_dir->sub_dirs.push_back(std::make_unique<dir>());
					current_dir->sub_dirs.back()->name = to;
					current_dir->sub_dirs.back()->parent = current_dir;
					current_dir = current_dir->sub_dirs.back().get();
				}
			}

			++i;
		}
		else if (line.starts_with("$ ls"))
		{
			const bool duplicate = !current_dir->files.empty();

			++i;
			while (i < lines.size() && !lines.at(i).starts_with("$"))
			{
				if (!lines.at(i).starts_with("dir"))
				{
					if (!duplicate)
					{
						const auto parts = split_string<char>(lines.at(i), " ");
						current_dir->files.push_back({});
						current_dir->files.back().name = parts.at(1);
						current_dir->files.back().size = std::atoi(std::string(parts.at(0)).c_str());
					}
				}
				++i;
			}
		}
		else
		{
			throw std::exception();
		}
	}

	std::vector<dir*> at_most;
	root->update_total_size_recursive(at_most, 100000);

	size_t total_at_most = 0;
	for (const dir* dir : at_most)
	{
		total_at_most += dir->total_size;
	}

	// 2061777

	const size_t total = 70000000;
	const size_t free = total - root->total_size;
	const size_t size_to_free = 30000000 - free;

	std::vector<dir*> to_free;
	root->get_to_free_recursive(to_free, size_to_free);
	std::sort(to_free.begin(), to_free.end(), [](const dir* a, const dir* b) { return a->total_size < b->total_size; });
}

void day7_2()
{
}

void day7()
{
	day7_1();
	day7_2();
}
