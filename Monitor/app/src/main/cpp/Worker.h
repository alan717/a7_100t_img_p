//
// Created by user567 on 2018/6/10.
//

#ifndef MONITOR_WORKER_H
#define MONITOR_WORKER_H

#include <thread>

class Worker {

    void process();
    void create();

private:
    std::thread * m_thrad;

};

#endif //MONITOR_WORKER_H
