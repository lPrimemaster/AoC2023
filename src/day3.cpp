#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include <algorithm>
#include "common.h"

void part1_2(const std::vector<std::string>& input)
{
    using NumberInfo = std::tuple<int, int, std::pair<size_t, size_t>>;
    std::vector<NumberInfo> numbers; // value, span, location (x, y)
    for(size_t i = 0; i < input.size(); i++) // views::enumerate not in gcc 12 =(
    {
        const auto& line = input[i];
        auto it = line.begin();
        while(it != line.end())
        {
            // std::cout << *it << std::endl;
            if(std::isdigit(*it))
            {
                auto start = it++;
                int span = 2;
                while(it != line.end() && std::isdigit(*it++))
                {
                    span++;
                }
                numbers.push_back(std::make_tuple(
                    std::atoi(std::string(start, it).c_str()),
                    span,
                    std::make_pair(std::distance(line.begin(), start), i)
                ));
                if(it == line.end()) break;
            }
            else
            {
                it++;
            }
        }
    }

    // Check the adjacent rows for non '.' chars on span +- 1
    long long sum = 0;
    long long sum_gears = 0;
    std::map<size_t, int> possible_gears;
    for(const auto& number : numbers)
    {
        int value = std::get<0>(number); 
        int span  = std::get<1>(number); 
        auto loc  = std::get<2>(number);
 
        for(const auto& i : {-1, 0, 1})
        {
            size_t adj = loc.second + i;
            if(adj > input.size()) continue;

            // Part 2
            int which = -1;
            if(std::any_of(
                input[adj].begin() + std::clamp(loc.first - 1, static_cast<size_t>(0), loc.first),
                input[adj].begin() + 
                    (loc.first + span > input[adj].size() ? input[adj].size() : loc.first + span),
                [&which](const auto& c) { which++; return c == '*'; }
            ))
            {
                if(loc.first == 0) which++;
                size_t loc_gear = loc.first - 1 + which + (loc.second + i) * input[0].size();
                if(possible_gears.count(loc_gear))
                {
                    sum_gears += possible_gears[loc_gear] * value;
                    possible_gears.erase(loc_gear);
                }
                else
                {
                    possible_gears[loc_gear] = value;
                }
            }

            // Part 1
            if(!std::all_of(
                input[adj].begin() + std::clamp(loc.first - 1, static_cast<size_t>(0), loc.first),
                input[adj].begin() + 
                    (loc.first + span > input[adj].size() ? input[adj].size() : loc.first + span),
                [](const auto& c) { return c == '.' || std::isdigit(c); }
            ))
            {
                sum += value;
                break;
            }
        }
    }
    std::cout << sum << std::endl;
    std::cout << sum_gears << std::endl;
}

int main()
{
    auto input = ReadInputTo<std::vector<std::string>>();
    part1_2(input);
    return 0;
}
