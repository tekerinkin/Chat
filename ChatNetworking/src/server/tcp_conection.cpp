#include "ChatNetworking/server/tcp_conection.h"
#include <iostream>

namespace chat
{
	TCPConnection::TCPConnection(tcp::socket&& socket)
		: socket_(std::move(socket))
	{
		boost::system::error_code error;
		std::stringstream name;
		name << socket_.remote_endpoint();

		username_ = name.str();
	}

	void TCPConnection::Start(MessageHandler&& message_handler, ErrorHandler&& error_handler)
	{
		message_handler_ = std::move(message_handler);
		error_handler_	 = std::move(error_handler);
		AsyncRead();
	}

	void TCPConnection::SendMessage(const std::string& message)
	{
		bool queue_idle = outgoing_messages_.empty();
		outgoing_messages_.push(message);

		if (queue_idle)
		{
			AsyncWrite();
		}
	}

	void TCPConnection::AsyncRead()
	{
		io::async_read_until(
			socket_, stream_buf_, "\n",
			[self = shared_from_this()](boost::system::error_code error, size_t bytes_transferred) {
				self->OnRead(error, bytes_transferred);
			});
	}

	void TCPConnection::OnRead(boost::system::error_code error, size_t bytes_transferred)
	{
		if (error)
		{
			socket_.close();

			error_handler_();
			return;
		}

		std::stringstream msg;
		msg << username_ << ": " << std::istream(&stream_buf_).rdbuf();

		stream_buf_.consume(bytes_transferred);
		std::cout << msg.str() << std::endl;

		message_handler_(msg.str());
		AsyncRead();
	}

	void TCPConnection::AsyncWrite()
	{
		io::async_write(
			socket_, io::buffer(outgoing_messages_.front()),
			[self = shared_from_this()](boost::system::error_code error, size_t bytes_transferred) {
				self->OnWrite(error, bytes_transferred);
			});
	}

	void TCPConnection::OnWrite(boost::system::error_code error, size_t bytes_transferred)
	{
		if (error)
		{
			socket_.close(error);

			error_handler_();
			return;
		}

		outgoing_messages_.pop();

		if (!outgoing_messages_.empty())
		{
			AsyncWrite();
		}
	}

} // namespace chat
