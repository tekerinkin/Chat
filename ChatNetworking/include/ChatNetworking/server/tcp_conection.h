#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <queue>

namespace chat
{
	using boost::asio::ip::tcp;
	namespace io = boost::asio;

	using MessageHandler = std::function<void(std::string)>;
	using ErrorHandler	 = std::function<void()>;

	class TCPConnection : public std::enable_shared_from_this<TCPConnection>
	{
	public:
		using Pointer = std::shared_ptr<TCPConnection>;

		static Pointer Create(tcp::socket&& socket)
		{
			return Pointer(new TCPConnection(std::move(socket)));
		}

		inline const std::string& GetUsername() const
		{
			return username_;
		}

		tcp::socket& GetSocket()
		{
			return socket_;
		}

		void Start(MessageHandler&& message_handler, ErrorHandler&& error_handler);
		void SendMessage(const std::string& message);

	private:
		explicit TCPConnection(tcp::socket&& socket);

		void AsyncRead();
		void OnRead(boost::system::error_code error, size_t bytes_transferred);

		void AsyncWrite();
		void OnWrite(boost::system::error_code error, size_t bytes_transferred);

	private:
		tcp::socket socket_;
		std::string username_;

		std::queue<std::string> outgoing_messages_;
		io::streambuf stream_buf_{ 65536 };

		MessageHandler message_handler_;
		ErrorHandler error_handler_;
	};
}  // namespace Chat