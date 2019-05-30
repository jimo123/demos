#include <threadsafe_stack_queue.h>


template<typename T=int>
threadsafe_stack<T>::threadsafe_stack(const threadsafe_stack& other)
{
    boost::lock_guard<boost::mutex> lk(other.mutex);
    //other.mutex.lock();
    data = other.data;
    //other.mutex.unlock();
}

template<typename T=int>
void threadsafe_stack<T>::push(T new_value)
{
    boost::lock_guard<boost::mutex> lk(mutex);
    data.push(new_value);
}

template<typename T=int>
shared_ptr<T> threadsafe_stack<T>::pop()
{
    boost::lock_guard<boost::mutex> lk(mutex);
    if(data.empty())
        throw empty_exception();

    shared_ptr<T> const res(make_shared<T>(data.top()));
    data.pop();

    return res;
}

template<typename T=int>
void threadsafe_stack<T>::pop(T& value)
{
    boost::lock_guard<boost::mutex> lk(mutex);
    if(data.empty())
        throw empty_exception();
    value = data.top();
    data.pop();
}

template<typename T=int>
bool threadsafe_stack<T>::empty()
{
    boost::lock_guard<boost::mutex> lk(mutex);
    return data.empty();
}

template<typename T=int>
threadsafe_queue<T>::threadsafe_queue(threadsafe_queue const &other)
{
    boost::lock_guard<boost::mutex> lk(other.mutex);
    //mutex.lock();
    data = other.data;
    //mutex.unlock();
}

template<typename T=int>
threadsafe_queue<T>::~threadsafe_queue()
{
    boost::lock_guard<boost::mutex> lk(mutex);
    // while(!data.empty())
    //     wait_and_pop();
}

template<typename T=int>
void threadsafe_queue<T>::push(T new_value)
{
    boost::lock_guard<boost::mutex> lk(mutex);
    data.push(new_value);
}

template<typename T=int>
void threadsafe_queue<T>::wait_and_pop(T &value)
{
    boost::lock_guard<boost::mutex> lk(mutex);
    value = data.front();
    data.pop();
}

template<typename T=int>
shared_ptr<T> threadsafe_queue<T>::wait_and_pop()
{
    boost::lock_guard<boost::mutex> lk(mutex);
    shared_ptr<T> res(make_shared<T>(data.front()));
    data.pop();
    return res;
}


template<typename T=int>
bool threadsafe_queue<T>::empty()
{
    boost::lock_guard<boost::mutex> lk(mutex);
    return data.empty();
}

template<typename T=int>
int threadsafe_queue<T>::size()
{
    boost::lock_guard<boost::mutex> lk(mutex);
    return data.size();
}
