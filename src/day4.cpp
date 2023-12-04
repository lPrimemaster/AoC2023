#include <iostream>
#include <ranges>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include "common.h"

namespace rv = std::ranges::views;

void part1(const std::vector<std::string>& input)
{
    PPROF;
    using std::operator""sv;
    int npoints = 0;
    for(const auto& card : input)
    {
        auto game_info = RangeToStringView(*++(card | rv::split(": "sv)).begin());
        auto game_vals = game_info | rv::split('|');
        auto game_win  = *game_vals.begin() | rv::split(' ') | rv::transform([](auto&& s) { 
            return std::atoi(std::string(RangeToStringView(s)).c_str()); 
        });
        auto game_self = *++game_vals.begin() | rv::split(' ') | rv::transform([](auto&& s) {
            return std::atoi(std::string(RangeToStringView(s)).c_str());
        });

        int nmatch = 0;
        for(const auto& gw : game_win)
        {
            if(gw == 0) continue; // This comes from std::atoi(" ");
            if(std::any_of(game_self.begin(), game_self.end(), [gw](const auto& i) { return i == gw; }))
            {
                nmatch += 1;
            }
        }
        npoints += (nmatch > 0 ? static_cast<int>(std::pow(2.0, nmatch - 1)) : 0);
    }
    std::cout << npoints << std::endl;
}

void part2(const std::vector<std::string>& input)
{
    PPROF;
    using std::operator""sv;
    std::vector<int> win_count_map;
    win_count_map.resize(input.size());
    int id = 0;
    for(const auto& card : input)
    {
        auto game_info = RangeToStringView(*++(card | rv::split(": "sv)).begin());
        auto game_vals = game_info | rv::split('|');
        auto game_win  = *game_vals.begin() | rv::split(' ') | rv::transform([](auto&& s) { 
            return std::atoi(std::string(RangeToStringView(s)).c_str()); 
        });
        auto game_self = *++game_vals.begin() | rv::split(' ') | rv::transform([](auto&& s) {
            return std::atoi(std::string(RangeToStringView(s)).c_str());
        });

        int nmatch = 0;
        for(const auto& gw : game_win)
        {
            if(gw == 0) continue; // This comes from std::atoi(" ");
            if(std::any_of(game_self.begin(), game_self.end(), [gw](const auto& i) { return i == gw; }))
            {
                nmatch += 1;
            }
        }
        win_count_map[id++] = nmatch;
    }

    std::queue<int> cards;
    for(size_t i = 0; i < win_count_map.size(); i++)
        cards.push(static_cast<int>(i));
    
    // One could discard duplicated iterations and multiply in the end
    long long ncards = 0;
    while(!cards.empty())
    {
        int id = cards.front();
        for(int i = 0; i < win_count_map[id]; i++) cards.push(id + 1 + i);
        ncards += 1;
        cards.pop();
    }
    std::cout << ncards << std::endl;
}


int main()
{
    PPROF;
    const auto input = ReadInputTo<std::vector<std::string>>();
    part1(input);
    part2(input);
    return 0;
}
