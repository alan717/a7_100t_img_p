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

int main(int argc,char* argv[])
{
	boost::asio::io_context io;
	printer p(io);
	//boost::thread t(boost::bind(&boost::asio::io_context::run, &io));
	io.run();
	//t.join();
	boost::scoped_ptr<int> i(new int );
	*i = 1;
	*i.get() = 2;
	i.reset(new int);

	boost::asio::io_context ioo;
	boost::asio::steady_timer ti(io, boost::asio::chrono::seconds(5));
	ti.async_wait(&print);
	io.run();




	try
	{
		if (argc != 2)
		{
			std::cerr << "Usage: client <host>" << std::endl;
			return 1;
		}

		boost::asio::io_context io_context;

		tcp::resolver resolver(io_context);
		tcp::resolver::results_type endpoints =
			resolver.resolve(argv[1], "daytime");

		tcp::socket socket(io_context);
		boost::asio::connect(socket, endpoints);

		for (;;)
		{
			boost::array<char, 128> buf;
			boost::system::error_code error;

			size_t len = socket.read_some(boost::asio::buffer(buf), error);

			if (error == boost::asio::error::eof)
				break; // Connection closed cleanly by peer.
			else if (error)
				throw boost::system::system_error(error); // Some other error.

			std::cout.write(buf.data(), len);
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}