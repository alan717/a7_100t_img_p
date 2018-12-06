//
// timer.cpp
// ~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <array>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>
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
#include <boost/date_time/posix_time/posix_time.hpp>
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

DWORD WINAPI prints(LPVOID p)
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




class handler
{
public:
	handler(boost::asio::io_service& io)
		: m_strand(io),
		m_timer1(io, boost::posix_time::seconds(1)),
		m_timer2(io, boost::posix_time::seconds(1)),
		m_count(0)
	{
		m_timer1.async_wait(m_strand.wrap(boost::bind(&handler::message1, this)));
		m_timer2.async_wait(m_strand.wrap(boost::bind(&handler::message2, this)));
	}

	~handler()
	{
		std::cout << "The last count : " << m_count << "\n";
	}

	void message1()
	{
		if (m_count < 10)
		{
			std::cout << "Timer 1: " << m_count << "\n";
			++m_count;

			m_timer1.expires_at(m_timer1.expires_at() + boost::posix_time::seconds(1));
			m_timer1.async_wait(m_strand.wrap(boost::bind(&handler::message1, this)));
		}
	}

	void message2()
	{
		if (m_count < 10)
		{
			std::cout << "Timer 2: " << m_count << "\n";
			++m_count;

			m_timer2.expires_at(m_timer2.expires_at() + boost::posix_time::seconds(1));
			m_timer2.async_wait(m_strand.wrap(boost::bind(&handler::message2, this)));
		}
	}

private:
	boost::asio::io_service::strand		m_strand;
	boost::asio::deadline_timer			m_timer1;
	boost::asio::deadline_timer			m_timer2;
	int m_count;
};

//int main()
//{

//}
 

boost::mutex global_stream_lock;

void WorkerThread(boost::shared_ptr<boost::asio::io_service> iosvc, int counter)
{
	global_stream_lock.lock();
	std::cout << "Thread " << counter << " Start.\n";
	global_stream_lock.unlock();

	iosvc->run();

	global_stream_lock.lock();
	std::cout << "Thread " << counter << " End.\n";
	global_stream_lock.unlock();
}

void Print(int number)
{
	std::cout << "Number: " << number << std::endl;
}


using boost::asio::ip::tcp;

// Class to manage the memory to be used for handler-based custom allocation.
// It contains a single block of memory which may be returned for allocation
// requests. If the memory is in use when an allocation request is made, the
// allocator delegates allocation to the global heap.
class handler_memory
{
public:
	handler_memory()
		: in_use_(false)
	{
	}

	handler_memory(const handler_memory&) = delete;
	handler_memory& operator=(const handler_memory&) = delete;

	void* allocate(std::size_t size)
	{
		if (!in_use_ && size < sizeof(storage_))
		{
			in_use_ = true;
			return &storage_;
		}
		else
		{
			return ::operator new(size);
		}
	}

	void deallocate(void* pointer)
	{
		if (pointer == &storage_)
		{
			in_use_ = false;
		}
		else
		{
			::operator delete(pointer);
		}
	}

private:
	// Storage space used for handler-based custom memory allocation.
	typename std::aligned_storage<1024>::type storage_;

	// Whether the handler-based custom allocation storage has been used.
	bool in_use_;
};

// The allocator to be associated with the handler objects. This allocator only
// needs to satisfy the C++11 minimal allocator requirements.
template <typename T>
class handler_allocator
{
public:
	using value_type = T;

	explicit handler_allocator(handler_memory& mem)
		: memory_(mem)
	{
	}

	template <typename U>
	handler_allocator(const handler_allocator<U>& other) noexcept
		: memory_(other.memory_)
	{
	}

	bool operator==(const handler_allocator& other) const noexcept
	{
		return &memory_ == &other.memory_;
	}

	bool operator!=(const handler_allocator& other) const noexcept
	{
		return &memory_ != &other.memory_;
	}

	T* allocate(std::size_t n) const
	{
		return static_cast<T*>(memory_.allocate(sizeof(T) * n));
	}

	void deallocate(T* p, std::size_t /*n*/) const
	{
		return memory_.deallocate(p);
	}

private:
	template <typename> friend class handler_allocator;

	// The underlying memory.
	handler_memory& memory_;
};

