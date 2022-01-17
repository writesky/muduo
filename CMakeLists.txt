cmake_minimum_required(VERSION 3.19)
project(muduo)

set(CMAKE_CXX_STANDARD 11)

add_executable(muduo main.cpp include/NonCopyable.h include/TimeStamp.h include/CurrentThread.h include/Channel.h src/TimeStamp.cpp src/CurrentThread.cpp include/Logger.h src/Logger.cpp src/Channel.cpp include/Poller.h src/Poller.cpp include/EventLoop.h src/EventLoop.cpp include/Thread.h src/Thread.cpp include/EventLoopThread.h src/EventLoopThread.cpp include/EventLoopThreadPool.h src/EventLoopThreadPool.cpp include/Socket.h src/Socket.cpp include/InetAddress.h src/InetAddress.cpp include/Buffer.h src/Buffer.cpp include/Acceptor.h src/Acceptor.cpp)