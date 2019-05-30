#include <iostream>
#include <iomanip>
#include <thread>
#include <fstream>

#include <boost/thread/lock_guard.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

#include <log.h>
#include <threadsafe_queue.h>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

#define THREAD_NUM 2

struct LogInfo
{
    int threadID;
    int num;

    friend ostream& operator << (ostream &out, LogInfo &log)
    {
        cout.flags(ios::left);
        out << setw(10) << "threadID: " << setw(3) << log.threadID << setw(4) << "num: " << setw(6) << log.num << endl;
        return out;
    }
};


class threadC
{
private:
    boost::thread *producer_thread[THREAD_NUM]; //生产者线程
    boost::thread *consumer_thread[THREAD_NUM]; //消费者线程
    boost::mutex mutex;  //互斥量

    threadsafe_queue<LogInfo> data[THREAD_NUM]; //线程安全队

    ofstream log_file;  //log文件

public:
    threadC()
    {
        log_file.open("logfile.txt");
        for (int i = 0; i < THREAD_NUM;i++)
        {
            producer_thread[i] = nullptr;
            consumer_thread[i] = nullptr;
        }
    }
    ~threadC() 
    {
        log_file.close();
        for (int i = 0; i < THREAD_NUM;i++)
        {
            delete producer_thread[i];
            delete consumer_thread[i];
        }
    }

    void producer(int threadID)
    {
        LogInfo log_info;
        int num = 0;
        while(true)
        {
            log_info.threadID = threadID;
            log_info.num = num;
            data[threadID].push(log_info);

            num++;
            Sleep(100);
        }
    }

    void consumer(int threadID)
    {
        LogInfo log_info;
        while(true)
        {
            data[threadID].wait_and_pop(log_info);
            print_log(log_info);
            print_log(log_file, log_info);

            Sleep(1000);
        }
    }

    void begin_producer(int id)
    {
        producer_thread[id] = new boost::thread(boost::bind(&threadC::producer, this, id));
        producer_thread[id]->detach();  //detach：非阻塞方式，join：阻塞方式
    }

    void begin_consumer(int id)
    {
        consumer_thread[id] = new boost::thread(boost::bind(&threadC::consumer, this, id));
        consumer_thread[id]->detach();
    }


};


int main(int argc, char **argv)
{
    threadC thread_c;
    thread_c.begin_producer(0);  //开始生成者线程
    thread_c.begin_producer(1);
    thread_c.begin_consumer(0);  //开始消费者线程
    thread_c.begin_consumer(1);

    //阻塞主线程
    while(1)
        Sleep(100000);

    return 0;
}