// Wrapper class template for handler objects to allow handler memory
// allocation to be customised. The allocator_type type and get_allocator()
// member function are used by the asynchronous operations to obtain the
// allocator. Calls to operator() are forwarded to the encapsulated handler.
template <typename Handler>
class custom_alloc_handler
{
public:
	using allocator_type = handler_allocator<Handler>;

	custom_alloc_handler(handler_memory& m, Handler h)
		: memory_(m),
		handler_(h)
	{
	}

	allocator_type get_allocator() const noexcept
	{
		return allocator_type(memory_);
	}

	template <typename ...Args>
	void operator()(Args&&... args)
	{
		handler_(std::forward<Args>(args)...);
	}

private:
	handler_memory& memory_;
	Handler handler_;
};

// Helper function to wrap a handler object to add custom allocation.
template <typename Handler>
inline custom_alloc_handler<Handler> make_custom_alloc_handler(
	handler_memory& m, Handler h)
{
	return custom_alloc_handler<Handler>(m, h);
}

class session
	: public std::enable_shared_from_this<session>
{
public:
	session(tcp::socket socket)
		: socket_(std::move(socket))
	{
	}

	void start()
	{
		do_read();
	}

private:
	void do_read()
	{
		auto self(shared_from_this());
		socket_.async_read_some(boost::asio::buffer(data_),
			make_custom_alloc_handler(handler_memory_,
				[this, self](boost::system::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				do_write(length);
			}
		}));
	}

	void do_write(std::size_t length)
	{
		auto self(shared_from_this());
		boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
			make_custom_alloc_handler(handler_memory_,
				[this, self](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				do_read();
			}
		}));
	}

	// The socket used to communicate with the client.
	tcp::socket socket_;

	// Buffer used to store data received from the client.
	std::array<char, 1024> data_;

	// The memory to use for handler-based custom memory allocation.
	handler_memory handler_memory_;
};

class server
{
public:
	server(boost::asio::io_context& io_context, short port)
		: acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
	{
		do_accept();
	}

private:
	void do_accept()
	{
		acceptor_.async_accept(
			[this](boost::system::error_code ec, tcp::socket socket)
		{
			if (!ec)
			{
				std::make_shared<session>(std::move(socket))->start();
			}

			do_accept();
		});
	}

	tcp::acceptor acceptor_;
};

 
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
	//int i = 1;
	//process_value(i);
	//process_value(std::move(3));
	//boost::asio::io_service io;
	//handler h(io);
	//boost::thread t(boost::bind(&boost::asio::io_service::run, &io));
	//io.run();
	//t.join();

	//boost::shared_ptr<boost::asio::io_service> io_svc(
	//	new boost::asio::io_service
	//);

	//boost::shared_ptr<boost::asio::io_service::work> worker(
	//	new boost::asio::io_service::work(*io_svc)
	//);

	//boost::asio::io_service::strand strand(*io_svc);

	//global_stream_lock.lock();
	//std::cout << "The program will exit once all work has finished.\n";
	//global_stream_lock.unlock();

	//boost::thread_group threads;
	//for (int i = 1; i <= 5; i++)
	//	threads.create_thread(boost::bind(&WorkerThread, io_svc, i));

	//boost::this_thread::sleep(boost::posix_time::milliseconds(500));

	//strand.post(boost::bind(&Print, 1));
	//strand.post(boost::bind(&Print, 2));
	//strand.post(boost::bind(&Print, 3));
	//strand.post(boost::bind(&Print, 4));
	//strand.post(boost::bind(&Print, 5));

	//worker.reset();

	//threads.join_all();
	//try
	//{
	//	if (argc != 2)
	//	{
	//		std::cerr << "Usage: server <port>\n";
	//		return 1;
	//	}
	//
	//	boost::asio::io_context io_context;
	//	server s(io_context, std::atoi(argv[1]));
	//	io_context.run();
	//}
	//catch (std::exception& e)
	//{
	//	std::cerr << "Exception: " << e.what() << "\n";
	//}
	boost::asio::io_context io_ctx;
	boost::asio::steady_timer M_timer(io_ctx,boost::asio::chrono::seconds(5));
	M_timer.async_wait(&print);
	io_ctx.run();
	std::cout << "Think easy£¡" << std::endl;
	return 0;
 
}