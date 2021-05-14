#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <deque>
#include <condition_variable>
#include <chrono>
#include "map_reduce.cpp"

inline std::chrono::high_resolution_clock::time_point get_current_time_fenced()
{
    std::atomic_thread_fence(std::memory_order_seq_cst);
    auto res_time = std::chrono::high_resolution_clock::now();
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return res_time;
}

template<class D>
inline long long to_us(const D& d)
{
    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}


std::string map1(std::string element) {
    return element+element;
}

int test(std::string a) {
    return a.size();
}

int main() {
    std::vector<std::string> data;

    // fill data
    data.reserve(1000000);

    for (int i = 0; i < 1000000; i++) {
        data.emplace_back("testing");
    }

    auto start1 = get_current_time_fenced();
    int s = mapReduce(data, map1, test, true);
    auto stop1 = get_current_time_fenced();
    auto duration1 = stop1 - start1;

    int s2 = 0;
    auto start2 = get_current_time_fenced();
    for (std::string const k : data) {
        s2 += map1(k).size();
    }
    auto stop2 = get_current_time_fenced();
    auto duration2 = stop2 - start2;

    std::cout << s << ' ' << to_us(duration1) << std::endl;
    std::cout << s2 << ' ' << to_us(duration2) << std::endl;
    return 0;
}

