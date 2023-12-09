#include <iostream>
#include <ranges>
#include <tuple>
#include <algorithm>
#include "common.h"

using std::operator""sv;
namespace rv = std::ranges::views;

using u64 = unsigned long long;
using i64 = long long;

u64 map(u64 input, const std::vector<std::tuple<u64, u64, u64>>& transform, bool reverse = false)
{
    if(reverse)
    {
        for(const auto& range : transform)
        {
            auto [dst, src, span] = range;
            if(input >= dst && input < dst + span)
                return input - (dst - src);
        }
    }
    else
    {
        for(const auto& range : transform)
        {
            auto [dst, src, span] = range;
            if(input >= src && input < src + span)
                return input + (dst - src);
        }
    }
    return input;
}

u64 map_recursive(u64 input, const std::vector<std::vector<std::tuple<u64, u64, u64>>>& transforms, u64 offset, bool reverse = false)
{
    u64 output = input;
    if(reverse)
    {
        for(u64 o = offset; o > 0; o--)
        {
            output = map(output, transforms[o - 1], true);
        }
    }
    else
    {
        for(u64 o = offset; o < transforms.size(); o++)
        {
            output = map(output, transforms[o]);
        }
    }
    return output;
}

using Transforms = std::vector<std::tuple<u64, u64, u64>>;

std::pair<std::vector<Transforms>, std::vector<u64>> parse(const std::string& input)
{
    PPROF;
    auto maps = input | rv::split("\n\n"sv);
    auto it = maps.begin();

    // Seeds
    auto seeds_input = RangeToStringView(*it++) | rv::split(": "sv);
    auto seeds_range = RangeToStringView(*++seeds_input.begin())
        | rv::split(" "sv)
        | rv::transform([](auto s) -> u64 {
            auto cvi = s | rv::common;
            return std::stoull(std::string(cvi.begin(), cvi.end()).c_str());
        });
    
    std::vector<std::vector<std::tuple<u64, u64, u64>>> transforms;

    for(auto cont = it; cont != maps.end(); cont++)
    {
        auto map  = RangeToStringView(*cont) | rv::split(":\n"sv);
        auto data = RangeToStringView(*++map.begin())
            | rv::split('\n')
            | rv::transform([](auto d) -> std::tuple<u64, u64, u64> {
                auto entries = d
                    | rv::split(' ')
                    | rv::transform([](auto e) -> u64 {
                        auto evi = e | rv::common;
                        return std::stoull(std::string(evi.begin(), evi.end()).c_str());
                    });
                auto it = entries.begin();
                auto destination = *it++;
                auto source = *it++;
                auto range = *it;
                return std::make_tuple(destination, source, range);
            });

        // Thing to convert ranges to other types is not in gcc yet
        std::vector<std::tuple<u64, u64, u64>> transform;
        for(const auto& d : data) transform.push_back(d);
        transforms.push_back(transform);
    }

    std::vector<u64> seeds;
    for(const auto& seed : seeds_range) seeds.push_back(seed);

    return std::make_pair(transforms, seeds);
}

void part1(const std::string& input)
{
    PPROF;
    auto [transforms, seeds] = parse(input);
    for(auto& seed : seeds)
    {
        seed = map_recursive(seed, transforms, 0);
    }
    std::cout << *std::min_element(seeds.begin(), seeds.end()) << std::endl;
}

void part2(const std::string& input)
{
    PPROF;
    auto [transforms, seeds] = parse(input);
    
    // We could do this with intersections
    // But I just want to solve this day anyways ...
    u64 loc = 0;
    bool found_loc = false;
    while(!found_loc)
    {
        i64 ipseed = static_cast<i64>(map_recursive(loc++, transforms, transforms.size(), true));
        if(ipseed < 0) continue;
        u64 pseed = static_cast<u64>(ipseed);
        for(auto it = seeds.begin(); it != seeds.end(); it += 2)
        {
            u64 start = *it;
            u64 span  = *(it+1);
            if(pseed >= start && pseed < start + span)
            {
                found_loc = true;
                break;
            }
        }
    }

    std::cout << loc - 1 << std::endl;
}

int main()
{
    PPROF;
    const auto input = ReadInputTo<std::string>();
    part1(input);
    part2(input);
    return 0;
}
