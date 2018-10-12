#include "mns_exception.h"

#ifdef _WIN32
#include <windows.h>
#include <DbgHelp.h>
#else
#include <execinfo.h>
#include <cxxabi.h>
#endif
#include <stdlib.h>
#include <iostream>
#include <sstream>

using namespace std;
using namespace mns::sdk;

MNSExceptionBase::MNSExceptionBase(const std::string& msg) throw()
    : mMsg(msg)
    , mFile("<unknown file>")
    , mFunc("<unknown func>")
    , mLine(-1)
    , mStackTraceSize(0)
{}

MNSExceptionBase::~MNSExceptionBase() throw()
{}

void MNSExceptionBase::Init(const char* file, const char* func, int line)
{
    mFile = file;
    mFunc = func;
    mLine = line;
#ifdef _WIN32
	mStackTraceSize = CaptureStackBackTrace(0, MAX_STACK_TRACE_SIZE, mStackTrace, NULL);
#else
    mStackTraceSize = backtrace(mStackTrace, MAX_STACK_TRACE_SIZE);
#endif
}

std::string MNSExceptionBase::GetExceptionClassName() const
{
    return "MNSExceptionBase";
}

const char* MNSExceptionBase::what() const throw()
{
    return ToString().c_str();
}

const std::string& MNSExceptionBase::ToString() const
{
    if (mWhat.empty())
    {
        stringstream sstr("");
        if (mLine > 0)
        {
            sstr << mFile << "(" << mLine << ")";
        }
        sstr << ": " << GetExceptionClassName();
        if (!GetExceptionMessage().empty())
        {
            sstr << ": " << GetExceptionMessage();
        }
        sstr << "\nStack Trace:\n";
        sstr << GetStackTrace();
        mWhat = sstr.str();
    }
    return mWhat;
}

std::string MNSExceptionBase::GetExceptionMessage() const
{
    return mMsg;
}

#ifndef _WIN32
std::string MNSExceptionBase::GetClassName() const
{
	return GetExceptionClassName();
}

std::string MNSExceptionBase::GetMessage() const
{
	return GetExceptionMessage();
}
#endif

std::string MNSExceptionBase::GetStackTrace() const
{
    if (mStackTraceSize == 0)
        return "<No stack trace>\n";

#ifdef _WIN32
	void * ptr = calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
	if (ptr == NULL)
		return "<Unknown error: backtrace_symbols returned NULL>\n";

	SYMBOL_INFO* symbol = (SYMBOL_INFO*)ptr;
	symbol->MaxNameLen = 255;
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
	HANDLE process = GetCurrentProcess();

	std::stringstream ss;
	for (int i = 0; i < mStackTraceSize; i++)
	{
		SymFromAddr(process, (DWORD64)mStackTrace[i], 0, symbol);
		ss << symbol->Name << " - " << symbol->Address << "\n";
	}
    free(symbol);

	return ss.str();

#else
    char** strings = backtrace_symbols(mStackTrace, mStackTraceSize);
    if (strings == NULL)
        return "<Unknown error: backtrace_symbols returned NULL>\n";

    std::string result;
    for (size_t i = 0; i < mStackTraceSize; ++i)
    {
        std::string mangledName = strings[i];
        std::string::size_type begin = mangledName.find('(');
        std::string::size_type end = mangledName.find('+', begin);
        if (begin == std::string::npos || end == std::string::npos)
        {
            result += mangledName;
            result += '\n';
            continue;
        }
        ++begin;
        int status;
        char* s = abi ::__cxa_demangle(mangledName.substr(begin, end-begin).c_str(), NULL, 0, &status);

        if (status != 0)
        {
            result += mangledName;
            result += '\n';
            continue;
        }

        std::string demangledName(s);
        free(s);

        result += mangledName.substr(0, begin);
        result += demangledName;
        result += mangledName.substr(end);
        result += '\n';
    }
    free(strings);
    return result;
#endif
}
