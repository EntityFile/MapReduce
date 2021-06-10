#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <deque>
#include <condition_variable>
#include <tbb/parallel_for.h>
#include <functional>
#include <omp.h>

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

template <typename T>
class Queue
{
public:
    int maxSize;

    explicit Queue(int& maxQueueSize) {
        maxSize = maxQueueSize;
    }

    int size() {
        return queue_.size();
    }

    void push_front(const T& item)
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        queue_.push_front(item);
        mlock.unlock();
        cond1.notify_one();
    }

    T pop()
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.empty())
        {
            cond1.wait(mlock);
        }
        auto item = queue_.front();
        queue_.pop_front();

        mlock.unlock();
        cond2.notify_one();

        return item;
    }

    void pop(T& item)
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.empty())
        {
            cond1.wait(mlock);
        }
        item = queue_.front();
        queue_.pop_front();

        mlock.unlock();
        cond2.notify_one();
    }

    void push(T& item)
    {
        std::unique_lock<std::mutex> mlock(mutex_);

        while (queue_.size() >= maxSize) {

            cond2.wait(mlock);
        }
        queue_.push_back(item);
        mlock.unlock();
        cond1.notify_one();
    }

    void push(T&& item)
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.size() >= maxSize) {
            cond2.wait(mlock);
        }
        queue_.push_back(std::move(item));
        mlock.unlock();
        cond1.notify_one();
    }

private:
    std::deque<T> queue_;
    std::mutex mutex_;
    std::condition_variable cond1;
    std::condition_variable cond2;
};

template<class T>
void pushDataToTheQueue(Queue<std::vector<T>> &queue, int mappingThreads, std::vector<T> *dataSet) {
    int sizePerThread = dataSet->size() / mappingThreads;

    for (int i = 0; i < mappingThreads; i++) {
        if (i == mappingThreads - 1) {
            std::vector<T> split(dataSet->begin() + sizePerThread * i, dataSet->end());
            queue.push(split);
        } else {
            std::vector<T> split(dataSet->begin() + sizePerThread * i, dataSet->begin() + sizePerThread * (i + 1));
            queue.push(split);
        }
    }
}

template<typename T>
void mapValuesUsingQueue(Queue<std::vector<T>> &queue, Queue<std::vector<T>> &mergingQueue, T (*f)(T)) {
    std::vector<T> data = queue.pop();
    for (int i = 0; i < data.size(); i++) {
        data[i] = f(data[i]);
    }

    mergingQueue.push(data);
}

template<typename T, typename K>
void reduceValuesUsingQueue(Queue<std::vector<T>> &mergingQueue, K (*f)(T), K &finalResult, std::mutex &m) {
    while (true) {
        std::vector<T> data = mergingQueue.pop();

        if (data.empty()) {
            mergingQueue.push(data);
            break;
        }

        K result = 0;

        for (int i = 0; i < data.size(); i++) {
            result += f(data[i]);
        }

        m.lock();
        finalResult += result;
        m.unlock();
    }

}

template<typename T>
void reduceDefault(std::vector<T> &data, int start, int end, std::vector<T> &finalResult, std::mutex &m) {
    std::vector<T> result;

    for (int i = start; i < end; i++) {
        // can be modified here
        result.template emplace_back(data[i]);
    }

    m.lock();
    finalResult.insert(finalResult.end(), result.begin(), result.end());
    m.unlock();
}


template<typename T, typename K>
void reduceValuesWithoutQueue(std::vector<T> *data, int start, int end, K (*f)(T), K &finalResult, std::mutex &m) {
    K result = 0;

    for (int i = start; i < end; i++) {
        result += f(data->at(i));
    }

    m.lock();
    finalResult += result;
    m.unlock();
}

template<typename T>
void mapValuesWithoutQueue(std::vector<T> *data, int start, int end, T (*f)(T)) {
    for (int i = start; i < end; i++) {
        data->at(i) = f(data->at(i));
    }
}

template<typename T>
void mapReduceTBB(std::vector<T> *dataSet, T (*f)(T)) {
    tbb::parallel_for( tbb::blocked_range<int>(0,dataSet->size()),
                       [&](tbb::blocked_range<int> r)
                       {
                           for (int i=r.begin(); i<r.end(); ++i)
                           {
                               dataSet->at(i) = f(dataSet->at(i));
                           }
                       });
}

template<typename T>
void mapReduceOpenMP(std::vector<T> *dataSet, T (*f)(T)) {
    #pragma omp parallel num_threads(4)
        for (int i=0; i<dataSet->size(); ++i)
        {
            dataSet->at(i) = f(dataSet->at(i));
        }
}


