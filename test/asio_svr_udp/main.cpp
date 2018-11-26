//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
//using boost::asio::ip::tcp;
using boost::asio::ip::udp;
std::string make_daytime_string() {
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}
//class tcp_connection : public boost::enable_shared_from_this<tcp_connection> {
//public:
//  typedef boost::shared_ptr<tcp_connection> pointer;
//  static pointer create(boost::asio::io_service& io_service) {
//    return pointer(new tcp_connection(io_service));
//  }
//  tcp::socket& socket() {
//    return socket_;
//  }
//  void start() {
//    message_ = make_daytime_string();
//    boost::asio::async_write(socket_, boost::asio::buffer(message_),
//        boost::bind(&tcp_connection::handle_write, shared_from_this(),
//          boost::asio::placeholders::error,
//          boost::asio::placeholders::bytes_transferred));
//  }
//private:
//  tcp_connection(boost::asio::io_service& io_service) : socket_(io_service) {
//  }
//  void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/) {
//  }
//  tcp::socket socket_;
//  std::string message_;
//};
//class tcp_server {
//public:
//  tcp_server(boost::asio::io_service& io_service) : acceptor_(io_service, tcp::endpoint(tcp::v4(), 13)) {
//    start_accept();
//  }
//private:
//  void start_accept() {
//    tcp_connection::pointer new_connection =
//      tcp_connection::create(acceptor_.get_io_service());
//    acceptor_.async_accept(new_connection->socket(),
//        boost::bind(&tcp_server::handle_accept, this, new_connection,
//          boost::asio::placeholders::error));
//  }
//  void handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code& error) {
//    if (!error) {
//      new_connection->start();
//    }
//    start_accept();
//  }
//  tcp::acceptor acceptor_;
//};


int main() {
  //try {
  //  boost::asio::io_service io_service;
  //  tcp_server server(io_service);
  //  io_service.run();
  //} catch (std::exception& e) {
  //  std::cerr << e.what() << std::endl;
  //}
  //try
  //{
	 // boost::asio::io_context io_context;

	 // tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 13));

	 // for (;;)
	 // {
		//  tcp::socket socket(io_context);
		//  acceptor.accept(socket);

		//  std::string message = make_daytime_string();

		//  boost::system::error_code ignored_error;
		//  boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
	 // }
  //}
  //catch (std::exception& e)
  //{
	 // std::cerr << e.what() << std::endl;
  //}

	//try
	//{
	//	boost::asio::io_context io_context;
	//	tcp_server server(io_context);
	//	io_context.run();
	//}
	//catch (std::exception& e)
	//{
	//	std::cerr << e.what() << std::endl;
	//}
	try
	{
		boost::asio::io_context io_context;

		udp::socket socket(io_context, udp::endpoint(udp::v4(), 13));

		for (;;)
		{
			boost::array<char, 1> recv_buf;
			udp::endpoint remote_endpoint;
			boost::system::error_code error;
			socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);

			std::string message = make_daytime_string();

			boost::system::error_code ignored_error;
			socket.send_to(boost::asio::buffer(message),
				remote_endpoint, 0, ignored_error);
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
  return 0;
}