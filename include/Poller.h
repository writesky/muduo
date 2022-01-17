#pragma once
#include "NonCopyable.h"
#include "Channel.h"
#include <vector>
#include <unordered_map>

class EventLoop;

using ChannelList=vector<Channel *>;
using ChannelMap=unordered_map<int,Channel*>;
/*
 * io复用模块，muduo中多路事件发生器的核心
 */
class Poller:NonCopyable{
public:
    Poller(EventLoop *loop);

    virtual ~Poller()=default;

    //所有io复用保留统一接口
    virtual TimeStamp poll(int timeout,ChannelList *active_channels)=0;

    //判断channel是否在当前poller中
    bool has_channel(Channel *channel) const;

    //获取这个事件循环的poller
    static Poller *new_defaultPolller(EventLoop *loop);

    virtual void update_channel(Channel *channel)=0;
    virtual void remove_channel(Channel *channel)=0;

protected:
    //key: fd   value:fd所属的channel
    ChannelMap channels_;
private:
    EventLoop *owner_loop_;  //pollers所属的事件循环
};