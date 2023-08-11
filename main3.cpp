
#include <chrono>
#include <iostream>

#include <thread>
#include <condition_variable>
#include <mutex>

#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>

#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>

#include <string>

#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/context.hpp>

#include "libs/nlohmann/json.hpp"

#include "order_update.h"
#include "Position.h"

#include <map>
#include <fstream>
#include "fmt/color.h"
#include "fmt/core.h"
std::mutex m;
std::condition_variable cv;
bool new_position = false;
Position latest_position;
// std::map<std::string, Position> positions;

void websocketThread()
{
	// The io_context is required for all I/O
	boost::asio::io_context ioc;

	// The SSL context is required, and holds certificates
	boost::asio::ssl::context ctx{boost::asio::ssl::context::tlsv12_client};

	// This holds the root certificate used for verification
	ctx.set_default_verify_paths();

	// These objects perform our I/O
	boost::asio::ip::tcp::resolver resolver{ioc};
	boost::beast::websocket::stream<boost::beast::ssl_stream<boost::asio::ip::tcp::socket>> wss{ioc, ctx};

	// boost::beast::websocket::stream<boost::asio::ip::tcp::socket> ws{ioc};

	// Look up the domain name
	auto const results = resolver.resolve("fstream.binance.com", "443");

	// Make the connection on the IP address we get from a lookup
	boost::asio::connect(wss.next_layer().next_layer(), results.begin(), results.end());

	// Perform the SSL handshake
	wss.next_layer().handshake(boost::asio::ssl::stream_base::client);

	// Perform the websocket handshake
	wss.handshake("fstream.binance.com", "/ws/..........................");

	// This buffer will hold the incoming message
	boost::beast::flat_buffer buffer;

	// Loop to keep the connection open and continuously read messages
	std::map<std::string, Position> positions;

	for (;;)
	{
		wss.read(buffer);

		std::string str(boost::beast::buffers_to_string(buffer.data()));
		json j = json::parse(str);
		json o = j["o"];

		if (j["e"] == "ORDER_TRADE_UPDATE" && o["x"] != "NEW" && o["t"] != "")
		{
			OrderUpdate ou{};
			ou.parse(str);
			// ou.print();
			buffer.consume(buffer.size());

			auto it = positions.find(ou.symbol);
			if (it == positions.end())
			{
				// This is a new position
				Position pos{ou};
				positions[ou.symbol] = pos;

				std::lock_guard<std::mutex> lock(m);
				latest_position = pos;
				new_position = true;

				// notify the other thread
				cv.notify_one();
			}
			else
			{

				// This is an existing position
				it->second.update(ou);
				if (it->second.isClosed())
				{
					// Position is closed. Log it and remove from map.
					positions.erase(it);
				}
			}
		}
		else
		{
			buffer.consume(buffer.size());
		}
	}
}
std::tm parse_unix_time(const long long time)
{
	std::chrono::milliseconds ms{time};
	time_t t = std::chrono::duration_cast<std::chrono::seconds>(ms).count();
	return *std::localtime(&t);
}

void userInputThread()
{
	const int separatorLength = 80; 
	const std::string separator = std::string(separatorLength, '-');

	std::ofstream file("output.txt", std::ios::app);

	while (true)
	{
		std::unique_lock<std::mutex> lock(m);
		cv.wait(lock, []
			   { return new_position; });

		std::tm tm_start = parse_unix_time(latest_position.time_start);

		std::stringstream ss;
		ss << std::put_time(&tm_start, "%H:%M:%S");
		std::string time_string = ss.str();

		fmt::print(fmt::fg(fmt::color::yellow), "\nSymbol: {}\t\tTime entry: {}", latest_position.symbol, time_string);
		latest_position.side == "SELL"
		    ? fmt::print(fmt::fg(fmt::color::dark_red), "\t\tSide: {}", latest_position.side)
		    : fmt::print(fmt::fg(fmt::color::light_green), "\t\tSide: {}", latest_position.side);

		fmt::print(fmt::fg(fmt::color::green), "\nArgs: \t");

		std::string args;
		std::string line;

		while (std::getline(std::cin, line))
		{
			if (line == "END")
			{
				std::cout << std::string(10, '-') << std::endl;
				break;
			}
			else
			{
				args += line + "\n";
			}
		}

		// Запись в файл
		file << separator << std::endl; 
		file << "Symbol: " << latest_position.symbol << "\t\tTime entry: "
			<< time_string << "\nArgs: " << args << std::endl;
		file << separator << std::endl
			<< std::endl; 

		new_position = false;
	}

	file.close();
}

int main()
{
	
	std::thread wsThread(websocketThread);
	std::thread uiThread(userInputThread);

	
	wsThread.join();
	uiThread.join();
	return 0;
}
