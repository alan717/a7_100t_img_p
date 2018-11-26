//
// timer.cpp
// ~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include<boost/config.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/weak_ptr.hpp> 
#include <atlbase.h> 

class printer
{
public:
	printer(boost::asio::io_context& io)
		: strand_(io),
		timer1_(io, boost::asio::chrono::seconds(1)),
		timer2_(io, boost::asio::chrono::seconds(1)),
		count_(0)
	{
		timer1_.async_wait(boost::asio::bind_executor(strand_,
			boost::bind(&printer::print1, this)));

		timer2_.async_wait(boost::asio::bind_executor(strand_,
			boost::bind(&printer::print2, this)));
	}

	~printer()
	{
		std::cout << "Final count is " << count_ << std::endl;
	}

	void print1()
	{
		if (count_ < 10)
		{
			std::cout << "Timer 1: " << count_ << std::endl;
			++count_;

			timer1_.expires_at(timer1_.expiry() + boost::asio::chrono::seconds(1));

			timer1_.async_wait(boost::asio::bind_executor(strand_,
				boost::bind(&printer::print1, this)));
		}
	}

	void print2()
	{
		if (count_ < 10)
		{
			std::cout << "Timer 2: " << count_ << std::endl;
			++count_;

			timer2_.expires_at(timer2_.expiry() + boost::asio::chrono::seconds(1));

			timer2_.async_wait(boost::asio::bind_executor(strand_,
				boost::bind(&printer::print2, this)));
		}
	}

private:
	boost::asio::io_context::strand strand_;
	boost::asio::steady_timer timer1_;
	boost::asio::steady_timer timer2_;
	int count_;
};
void print(const boost::system::error_code& /*e*/)
{
	std::cout << "Hello, world!" << std::endl;
}

using boost::asio::ip::tcp;
void test()
{
	// test scoped_ptr with a built-in type
	long * lp = new long;
	boost::scoped_ptr<long> sp(lp);
	BOOST_TEST(sp.get() == lp);
	BOOST_TEST(lp == sp.get());
	BOOST_TEST(&*sp == lp);

	*sp = 1234568901L;
	BOOST_TEST(*sp == 1234568901L);
	BOOST_TEST(*lp == 1234568901L);

	long * lp2 = new long;
	boost::scoped_ptr<long> sp2(lp2);

	sp.swap(sp2);
	BOOST_TEST(sp.get() == lp2);
	BOOST_TEST(sp2.get() == lp);

	sp.reset(NULL);
	BOOST_TEST(sp.get() == NULL);

}
DWORD WINAPI reset(LPVOID p)
{
	boost::shared_ptr<int> *sh = static_cast<boost::shared_ptr<int>*>(p);
	sh->reset();
	return 0;
}

DWORD WINAPI print(LPVOID p)
{
	boost::weak_ptr<int> *w = static_cast<boost::weak_ptr<int>*>(p);
	boost::shared_ptr<int> sh = w->lock();
	if (sh)
		std::cout << *sh << std::endl;
	return 0;
}
void intrusive_ptr_add_ref(IDispatch *p)
{
	p->AddRef();
}

void intrusive_ptr_release(IDispatch *p)
{
	p->Release();
}

