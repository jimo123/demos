#ifndef __LOG_H__
#define __LOG_H__

#include <iostream>
#include <iomanip>

#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>

#include <fstream>

using namespace std;

/**************************************/
// 向终端输出log
/**************************************/
template<typename T>
void print_log(T log)
{
    static boost::mutex mutex; 
    boost::unique_lock<boost::mutex> lk(mutex); // 在构造函数中对互斥体mutex加锁，在析构中解锁，相对于lock_guard可以使用mutex.unlock()手动解锁
    cout << log;
}

/*************************************/
// 向文件写log
/*************************************/
template<typename T>
void print_log(ofstream& file, T log)
{
    static boost::mutex mutex;
    boost::unique_lock<boost::mutex> lk(mutex); // 在构造函数中对互斥体mutex加锁，在析构中解锁，相对于lock_guard可以使用mutex.unlock()手动解锁
    file << log;
}


#endif