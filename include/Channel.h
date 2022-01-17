#pragma once

#include <functional>
#include <memory>
#include "NonCopyable.h"
#include "TimeStamp.h"

class EventLoop;

using namespace std;

using EventCallback=function<void()>;
using ReadEventCallBack=function<void(TimeStamp)>;

/*
 *相当于sockfd及其感兴趣event的集合
 * 绑定了poller返回的事件
 */
class Channel:NonCopyable{
public:
    Channel(EventLoop *loop,int fd);
    ~Channel();

    //fd得到poller通知后，根据具体发生的事件，调用相应的回调
    void handle_event(TimeStamp receive_time);

    //防止channel被remove掉，channel还在执行回调
    //一个tcpconnection新链接创建的时候，调用tie
    void tie(const shared_ptr<void> &);

    //得到socket套接字
    int get_fd() const {return fd_;}

    //得到感兴趣的事件
    int get_events() const { return events_; }

    //设置真正发生的事件。poller监听到事件然后real_event
    int set_revent(int event) {
        real_events_ = event;
    }

    //判断该fd是否设置过感兴趣事件
    bool is_noneEvent() const {
        return events_==k_none_event_;
    }

    //返回所属eventloop
    EventLoop *owner_loop(){
        return loop_;
    }

    //在channel所属的eventloop中删除自己
    void remove();

    int index(){ return index_;}
    void set_index(int index) {
        index_=index;
    }

public:
    //设置fd感兴趣的事
    void enable_reading(){
        events_ |=k_read_event_;
        update();
    }

    void dis_enable_reading(){
        events_ &= ~k_read_event_;
        update();
    }

    void enable_writing(){
        events_ |= k_write_event_;
        update();
    }

    void dis_enable_writing(){
        events_ &= ~k_write_event_;
        update();
    }

    void dis_enable_all(){
        events_ = k_none_event_;
        update();
    }

public:
    //返回fd当前感兴趣的事
    bool is_none__event() const {return events_==k_none_event_;}
    bool is_writing() const {return events_ & k_write_event_;}
    bool is_reading() const {return events_ & k_read_event_;}

public:
    //设置不同事件的回调操作
    void set_readcallback(ReadEventCallBack callBack){read_callback_=move(callBack);}//通过std::move，可以避免不必要的拷贝操作
    void set_writecallback(EventCallback callback) {write_callback_=move(callback);}
    void set_closecallback(EventCallback callback){close_callback_=move(callback);}
    void set_errorcallback(EventCallback callback){error_acllback_=move(callback);}
private:
    //与poller更新fd所感兴趣事件
    void update();

    //根据发生的具体事件调用相应的回调操作
    void handle_event_withGuard(TimeStamp receive_time);
private:
    static const int k_none_event_;
    static const int k_read_event_;
    static const int k_write_event_;
private:
    EventLoop *loop_;
    const int fd_;
    int events_;    //fd感兴趣的事
    int real_events_; //poller 具体发生的事
    int index_;

    weak_ptr<void> tie_;  //观察当前channel的存在状态
    bool tied_;           //判定tie_是否绑定

    //发生事件所要调用的具体事件的回调操作
    ReadEventCallBack read_callback_;
    EventCallback write_callback_;
    EventCallback close_callback_;
    EventCallback error_acllback_;
};