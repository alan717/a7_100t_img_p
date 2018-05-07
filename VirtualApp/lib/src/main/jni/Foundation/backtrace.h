//
// Created by poi on 18-3-6.
//

#ifndef VIRTUALAPP_BACKTRACE_H
#define VIRTUALAPP_BACKTRACE_H
#include <iostream>
#include <iomanip>

#include <unwind.h>
#include <dlfcn.h>
#include <sstream>
#include <android/log.h>


void backtraceToLogcat();
#endif //VIRTUALAPP_BACKTRACE_H
