#include <iostream>
#include <ranges>
#include <cmath>
#include <limits>
#include "common.h"

namespace rv = std::ranges::views;

std::pair<std::vector<unsigned int>, std::vector<unsigned int>> parse(const std::vector<std::string>& input)
{
    PPROF;
    std::vector<unsigned int> outs[2];

    for(size_t i = 0; i < input.size(); i++)
    {
        for(const auto& t : input[i] | rv::split(' '))
        {
            const auto& str = std::string(RangeToStringView(t));
            if(IsNumber(str)) 
            {
                auto v = std::atoi(str.c_str());
                if(v > 0) outs[i].push_back(v);
            }
        }
    }
    return std::make_pair(outs[0], outs[1]);
}

bool is_int(float x) { return std::round(x) - x == 0; }

std::pair<float, float> win_cond(unsigned long long t_max, unsigned long long d_min)
{
    // t * (tmax - t) - r = 0
    // -t**2 * tmax * t - r = 0
    // t = (-tmax +/- sqrt(tmax**2 - 4 * (-1) * (-r)))/( 2 * (-1))
    float disc = t_max * t_max - 4 * d_min;
    float disc_sqrt = std::sqrt(disc);
    return std::make_pair(
        (t_max - disc_sqrt) / 2.0,
        (t_max + disc_sqrt) / 2.0
    );
}

void part1(const std::vector<std::string>& input)
{
    PPROF;
    auto [times, dists] = parse(input);
    
    unsigned int permutations = 1;
    for(size_t i = 0; i < times.size(); i++) // GCC 12 no std::views::zip() ???
    {
        auto sol = win_cond(times[i], dists[i]);
        unsigned int num_sol = std::floor(sol.second) - std::ceil(sol.first) + 1;
        if(is_int(sol.first )) num_sol -= 1;
        if(is_int(sol.second)) num_sol -= 1;
        permutations *= num_sol;
    }

    std::cout << permutations << std::endl;
}

void part2(const std::vector<std::string>& input)
{
    PPROF;
    auto [times, dists] = parse(input);
    std::string strcat;
    for(const auto& t : times) strcat += std::to_string(t);
    unsigned int time = std::atoi(strcat.c_str());
    
    strcat.clear();
    for(const auto& d : dists) strcat += std::to_string(d);
    unsigned long long dist = std::stoull(strcat.c_str());

    auto sol = win_cond(time, dist);
    unsigned int num_sol = std::floor(sol.second) - std::ceil(sol.first) + 1;
    if(is_int(sol.first )) num_sol -= 1;
    if(is_int(sol.second)) num_sol -= 1;

    std::cout << num_sol << std::endl;
}

int main()
{
    PPROF;
    const std::vector<std::string> input = ReadInputTo<std::vector<std::string>>();
    part1(input);
    part2(input);
    return 0;
}
