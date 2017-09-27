/* Copyright (C)
 * 2017 - Bob Li, oxox0@qq.com
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#include <iostream>
#include <sstream>
#include <string>

#include "asio.hpp"
#include "Url.h"

using namespace asio;

using asio::ip::tcp;

int main(int argc, char* argv[]) {
    std::string a;
    Url url("www.baidu.com:999/index.htm");
    printf("%s\n", url.Host().c_str());
    printf("%s\n", url.Path().c_str());
	//try {
	//	if (argc != 3) {
	//		std::cout << "Usage: sync_client <server> <path>\n";
	//		std::cout << "Example:\n";
	//		std::cout << "  sync_client www.boost.org /LICENSE_1_0.txt\n";
	//		return 1;
	//	}

	//	asio::io_service io_service;

	//	// Get a list of endpoints corresponding to the server name.
	//	tcp::resolver resolver(io_service);
	//	tcp::resolver::query query(argv[1], "80");
	//	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	//	tcp::resolver::iterator end;

	//	// Try each endpoint until we successfully establish a connection.
	//	tcp::socket socket(io_service);
	//	error_code error = asio::error::host_not_found;
	//	while (error && endpoint_iterator != end) {
	//		socket.close();
	//		socket.connect(*endpoint_iterator++, error);
	//	}
	//	if (error)
	//		throw system_error(error);

	//	// Form the request. We specify the "Connection: close" header so that the
	//	// server will close the socket after transmitting the response. This will
	//	// allow us to treat all data up until the EOF as the content.
	//	asio::streambuf request;
	//	std::ostream request_stream(&request);
	//	request_stream << "GET " << argv[2] << " HTTP/1.0\r\n";
	//	request_stream << "Host: " << argv[1] << "\r\n";
	//	request_stream << "Accept: */*\r\n";
	//	request_stream << "Connection: close\r\n\r\n";

	//	// Send the request.
	//	asio::write(socket, request);

	//	// Read the response status line.
	//	asio::streambuf response;
	//    asio::read_until(socket, response, "\r\n");

	//	// Check that response is OK.
	//	std::istream response_stream(&response);
	//	std::string http_version;
	//	response_stream >> http_version;
	//	unsigned int status_code;
	//	response_stream >> status_code;
	//	std::string status_message;
	//	std::getline(response_stream, status_message);
	//	if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
	//		std::cout << "Invalid response\n";
	//		return 1;
	//	}
	//	if (status_code != 200) {
	//		std::cout << "Response returned with status code " << status_code
	//			<< "\n";
	//		return 1;
	//	}

	//	// Read the response headers, which are terminated by a blank line.
	//	asio::read_until(socket, response, "\r\n\r\n");

	//	// Process the response headers.
	//	std::string header;
	//	while (std::getline(response_stream, header) && header != "\r");
	//	std::cout << header << "\n";
	//	std::cout << "\n";

	//	// Write whatever content we already have to output.
	//	if (response.size() > 0)
	//		std::cout << &response;

	//	// Read until EOF, writing data to output as we go.
	//	while (asio::read(socket, response,
	//		asio::transfer_at_least(1), error))
	//		std::cout << &response;
	//	if (error != asio::error::eof)
	//		throw system_error(error);
	//}
	//catch (std::exception& e) {
	//	std::cout << "Exception: " << e.what() << "\n";
	//}

	return 0;
}
