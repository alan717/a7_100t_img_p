//
// Created by user567 on 2018/6/10.
//

#include "Worker.h"
//#include <thread>
void Worker::process() {
    std::this_thread::sleep_for(250ms);
    //m_thrad=new thread();
}

void Worker::create() {
    m_thrad=new std::thread(Worker::process,this);
    std::this_thread::get_id();

}


