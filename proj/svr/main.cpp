#include<iostream>
#include<boost/asio.hpp>
void print(const boost::system::error_code& /*e*/)
{
	std::cout << "HLW " << std::endl;
}
int main()
{
	boost::asio::io_context io;
	boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));
	//t.wait();
	t.async_wait(&print);
	//std::cout << "hello wordl" << std::endl;
	io.run();
	return 0;
}