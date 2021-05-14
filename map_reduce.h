#ifndef MAPREDUCE_MAP_REDUCE_H
#define MAPREDUCE_MAP_REDUCE_H

template <typename T>
class Queue
{
public:
    int maxSize;

    explicit Queue(int& maxQueueSize);

    int size();

    void push_front(const T& item);

    T pop();

    void pop(T& item);

    void push(T& item);

    void push(T&& item);

private:
    std::deque<T> queue_;
    std::mutex mutex_;
    std::condition_variable cond1;
    std::condition_variable cond2;
};

template<class T>
void pushDataToTheQueue(Queue<std::vector<T>> &queue, int mappingThreads, std::vector<T> *dataSet);

template<typename T>
void mapValuesUsingQueue(Queue<std::vector<T>> &queue, Queue<std::vector<T>> &mergingQueue, T (*f)(T));

template<typename T, typename K>
void reduceValuesUsingQueue(Queue<std::vector<T>> &mergingQueue, K (*f)(T), K &finalResult, std::mutex &m);

template<typename T>
T defaultSum (T a, T b)

template<typename T>
std::vector<T> mapReduce(std::vector<T> dataSet, T (*f)(T));

template<typename T>
void mapValuesWithoutQueue(std::vector<T> &data, int start, int end, Queue<std::vector<T>> &mergingQueue, T (*f)(T));

template<typename T, typename K>
void reduceValuesWithoutQueue(std::vector<T> &data, int start, int end, K (*f)(T), K &finalResult, std::mutex &m);

template<typename T, typename K>
K mapReduce(std::vector<T> dataSet, T (*f)(T), K (*f2)(T));

#endif //MAPREDUCE_MAP_REDUCE_H


