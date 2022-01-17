#include <semaphore.h>   //并发编程，信号量
#include "/Users/yankaixin/Desktop/muduo/include/Thread.h"
#include "/Users/yankaixin/Desktop/muduo/include/CurrentThread.h"

#define BUFFER_SIZE 32

atomic_int Thread::thread_nums_(0);

Thread::Thread(ThreadFunc func, const string &name):started_(false),
    joined_(false),tid_(false),function_(move(func)),name_(name){

}

Thread::~Thread() {
    if(started_ && !joined_){
        thread_->detach();
    }
}

//一个Thread对象记录一个thread的详细信息
void Thread::start() {
    started_=true;
    sem_t sem;  //信号量
    sem_init(&sem,false,0);

    //开启线程
    thread_= shared_ptr<thread>(new thread([&](){
        //获取线程tid值
        tid_=Current_thread::tid();
        sem_post(&sem);
        //执行函数
        function_();
    }));

    //需要等待新创建的线程，获取其线程id
    sem_wait(&sem);
}

void Thread::join() {
    joined_=true;
    //等待线程运行结束
    thread_->join();  //https://www.cnblogs.com/adorkable/p/12722209.html,简要概述，等待完成
}

void Thread::set_default_name() {
    int num=++thread_nums_;
    if(name_.empty()){
        char buffer[BUFFER_SIZE]={0};
        snprintf(buffer,sizeof (buffer),"Thread %d",num);
    }
}