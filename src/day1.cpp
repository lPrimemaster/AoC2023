#include <iostream>
#include <ranges>
#include <string_view>
#include <cctype>
#include <algorithm>
#include <numeric>
#include <map>
#include "common.h"

namespace rv = std::ranges::views;

static std::map<std::string_view, int> str_int_map = {
    { "one", 1 },
    { "two", 2 },
    { "three", 3 },
    { "four", 4 },
    { "five", 5 },
    { "six", 6 },
    { "seven", 7 },
    { "eight", 8 },
    { "nine", 9 }
};

void part1(const std::string& input)
{
    auto instructions = input
        | rv::split('\n')
        | rv::transform([](auto&& s) -> int { 
            auto i = std::string_view(&*s.begin(), std::ranges::distance(s))
                | rv::transform([](auto&& s) -> char { return std::isdigit(s) ? s : ' '; });
            
            auto svalue = std::string(i.begin(), i.end());
            svalue.erase(std::remove(svalue.begin(), svalue.end(), ' '), svalue.end());
            return std::atoi((std::string(1, *svalue.begin()) + *(svalue.end()-1)).c_str());
        })
    ;

    int sum = std::accumulate(instructions.begin(), instructions.end(), 0);
    std::cout << sum << std::endl;
}

void part2(const std::string& input)
{
    auto instructions = input
        | rv::split('\n')
        | rv::transform([](auto&& s) -> int {
            auto i = std::string_view(&*s.begin(), std::ranges::distance(s));

            // This two statements are just a cleaner way to do part1, but I will keep it
            auto first = std::ranges::find_if(i, [](auto c) { return std::isdigit(c); });
            auto last = std::ranges::find_if(i | std::views::reverse, [](auto c) { return std::isdigit(c); });
            
            auto first_str = std::string_view(i.begin(), first);
            auto last_str = std::string_view((last+1).base(), i.end());
            
            int value = 10 * (*first - '0');

            decltype(first_str)::size_type it_min = -1;

            for(const auto& kv : str_int_map)
            {
                auto it = first_str.find(kv.first);
                if(it != std::string_view::npos && it < it_min)
                {
                    it_min = it;
                    value = 10 * kv.second;
                }
            }

            value += *last - '0';
            int tmp_value = 0;
            decltype(it_min) it_max = 0;
            for(const auto& kv : str_int_map)
            {
                auto it = last_str.rfind(kv.first);
                if(it != std::string_view::npos && it > it_max)
                {
                    it_max = it;
                    tmp_value = kv.second;
                }
            }

            if(tmp_value != 0)
            {
                value -= *last - '0';
                value += tmp_value;
            }

            return value;
        })
    ;

    int sum = std::accumulate(instructions.begin(), instructions.end(), 0);
    std::cout << sum << std::endl;
}

int main()
{
    ScopedTimer _;
    const std::string input = ReadInputTo<std::string>();
    part1(input);
    part2(input);

    return 0;
}

