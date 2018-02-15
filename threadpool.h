#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <auto_ptr.h>

class ThreadPool;

// our worker thread objects
class Worker {
public:
    Worker(ThreadPool &s) : pool(s) { }
    void operator()();
private:
    ThreadPool &pool;
};

class ThreadPool
{
public:
    ThreadPool(size_t);
    template<class F>
    void enqueue(F f);
    void wait(int i);
    void clear();
    ~ThreadPool();
private:
    // need to keep track of threads so we can join them
    std::vector< boost::shared_ptr<boost::thread> > workers;

    // the io_service we are wrapping
    boost::asio::io_service service;
    boost::shared_ptr<boost::asio::io_service::work> work;
    friend class Worker;
};
// all the workers do is execute the io_service
void Worker::operator()() { pool.service.run(); }

// the constructor just launches some amount of workers
ThreadPool::ThreadPool(size_t threads): work(new boost::asio::io_service::work(service))
{
    for(size_t i = 0;i<threads;++i)
        workers.push_back(
            boost::shared_ptr<boost::thread>(
                new boost::thread(Worker(*this))
            )
        );
}

// add new work item to the pool
template<class F>
void ThreadPool::enqueue(F f)
{
    service.post(f);
}

void ThreadPool::wait(int i)
{
    work.reset();
    for(size_t ii = 0; ii < workers.size(); ++ii)
        workers[ii]->join();
    work.reset(new boost::asio::io_service::work(service));
}

void ThreadPool::clear()
{
    service.stop();
}

// the destructor joins all threads
ThreadPool::~ThreadPool()
{
    // std::cout<<"in ThreadPool destructor"<<std::endl;
    service.stop();
    for(size_t i = 0;i<workers.size();++i){
        workers[i]->join();
        workers[i].reset();
    }
    workers.clear();
    // std::cout<<"exit from ThreadPool destructor"<<std::endl;
}

#endif // THREADPOOL_H
