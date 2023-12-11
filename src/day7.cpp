#include <iostream>
#include <vector>
#include <map>
#include <bitset>
#include <algorithm>
#include <numeric>
#include "common.h"

using Hand = std::pair<std::string, int>;

std::vector<Hand> parse(const std::vector<std::string>& input)
{
    PPROF;
    std::vector<Hand> hands;
    hands.reserve(input.size());
    for(const auto& line : input)
    {
        const std::string hand = line.substr(0, 5);
        const int bid = std::atoi(line.substr(6).c_str());
        hands.push_back(std::make_pair(hand, bid));
    }
    return hands;
}

std::int32_t hand_to_score(Hand& hand, const std::map<char, std::int32_t>& card_i32)
{
    std::map<char, int> card_count;
    std::int32_t score = 0;

    for(int i = 4; const char card : hand.first)
    {
        if(card_count.find(card) != card_count.end())
        {
            card_count[card] += 1;
        }
        else
        {
            card_count[card] = 1;
        }
        score |= card_i32.at(card) << (4 * i--); // High card calc
    }
    
    if(card_i32.at('J') == 0 && card_count.find('J') != card_count.end())
    {
        // J is now a mimic card of the card with the highest count
        int jcount = card_count['J'];
        card_count.erase('J');
        auto max_count = std::max_element(card_count.begin(), card_count.end(), [](const auto& a, const auto& b) { return a.second < b.second; });
        card_count[max_count->first] += jcount;
    }

    // High hand calc
    if(card_count.size() == 1)                                                                                                          score |= (7 << 20);
    else if(card_count.size() == 2 && std::any_of(card_count.begin(), card_count.end(), [](const auto& kv) { return kv.second == 4; })) score |= (6 << 20);
    else if(card_count.size() == 2 && std::any_of(card_count.begin(), card_count.end(), [](const auto& kv) { return kv.second == 3; })) score |= (5 << 20);
    else if(card_count.size() == 3 && std::any_of(card_count.begin(), card_count.end(), [](const auto& kv) { return kv.second == 3; })) score |= (4 << 20);
    else if(card_count.size() == 3 && std::any_of(card_count.begin(), card_count.end(), [](const auto& kv) { return kv.second == 2; })) score |= (3 << 20);
    else if(card_count.size() == 4)                                                                                                     score |= (2 << 20);
    else                                                                                                                                score |= (1 << 20);

    return score;
}

const static std::map<char, std::int32_t> card_i32 = {
        {'2', 1}, {'3', 2}, {'4', 3},
        {'5', 4}, {'6', 5}, {'7', 6},
        {'8', 7}, {'9', 8}, {'T', 9},
        {'J', 10}, {'Q', 11},
        {'K', 12}, {'A', 13}
    };
const static std::map<char, std::int32_t> card_i32_2 = {
        {'2', 1}, {'3', 2}, {'4', 3},
        {'5', 4}, {'6', 5}, {'7', 6},
        {'8', 7}, {'9', 8}, {'T', 9},
        {'J', 0}, {'Q', 11},
        {'K', 12}, {'A', 13}
    };


void part1(const std::vector<std::string>& input)
{
    PPROF;
    auto hands = parse(input);

    std::vector<std::pair<std::int32_t, int>> scores;
    scores.reserve(hands.size());
    
    for(auto& hand : hands)
    {
        scores.push_back(std::make_pair(hand_to_score(hand, card_i32), hand.second));
    }

    std::sort(scores.begin(), scores.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    int winnings = std::accumulate(scores.begin(), scores.end(), 0, [](int sum, const auto& s) { static int i = 1; return sum + s.second * i++; });
    std::cout << winnings << std::endl;
}

void part2(const std::vector<std::string>& input)
{
    PPROF;
    auto hands = parse(input);

    std::vector<std::pair<std::int32_t, int>> scores;
    scores.reserve(hands.size());
    
    for(auto& hand : hands)
    {
        scores.push_back(std::make_pair(hand_to_score(hand, card_i32_2), hand.second));
    }

    std::sort(scores.begin(), scores.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    int winnings = std::accumulate(scores.begin(), scores.end(), 0, [](int sum, const auto& s) { static int i = 1; return sum + s.second * i++; });
    std::cout << winnings << std::endl;
}

int main()
{
    PPROF;
    const std::vector<std::string> input = ReadInputTo<std::vector<std::string>>();
    part1(input);
    part2(input);
    return 0;
}