void check_windows_folder()
{
	CLSID clsid;
	CLSIDFromProgID(CComBSTR("Scripting.FileSystemObject"), &clsid);
	void *p;
	CoCreateInstance(clsid, 0, CLSCTX_INPROC_SERVER, __uuidof(IDispatch), &p);
	boost::intrusive_ptr<IDispatch> disp(static_cast<IDispatch*>(p));
	CComDispatchDriver dd(disp.get());
	CComVariant arg("C:\\Windows");
	CComVariant ret(false);
	dd.Invoke1(CComBSTR("FolderExists"), &arg, &ret);
	std::cout << (ret.boolVal != 0) << std::endl;
}
class addd : public std::binary_function<int, int, void>
{
public:
	void operator()(int i, int j) const
	{
		std::cout << i + j << std::endl;
	}
};
void add(int i, int j)
{
	std::cout << i + j << std::endl;
}
void process_value(int& i)
{
	std::cout << "left value:"<<i<< '\n';
}
void process_value(int&& i)
{
	std::cout <<"right value:" <<i << '\n';
}
//#define BOOST_SP_USE_QUICK_ALLOCATOR
int main(int argc,char* argv[])
{
	//std::vector<int > v;
	//v.push_back(1);
	//v.push_back(2);
	//v.push_back(3);
	//std::for_each(v.begin(),v.end(),std::bind1st(add(),10));
	//std::for_each(v.begin(), v.end(), std::bind2nd(add(), 20));
	//std::for_each(v.begin(), v.end(), std::bind(add, 10, std::placeholders::_1));
	//boost::asio::io_context io;
	//printer p(io);
	//boost::thread t(boost::bind(&boost::asio::io_context::run, &io));
	//io.run();
	////t.join();
	//boost::scoped_ptr<int> i(new int );
	//*i = 1;
	//*i.get() = 2;
	//i.reset(new int);

	//boost::asio::io_context ioo;
	//boost::asio::steady_timer ti(io, boost::asio::chrono::seconds(5));
	//ti.async_wait(&print);
	//io.run();




	//try
	//{
	//	if (argc != 2)
	//	{
	//		std::cerr << "Usage: client <host>" << std::endl;
	//		return 1;
	//	}

	//	boost::asio::io_context io_context;

	//	tcp::resolver resolver(io_context);
	//	tcp::resolver::results_type endpoints =
	//		resolver.resolve(argv[1], "daytime");

	//	tcp::socket socket(io_context);
	//	boost::asio::connect(socket, endpoints);

	//	for (;;)
	//	{
	//		boost::array<char, 128> buf;
	//		boost::system::error_code error;

	//		size_t len = socket.read_some(boost::asio::buffer(buf), error);

	//		if (error == boost::asio::error::eof)
	//			break; // Connection closed cleanly by peer.
	//		else if (error)
	//			throw boost::system::system_error(error); // Some other error.

	//		std::cout.write(buf.data(), len);
	//	}
	//}
	//catch (std::exception& e)
	//{
	//	std::cerr << e.what() << std::endl;
	//}
	//std::vector<boost::shared_ptr<int>> v;
	//v.push_back(boost::shared_ptr<int>(new int(1)));
	//v.push_back(boost::shared_ptr<int>(new int(2)));
	//test();
	//boost::shared_ptr<void> h(OpenProcess(PROCESS_SET_INFORMATION, FALSE, GetCurrentProcessId()), CloseHandle);
	//SetPriorityClass(h.get(), HIGH_PRIORITY_CLASS);
	//boost::shared_ptr<int> sh(new int(99));
	//boost::weak_ptr<int> w(sh);
	//HANDLE threads[2];
	//threads[0] = CreateThread(0, 0, reset, &sh, 0, 0);
	//threads[1] = CreateThread(0, 0, print, &w, 0, 0);
	//WaitForMultipleObjects(2, threads, TRUE, INFINITE);
	//CoInitialize(0);
	//check_windows_folder();
	//CoUninitialize();
	//boost::shared_ptr<int> p1{ new int{1} };
	//std::cout << *p1 << '\n';
	//boost::shared_ptr<int> p2{ p1 };
	//p1.reset(new int{2} );
	//std::cout << *p1.get() << '\n';
	//p1.reset();
	//std::cout << std::boolalpha << static_cast<bool>(p2) << '\n';
	//boost::shared_ptr<int> p;
	//std::time_t then = std::time(nullptr);
	//for (int i = 0; i < 1000000; ++i)
	//	p.reset(new int{ i });
	//std::time_t now = std::time(nullptr);
	//std::cout << now - then << '\n';
	int i = 1;
	process_value(i);
	process_value(std::move(3));
	return 0;
}