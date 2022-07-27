#include <ChatNetworking/client/tcp_client.h>
#include <iostream>
#include <thread>


int main(int argc, char* argv[]) {
	chat::TCPClient client {"localhost", 1337};

	client.on_message_ = [](const std::string& message) {
		std::cout << message << std::endl;
	};

	std::thread t([&client](){ client.Run(); });

	while(true) {
		std::string message;
		getline(std::cin, message);

		if(message == "\\q") break;
		message += "\n";

		client.SendMessage(message);
	}

	client.Stop();
	t.join();

  	return 0;
}
