#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>
#include <boost/bind.hpp>
#include <iostream>
 
using boost::asio::deadline_timer;
using boost::asio::ip::tcp;
 
 
class client
{
public:
	// 在构造函数的初始化列表中 初始化所有变量
	client(boost::asio::io_service& io_service)
		: stopped_(false),
		socket_(io_service),
		deadline_(io_service),
		heartbeat_timer_(io_service)
	{
	}
 
	// 对外公开并使用的唯一接口
	// 
	void start(tcp::resolver::iterator endpoint_iter)
	{
		// 连接server
		start_connect(endpoint_iter);
 
		// 设定连接超时间，并指定回调函数
		deadline_.async_wait(boost::bind(&client::check_deadline, this));
	}
 
private:
	void stop()
	{
		stopped_ = true;
		boost::system::error_code ignored_ec;
		socket_.close(ignored_ec);// 关闭socket
		deadline_.cancel();// 取消定时器
		heartbeat_timer_.cancel();
	}
 
	void start_connect(tcp::resolver::iterator endpoint_iter)
	{
		if (endpoint_iter != tcp::resolver::iterator())
		{
			std::cout << "Trying " << endpoint_iter->endpoint() << "...\n";
 
			// 设定定时器超时时间
			deadline_.expires_from_now(boost::posix_time::seconds(60));
 
			// 设置连接成功回调函数
			socket_.async_connect(endpoint_iter->endpoint(),
				boost::bind(&client::handle_connect,
				this, _1, endpoint_iter));
		}
		else
		{
			stop();
		}
	}
 
	void handle_connect(const boost::system::error_code& ec,
		tcp::resolver::iterator endpoint_iter)
	{
		if (stopped_)
			return;
 
		if (!socket_.is_open())
		{
			std::cout << "Connect timed out\n";
 
			start_connect(++endpoint_iter);
		}
		else if (ec)
		{// 其他方面的错误 如服务器没有打开
			std::cout << "Connect error: " << ec.message() << "\n";
 
			socket_.close();
 
			start_connect(++endpoint_iter);
		}
		else
		{// 连接成功
			std::cout << "Connected to " << endpoint_iter->endpoint() << "\n";
 
			start_read();
 
			start_write();// 开始心跳
		}
	}
 
	void start_read()
	{
		// 设置recv 超时时间，此处会触发 check_deadline 但不是真正的超时
		deadline_.expires_from_now(boost::posix_time::seconds(30));
 
		// 异步接收以"\n"为结束符/  意为从网络缓存中读取一行
		boost::asio::async_read_until(socket_, input_buffer_, '\n',
			boost::bind(&client::handle_read, this, _1));
	}
 
	void handle_read(const boost::system::error_code& ec)
	{
		if (stopped_)
			return;
 
		if (!ec)
		{
			std::string line;
			std::istream is(&input_buffer_);
			std::getline(is, line);// 从流中获取一行内容  舍去回车
 
			if (!line.empty())// 如果line.empty == true 是表示是一个心跳包，心跳包只发送一个回车\n
			{
				std::cout << "Received: " << line << "\n";
			}
 
			// 开始新一次读取
			start_read();
		}
		else
		{// 读取发生错误/ 远端已关闭socket
			std::cout << "Error on receive: " << ec.message() << "\n";
 
			stop();
		}
	}
 
 
	//////////////////////////////////////////////////////////////////////////
	// 定时器完成心跳
	void start_write()
	{
		if (stopped_)
			return;
 
		// Start an asynchronous operation to send a heartbeat message.
		boost::asio::async_write(socket_, boost::asio::buffer("\n", 1),
			boost::bind(&client::handle_write, this, _1));
	}
 
	void handle_write(const boost::system::error_code& ec)
	{
		if (stopped_)
			return;
 
		if (!ec)
		{
			// 设定超时时间
			heartbeat_timer_.expires_from_now(boost::posix_time::seconds(10));
			
			// 开始异步等待，并设定超时回调函数
			heartbeat_timer_.async_wait(boost::bind(&client::start_write, this));
		}
		else
		{
			std::cout << "Error on heartbeat: " << ec.message() << "\n";
 
			stop();
		}
	}
	//////////////////////////////////////////////////////////////////////////
 
	void check_deadline()
	{
		if (stopped_)// check 是存已标识并等待程序退出
			return;
 
		if (deadline_.expires_at() <= deadline_timer::traits_type::now())
		{// 真正的等待超时
 
			socket_.close();// 关闭对应的socket 包括连接超时/recv超时
 
			deadline_.expires_at(boost::posix_time::pos_infin);// 定时器 设定为永不超时/不可用状态
		}
 
		// 如果不是真正的超时，定是其操作已成功/用户重新设置了定时器
		// 重新启动定时器
		deadline_.async_wait(boost::bind(&client::check_deadline, this));
	}
 
private:
	bool stopped_;								// 标识程序是否停止运行
	tcp::socket socket_;						// 
	boost::asio::streambuf input_buffer_;		// recv buf
	deadline_timer deadline_;					// 连接/recv 超时 定时器
	deadline_timer heartbeat_timer_;			// 心跳超时 计时器
};
 
int main(int argc, char* argv[])
{
	try
	{
 
		boost::asio::io_service io_service;
		tcp::resolver r(io_service);
		client c(io_service);
 
		c.start(r.resolve(tcp::resolver::query("127.0.0.1", "1082")));
 
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
 
	return 0;
}
