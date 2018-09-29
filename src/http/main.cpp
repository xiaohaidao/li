#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "asio.hpp"
#include "requests.h"

int main(int argc, char* argv[]) {
    Request re;
    re.SetUrl("www.baidu.com");

    re.Open();
    std::fstream f;
    f.open("1.html", std::ios::out);
    f.write(re.Text().c_str(), re.Text().size());
    f.close();
    //for (int i = 0; i < re.Text().size(); ++i) {
    //    std::cout << re.Text()[i];
    //}

    //try {
    //    asio::io_service io;
    //    //asio::ip::tcp::endpoint ep()
    //    asio::ip::tcp::resolver rs(io);
    //    asio::ip::tcp::resolver::query qu("www.baidu.com", "http");
    //    asio::ip::tcp::resolver::iterator endp = rs.resolve(qu);
    //    asio::ip::tcp::resolver::iterator end;

    //    asio::ip::tcp::socket sock(io);
    //    asio::error_code error = asio::error::host_not_found;
    //    while (error && endp != end) {
    //        sock.close();
    //        sock.connect(*endp++, error);
    //    }
    //    if (error) {
    //        return 0;
    //    }

    //    sock.write_some(asio::buffer(s));
    //    for (;;) {
    //        char buff[512];
    //        asio::error_code erro;
    //        size_t len = asio::read(sock, asio::buffer(buff), erro);

    //        if (erro == asio::error::eof)
    //            break;
    //        else if (erro) {
    //            throw asio::system_error(erro);
    //        }
    //        std::cout.write(buff, len);
    //    }
    //}
    //catch (std::exception &e) {
    //    std::cout << e.what() << std::endl;
    //}
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
