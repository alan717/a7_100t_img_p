#include<iostream>
#include<boost/asio.hpp>
#include<boost/bind.hpp>

void print(const boost::system::error_code& e,
	boost::asio::steady_timer* t,
	int* count)
{

	if (*count < 5)
	{
		std::cout << *count << std::endl;
		++(*count);

		t->expires_at(t->expiry() + boost::asio::chrono::seconds(1));
		t->async_wait(boost::bind(print,
			boost::asio::placeholders::error, t, count));
	}
}
int main()
{
	boost::asio::io_context io;
	boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));
	//t.wait();
	int count = 0;
	t.async_wait(boost::bind(print, boost::asio::placeholders::error, &t, &count));
	
	//std::cout << "hello wordl" << std::endl;
	io.run();
	return 0;
}