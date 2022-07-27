//
// Created by Дима Шестаков on 27.07.2022.
//

#include "ChatNetworking/client/tcp_client.h"

namespace chat
{

	TCPClient::TCPClient(const std::string& address, int port)
		: socket_(io_context_)
	{
		io::ip::tcp::resolver resolver{ io_context_ };
		endpoints_ = resolver.resolve(address, std::to_string(port));
	}
	void TCPClient::Run()
	{
		io::async_connect(
			socket_, endpoints_, [this](boost::system::error_code error, io::ip::tcp::endpoint ep) {
				if (!error)
					AsyncRead();
			});
		io_context_.run();
	}
	void TCPClient::Stop()
	{
		boost::system::error_code error;
		socket_.close();

		if (error)
		{
		}
	}
	void TCPClient::SendMessage(const std::string& message)
	{
		bool queue_idle = outgoing_messages_.empty();
		outgoing_messages_.push(message);

		if (queue_idle)
			AsyncWrite();
	}
	void TCPClient::AsyncRead()
	{
		io::async_read_until(socket_, stream_buf_, "\n", [this](boost::system::error_code error, size_t bytes_transferred) {
			OnRead(error, bytes_transferred);
		});
	}
	void TCPClient::OnRead(boost::system::error_code error, size_t bytes_transffered)
	{
		if(error) {
			Stop();
			return;
		}

		std::stringstream message;
		message << std::istream{&stream_buf_}.rdbuf();
		on_message_(message.str());
		AsyncRead();
	}
	void TCPClient::AsyncWrite()
	{
		io::async_write(socket_, io::buffer(outgoing_messages_.front()),  [this](boost::system::error_code error, size_t bytes_transferred) {
			OnWrite(error, bytes_transferred);
		});
	}
	void TCPClient::OnWrite(boost::system::error_code error, size_t bytes_transffered)
	{
		if(error) {
			Stop();
			return;
		}

		outgoing_messages_.pop();

		if(!outgoing_messages_.empty())
			AsyncWrite();
	}

} // namespace chat
