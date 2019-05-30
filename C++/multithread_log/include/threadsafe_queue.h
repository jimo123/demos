

/****************************************/
// std::queue 中的方法：
//   * empty: 测试容器是否为空
//   * size: 返回容器大小
//   * front：使用下一个元素
//   * back: 使用最后一个元素
//   * push: 插入元素
//   * emplace(c++11): 构造并插入元素
//   * pop: 删除下一个元素
//   * swap(c++11)：交换两个队列的内容
/****************************************/


#ifndef __THREADSAFE_STACK_QUEUE__
#define __THREADSAFE_STACK_QUEUE__

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <queue>

using namespace std;


template<typename T = int>
class threadsafe_queue
{
private:
    boost::mutex mutex;
    boost::condition_variable_any cond;  // 条件变量
    queue<T> data;

public:
    threadsafe_queue(){}
    threadsafe_queue(threadsafe_queue const &other);
    ~threadsafe_queue();
    void push(T new_value);
    void wait_and_pop(T& value);
    shared_ptr<T> wait_and_pop();
    bool try_pop(T& value);
    shared_ptr<T> try_pop();
    bool empty();
    int size();
};

/***********************************/
// 拷贝够在函数
/***********************************/
template<typename T=int>
threadsafe_queue<T>::threadsafe_queue(threadsafe_queue const &other)
{
    boost::unique_lock<boost::mutex> lk(other.mutex); // 在构造函数中对互斥体mutex加锁，在析构中解锁，相对于lock_guard可以使用mutex.unlock()手动解锁
    //mutex.lock();
    data = other.data;
    //mutex.unlock();
}

/***********************************/
// 析构函数
/***********************************/
template<typename T=int>
threadsafe_queue<T>::~threadsafe_queue()
{
    boost::unique_lock<boost::mutex> lk(mutex);
    while(!data.empty())
        wait_and_pop();
}

template<typename T=int>
void threadsafe_queue<T>::push(T new_value)
{
    boost::unique_lock<boost::mutex> lk(mutex);
    data.push(new_value);
    cond.notify_one(); // 通知等待线程，notify_one仅通知一个等待线程，notify_all通知所有等待线程
}

template<typename T=int>
void threadsafe_queue<T>::wait_and_pop(T &value)
{
    boost::unique_lock<boost::mutex> lk(mutex);
    if(data.empty())
        cond.wait(lk); // 等待队中有数据，在wait中对mutex解锁，使其他线程能够使用临界区，并在返回之前加锁
    value = data.front();
    data.pop();
}
template<typename T=int>
bool threadsafe_queue<T>::try_pop(T& value)
{
    boost::unique_lock<boost::mutex> lk(mutex);
    if(!data.empty())
    {
        value = data.front(); // 返回首个元素
        data.pop(); // 删除首个元素
        return true;
    }
    return false;
}

template<typename T=int>
shared_ptr<T> threadsafe_queue<T>::wait_and_pop()
{
    boost::unique_lock<boost::mutex> lk(mutex);
    if(data.empty())
        cond.wait(lk);
    shared_ptr<T> res(make_shared<T>(data.front())); // 智能指针，可以使用模板函数 make_shared 创建对象, make_shared 需指定类型('<>'中)及参数('()'内)
    data.pop();
    return res;
}

template<typename T=int>
shared_ptr<T> threadsafe_queue<T>::try_pop()
{
    boost::unique_lock<boost::mutex> lk(mutex);
    if(!data.empty())
    {
        shared_ptr<T> res(make_shared<T>(data.front()));
        data.pop();
        return res;
    }
    return std::shared_ptr<T>();
}

template<typename T=int>
bool threadsafe_queue<T>::empty()
{
    boost::unique_lock<boost::mutex> lk(mutex);
    return data.empty();
}

template<typename T=int>
int threadsafe_queue<T>::size()
{
    boost::unique_lock<boost::mutex> lk(mutex);

    return data.size();
}


#endif
