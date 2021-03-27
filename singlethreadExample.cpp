#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <numeric>
#include <unordered_map>



class MapFunc
{
public:
    std::unordered_map<std::string, size_t> operator()(std::vector<std::string>& elements) const
    {
        std::unordered_map<std::string, size_t> m;
        for_each(begin(elements), end(elements), [&m](const std::string& elem)
        {
            m[elem]++;
        });
        return m;
    }
};

struct ReduceFunc : std::binary_function<std::unordered_map<std::string, size_t>,
        std::unordered_map<std::string, size_t>, std::unordered_map<std::string, size_t>>
{
    std::unordered_map<std::string, size_t> operator() (
        const std::unordered_map<std::string, size_t>& x,
        const std::unordered_map<std::string, size_t>& y) const
{
    std::unordered_map<std::string, size_t> ret(x);
    for_each(begin(y), end(y), [&ret](const std::pair<std::string, size_t>& pr) {
        auto key = pr.first;
        auto val = pr.second;
        ret[key] += val;
    });
    return ret;
}
};

int main()
{
    // File 1
    std::vector<std::string> v1;
    v1.emplace_back("word1"); //1
    v1.emplace_back("word1"); //2
    v1.emplace_back("word2");
    v1.emplace_back("word3");
    v1.emplace_back("word4");

    // File 3
    std::vector<std::string> v3;
    v3.emplace_back("word1"); //1
    v3.emplace_back("word1"); //2
    v3.emplace_back("word2");
    v3.emplace_back("word3");
    v3.emplace_back("word4");

    // File 2
    std::vector<std::string> v2;
    v2.emplace_back("word5");
    v2.emplace_back("word6");
    v2.emplace_back("word7");
    v2.emplace_back("word8");
    v2.emplace_back("word1"); //3

    std::vector<std::vector<std::string>> v;
    v.push_back(v1);
    v.push_back(v2);
    v.push_back(v3);

    std::vector<std::unordered_map<std::string, size_t>> map(v.size());

    // Mapping. Multithreading option - Parallel Patterns Library (ppl.h.parallel_transform())
    std::transform(begin(v), end(v), begin(map), MapFunc());

    for(const auto& elem : map) {
        for(const auto& pair: elem){
            std::cout << pair.first << " " << pair.second << std::endl;
        }
        std::cout << " " << std::endl;
    }

    // Reducing. Multithreading option - Parallel Patterns Library (ppl.h.parallel_reduce())
    std::unordered_map<std::string, size_t> result = std::accumulate(
            begin(map), end(map), std::unordered_map<std::string, size_t>(), ReduceFunc());

    std::cout << "\"word1\" occurs " << result.at("word1") << " times. " << std::endl;
}