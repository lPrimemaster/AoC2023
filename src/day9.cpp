#include <iostream>
#include <vector>
#include <ranges>
#include <numeric>
#include "common.h"

using Sequence = std::vector<std::int32_t>;

namespace rv = std::ranges::views;

std::vector<Sequence> parse(const std::vector<std::string>& input)
{
    PPROF;
    std::vector<Sequence> out;
    out.reserve(input.size());
    for(const auto& line : input)
    {
        auto seq = line | rv::split(' ') | rv::transform([](auto&& x) { return std::stoi(std::string(RangeToStringView(x)).c_str()); });
        Sequence sequence = RangeToVector(seq);
        out.push_back(sequence);
    }
    return out;
}

void part1_2(const std::vector<std::string>& input)
{
    PPROF;
    auto data = parse(input);
    
    std::int32_t sum_1 = 0;
    std::int32_t sum_2 = 0;
    for(auto& sequence : data)
    {
        std::vector<std::int32_t> chain_1, chain_2;
        for(size_t i = 0; i < sequence.size(); i++)
        {
            chain_1.push_back(sequence.back());
            chain_2.push_back(*(sequence.begin() + i));
            std::adjacent_difference(sequence.begin() + i, sequence.end(), sequence.begin() + i);
            if(std::all_of(sequence.begin() + i, sequence.end(), [](int& x) { return x == 0; })) break;
        }
        
        // Part 1
        std::int32_t prediction_1 = std::accumulate(chain_1.begin(), chain_1.end(), static_cast<std::int32_t>(0));
        sum_1 += prediction_1;
        
        // Part2
        std::reverse(chain_2.begin(), chain_2.end());
        std::int32_t prediction_2 = std::accumulate(chain_2.begin(), chain_2.end(), static_cast<std::int32_t>(0), [](std::int32_t v, std::int32_t x) {
            return x - v;
        });
        sum_2 += prediction_2;
    }
    std::cout << sum_1 << std::endl;
    std::cout << sum_2 << std::endl;
}

int main()
{
    PPROF;
    const auto input = ReadInputTo<std::vector<std::string>>();
    part1_2(input);
    return 0;
}
