#include <iostream>
#include <vector>
#include <ranges>
#include <numeric>
#include <map>
#include "common.h"

struct Node
{
    std::string name;
    char mask;
    std::string l, r;
};

namespace rv = std::ranges::views;
using namespace std::literals;

std::pair<std::string, std::map<std::string, Node>> parse(const std::vector<std::string>& input)
{
    PPROF;
    const std::string& instructions = input[0];
    
    std::map<std::string, Node> nodes;
    for(size_t i = 2; i < input.size(); i++)
    {
        Node node;
        auto split1 = input[i] | rv::split(" = "sv);
        auto split2 = std::string(RangeToStringView(*++split1.begin()));

        node.name = RangeToStringView(*split1.begin());
        node.mask = node.name[2];
        node.l    = split2.substr(1, 3);
        node.r    = split2.substr(6, 3);

        nodes.emplace(node.name, node);
    }

    return std::make_pair(instructions, nodes);
}

void part1(const std::vector<std::string>& input)
{
    PPROF;
    auto [instructions, nodes] = parse(input);
    Node& curr_node = nodes["AAA"];
    
    int i = 0;
    while(curr_node.name != "ZZZ")
    {
        curr_node = instructions[i++ % instructions.size()] == 'L' ? nodes.at(curr_node.l) : nodes.at(curr_node.r);
    }

    std::cout << i << std::endl;
}

void part2(const std::vector<std::string>& input)
{
    PPROF;
    auto [instructions, nodes] = parse(input);

    std::vector<const Node*> node_paths;
    for(const auto& kv : nodes)
    {
        if(kv.second.mask == 'A') node_paths.push_back(&kv.second); // Is a starting node
    }

    std::vector<int> i_per_node(node_paths.size());
    for(size_t n = 0; n < node_paths.size(); n++)
    {
        int i = 0;
        while(node_paths[n]->mask != 'Z')// && (nodes.at(node_paths[n]->l).mask == 'A' || nodes.at(node_paths[n]->r).mask == 'A'))
        {
            node_paths[n] = instructions[i++ % instructions.size()] == 'L' ? &nodes.at(node_paths[n]->l) : &nodes.at(node_paths[n]->r);
        }
        i_per_node[n] = i;
    }
    std::cout << std::accumulate(i_per_node.begin(), i_per_node.end(), 1UL, [](unsigned long lcm, int v) { return std::lcm(lcm, v); }) << std::endl;
}

int main()
{
    PPROF;
    const std::vector<std::string> input = ReadInputTo<std::vector<std::string>>();
    part1(input);
    part2(input);
    return 0;
}
