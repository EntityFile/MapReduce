#include <iostream>
#include <vector>
#include <chrono>
#include "map_reduce.cpp"

//inline std::chrono::high_resolution_clock::time_point get_current_time_fenced()
//{
//    std::atomic_thread_fence(std::memory_order_seq_cst);
//    auto res_time = std::chrono::high_resolution_clock::now();
//    std::atomic_thread_fence(std::memory_order_seq_cst);
//    return res_time;
//}
//
//template<class D>
//inline long long to_us(const D& d)
//{
//    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
//}


std::string mapFunction(std::string element) {
    return element+element;
}

int reduceFunction(std::string a) {
    return a.size();
}

template<typename T>
void thrMap(std::vector<T> &data, int start, int end) {
    for (int i = start; i < end; i++){
        data[i] = data[i]+data[i];
    }
}

int main() {
    std::vector<std::string> data;
    std::vector<std::string> data2;
    // fill data with strings
    data.reserve(1000000);
    for (int i = 0; i < 1000000; i++) {
        data.emplace_back("testing");
    }
    data2.reserve(data.size());
    for (int i=0; i<data.size(); i++)
        data2.push_back(data[i]);
    auto start1 = get_current_time_fenced();

    int result = mapReduceTBB(&data, mapFunction, reduceFunction);

    std::cout << "r1 " << result << std::endl;


//    int thr = 2;
//    int start;
//    int end;
//
//    std::vector<std::thread> v;
//    for (int k = 0; k < thr; k++){
//        start = data.size()/thr*k;
//        end = start + data.size()/thr;
//        v.emplace_back(thrMap, std::ref(data), start, end);
//    }
//
//    for (int j = 0; j < thr; j++) {
//        v[j].join();
//    }

    auto stop1 = get_current_time_fenced();
    auto duration1 = stop1 - start1;

    auto start2 = get_current_time_fenced();
    for (int i = 0; i < 1000000; i++) {
        data2[i] = mapFunction(data2[i]);
    }
    int r = 0;
    for (int k = 0; k < 1000000; k++) {
        r += reduceFunction(data2[k]);
    }
    auto stop2 = get_current_time_fenced();
    auto duration2 = stop2 - start2;

//    std::cout << "r2 " << r << std::endl;

    std::cout << "MapReduce library time:\t" << to_us(duration1) << std::endl;
    std::cout << "Single thread time:\t" << to_us(duration2) << std::endl;

    return 0;
}