template<typename T, typename K>
K mapReduceOpenMP(std::vector<T> *dataSet, T (*f)(T), K (*f2)(T), int mappingThreads = 2) {
    #pragma omp parallel num_threads(mappingThreads)
    for (int i=0; i<dataSet->size(); ++i)
    {
        dataSet->at(i) = f(dataSet->at(i));
    }

    K result = 0;
    std::mutex m;

    #pragma omp parallel num_threads(4)
    for (int i=0; i<dataSet->size(); ++i)
    {
        m.lock();
        result += f2(dataSet->at(i));
        m.unlock();
    }

    return result;
}

template<typename T>
void mapReduce(std::vector<T> *dataSet, T (*f)(T), int mappingThreads = 2) {
    std::vector<std::thread> v;

    int sizePerThread1 = dataSet->size() / mappingThreads;

    for (int i = 0; i < mappingThreads; i++) {
        int start = sizePerThread1 * i;
        int end;

        if (i == mappingThreads - 1) {
            end = dataSet->size();
        } else {
            end =  sizePerThread1 * (i + 1);
        }

        v.template emplace_back(mapValuesWithoutQueue<T>, dataSet, start, end, f);
    }

    for (int j = 0; j < mappingThreads; j++) {
        v[j].join();
    }
}

template<typename T, typename K>
K mapReduceTBB(std::vector<T> *dataSet, T (*f)(T), K (*f2)(T)) {
    tbb::parallel_for( tbb::blocked_range<int>(0,dataSet->size()),
                       [&](tbb::blocked_range<int> r)
                       {
                           for (int i=r.begin(); i<r.end(); ++i)
                           {
                               dataSet->at(i) = f(dataSet->at(i));

                           }
                       });

    K result = 0;
    std::mutex m;

    tbb::parallel_for( tbb::blocked_range<int>(0,dataSet->size()),
                       [&](tbb::blocked_range<int> r)
                       {
                           for (int i=r.begin(); i<r.end(); ++i)
                           {
                               m.lock();
                               result += f2(dataSet->at(i));
                               m.unlock();
                           }
                       });

    return result;
}

template<typename T, typename K>
K mapReduce(std::vector<T> *dataSet, T (*f)(T), K (*f2)(T), bool useQueue = false,
            int mappingThreads = 2, int reducingThreads = 2) {
    int maxSize = 10000;

    Queue<std::vector<T>> queue(maxSize);
    Queue<std::vector<T>> mergingQueue(maxSize);

    std::vector<std::thread> v;
    K finalResult = 0;
    std::mutex m;

    if (!useQueue) {
        int sizePerThread1 = dataSet->size() / mappingThreads;
        int sizePerThread2 = dataSet->size() / reducingThreads;

        for (int i = 0; i < mappingThreads; i++) {
            int start = sizePerThread1 * i;
            int end;

            if (i == mappingThreads - 1) {
                end = dataSet->size();
            } else {
                end =  sizePerThread1 * (i + 1);
            }

            v.template emplace_back(mapValuesWithoutQueue<T>, dataSet, start, end, f);
        }

        for (int j = 0; j < mappingThreads; j++) {
            v[j].join();
        }

        for (int a = 0; a < reducingThreads; a++) {
            int start2 = sizePerThread2 * a;
            int end2;

            if (a == reducingThreads - 1) {
                end2 = dataSet->size();
            } else {
                end2 =  sizePerThread2 * (a + 1);
            }
            v.template emplace_back(reduceValuesWithoutQueue<T, K>, dataSet, start2, end2, f2, std::ref(finalResult), std::ref(m));
        }

        for (int k = mappingThreads; k < reducingThreads + mappingThreads; k++) {
            v[k].join();
        }
    } else {
        v.template emplace_back(pushDataToTheQueue<T>, std::ref(queue), mappingThreads, dataSet);

        for (int i = 0; i < mappingThreads; i++) {
            v.template emplace_back(mapValuesUsingQueue<T>, std::ref(queue), std::ref(mergingQueue), f);
        }

        for (int j = 0; j <= mappingThreads; j++) {
            v[j].join();
        }

        std::vector<T> deathPill;
        mergingQueue.push(deathPill);

        for (int ind = 0; ind < reducingThreads; ++ind) {
            v.template emplace_back(reduceValuesUsingQueue<T, K>, std::ref(mergingQueue), f2, std::ref(finalResult), std::ref(m));
        }

        for (int k = mappingThreads+1; k <= reducingThreads + mappingThreads; k++) {
            v[k].join();
        }
    }

    return finalResult;
}