#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include <stack>
#include <memory>
#include <ranges>
#include <string_view>
#include <iomanip>
#include <chrono>

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

// RAII Timer
class ScopedTimer
{
private:
    ScopedTimer(const char* scope_name, int line, const std::string& suffix = "")
        : scope_name(scope_name + suffix), line(line)
    {
        if(parents.empty()) print_on_end = true;
        _tp = std::chrono::high_resolution_clock::now();
    }

    struct Node
    {
        float millis;
        float micros;
        std::string name;
        int indent;
        std::vector<Node> children;
        bool use_micros;

        inline void print() const
        {
            for(int i = 0; i < indent; i++) std::cout << '\t';
            std::cout << "-> [" << name << "] Execution time : " << 
                    (use_micros ? micros : millis) << 
                    (use_micros ? "us." : "ms.") << std::endl;
        }
    };

public:
    ~ScopedTimer()
    {
        auto ms_d = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - _tp
        );
        _scope_stack.top()->use_micros = ms_d.count() < 1000;
        _scope_stack.top()->millis = ms_d.count() / 1000.0f;
        _scope_stack.top()->micros = ms_d.count() / 1.0f;
        _scope_stack.top()->name   = scope_name;
        _scope_stack.top()->indent = _scope_stack.size() - 1;
        _scope_stack.pop();

        if(print_on_end) PrintTimerTree();
    }
    
    ScopedTimer(const ScopedTimer& t) = delete;
    ScopedTimer(ScopedTimer&& t) = delete;
    
    inline static std::shared_ptr<ScopedTimer> New(const char* scope_name, int line, const std::string& suffix = "")
    {
        auto ptr = std::shared_ptr<ScopedTimer>(new ScopedTimer(scope_name, line, suffix));
        ptr->add_leaf();
        return ptr;
    }

    inline static void PrintTimerTree()
    {
        for(const auto& p : parents)
        {
            PrintNodeFull(p);
        }
    }

private:
    std::chrono::high_resolution_clock::time_point _tp;
    std::string scope_name;
    int line;
    bool print_on_end = false;
    inline static std::stack<Node*> _scope_stack;
    inline static std::vector<Node> parents;

    inline void add_leaf()
    {
        if(_scope_stack.empty())
        {
            parents.emplace_back();
            _scope_stack.push(&parents.back());
        }
        else
        {
            _scope_stack.top()->children.emplace_back();
            _scope_stack.push(&_scope_stack.top()->children.back());
        }
    }

    inline static void PrintNodeFull(const Node& node)
    {
        node.print();
        for(const auto& children : node.children)
        {
            PrintNodeFull(children);
        }
    }
};

#ifdef ENABLE_PPROF
#define PPROF auto _ = ScopedTimer::New(__func__, __LINE__, "()")
#define PPROF_NAMED(x) auto _ = ScopedTimer::New(x, __LINE__)
#else
#define PPROF
#define PPROF_NAMED(x)
#endif
