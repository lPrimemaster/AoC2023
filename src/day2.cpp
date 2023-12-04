#include <iostream>
#include <string_view>
#include <ranges>
#include <numeric>
#include <map>
#include "common.h"

namespace rv = std::ranges::views;

void part1_2(const std::string& input)
{
    int game_id_sum = 0;
    long long color_power = 0;
    int id = 0;
    auto games = input | rv::split('\n');
    for(const auto& game : games)
    {
        id++;
        bool game_passed = true;
        std::map<std::string, int> min_color;
        auto sets = *(++(game | rv::split(':')).begin()) | rv::split(';');
        for(const auto& set : sets)
        {
            auto colors = set
                | rv::split(',')
                | rv::transform([](auto&& c) -> std::pair<std::string, int> {
                    auto color_count = RangeToStringView(c).substr(1) | rv::split(' ');
                    auto color_count_it = color_count.begin();
                    auto count = std::atoi(std::string(RangeToStringView(*color_count_it)).c_str());
                    auto name = std::string(RangeToStringView(*++color_count_it));
                    return std::make_pair(name, count);
                })
            ;
            
            static std::map<std::string, int> conditions = {
                {"red"  , 12},
                {"green", 13},
                {"blue" , 14}
            };

            for(const auto& color : colors)
            {
                // Part 1
                if(conditions[color.first] < color.second)
                {
                    game_passed = false;
                }
                
                // Part 2
                if(min_color.count(color.first) != 0)
                {
                    min_color[color.first] = std::max(min_color[color.first], color.second);
                }
                else
                {
                    min_color[color.first] = color.second;
                }
            }
        }

        if(game_passed)
        {
            game_id_sum += id;
        }
        
        auto values = min_color | rv::values;
        color_power += std::accumulate(values.begin(), values.end(), 1LL, std::multiplies<long long>());
    }
    std::cout << game_id_sum << std::endl;
    std::cout << color_power << std::endl;
}

int main()
{
    const auto input = ReadInputTo<std::string>();
    part1_2(input);
    return 0;
}
