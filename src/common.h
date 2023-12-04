#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include <ranges>
#include <string_view>

template<typename T>
concept ReadableFmt = requires(T r)
{
    std::is_same_v<T, std::string>;
    std::is_same_v<T, std::vector<std::string>>;
};

template<ReadableFmt R>
static inline R ReadInputTo()
{
    std::vector<std::string> vec;
    std::string buffer;
    while(std::getline(std::cin, buffer)) vec.push_back(buffer);
    
    if constexpr(std::is_same_v<R, std::string>)
    {
        buffer.clear();
        for(const auto& s : vec) buffer += s, buffer += '\n';
        buffer.erase(buffer.end() - 1);
        return buffer;
    }
    else if constexpr(std::is_same_v<R, std::vector<std::string>>)
    {
        return vec;
    }
}

template<typename T>
const std::string_view RangeToStringView(const T& range)
{
    return std::string_view(&*range.begin(), std::ranges::distance(range));
}
