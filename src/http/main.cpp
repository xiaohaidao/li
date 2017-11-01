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
#include <fstream>
#include <string>
#include <ctime>
#include <functional>

#include "asio.hpp"
#include "requests.h"

using namespace asio;

using asio::ip::tcp;

std::string MakeDateTime() {
    std::time_t now = time(0);
    return ctime(&now);
}

void example_servertime() {
    asio::io_service service;
    tcp::endpoint point(tcp::v4(), 8080);
    tcp::acceptor ac(service, point);

    while (true) {
        tcp::socket soc(service);
        ac.accept(soc);

        std::string mes = MakeDateTime();
        asio::error_code ig_error;
        asio::write(soc, asio::buffer(mes), ig_error);

    }

}

void getResult(const std::string& cmdPrefix, const char* outputFile, std::string& res)
{
    // cmd == "w > who"
    std::string cmd(cmdPrefix + outputFile);
    system(cmd.c_str());

    std::ifstream fin;
    fin.open(outputFile);
    if (fin) {
        std::ostringstream os;
        os << fin.rdbuf();
        res = os.str();
    }
    if (fin.is_open()) {
        fin.close();
    }
}

std::string getServiceContent(const int& select) {
    std::string res;
    switch (select) {
    case 1: {
        time_t t = time(0);
        res = ctime(&t);
        break;
    }
    case 2:
        getResult("w > ", "who", res);
        break;
    case 3:
        getResult("uname -a > ", "uname", res);
        break;
    default:
        res = "Sorry, no such service.\n";
        break;
    }
    return res;
}

void example_server2() {
    const char serviceList[] =
        "        Services          \n"
        "**************************\n"
        "[1] Get current time.     \n"
        "[2] Who's online.         \n"
        "[3] Get system info.      \n"
        "**************************\n"
        "Please pick a service[1-3]: ";
    asio::io_service io_service; // #1
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 8868)); // #2
    while (1)
    {
        tcp::socket socket(io_service);
        acceptor.accept(socket);

        asio::error_code ignored_error;
        asio::write(socket, asio::buffer(serviceList), ignored_error);

        char selection[20];
        size_t n = socket.read_some(asio::buffer(selection), ignored_error);

        std::string response = getServiceContent(atoi(selection));
        asio::write(socket, asio::buffer(response), asio::transfer_all(), ignored_error);
    }
}

class Connection {//: public enable_shared_from_this<Connection> {
public:
    Connection(asio::io_service& service) :
        sock(service) {
    }

    void start() {
        //        sock.async_read_some(boost::asio::buffer(buf), boost::bind(
        //                &Connection::handleRead, // ï¼?
        //                shared_from_this(), boost::asio::placeholders::error));
        asio::async_read(sock, asio::buffer(buf), std::bind(&Connection::handleRead, this)); // ï¼?
       //                 shared_from_this(),
                //        asio::placeholders::error));
    }

    tcp::socket& getSocket() {
        return sock;
    }

private:
    void handleRead(const asio::error_code& error) {
        if (!error) {
            std::cout << "recv from: " << sock.remote_endpoint().address() << ":" << sock.remote_endpoint().port() << std::endl;
            //            boost::asio::async_write(sock, boost::asio::buffer(buf),
            //                    boost::bind(
            //                            &Connection::handleWrite, // ï¼?
            //                            shared_from_this(),
            //                            boost::asio::placeholders::error));
            sock.async_write_some(asio::buffer(buf),
                std::bind(&Connection::handleWrite,this)); // ï¼?
                         //   shared_from_this(),
              //              asio::placeholders::error));
        }
    }

    void handleWrite(const asio::error_code& error) {
        if (!error) {
            memset(buf, 0, 512); // æ³¨æ„ï¼šé‡ç½®buf
            sock.async_read_some(asio::buffer(buf), std::bind(&Connection::handleRead,this)); // ï¼?
           //         shared_from_this(), 
     //               asio::placeholders::error));
        }
    }

private:
    tcp::socket sock;
    char buf[512];
};

typedef std::shared_ptr<Connection> ConnectionPtr;
class Server {
public:
    Server(asio::io_service& service) :
        acceptor(service, tcp::endpoint(tcp::v4(), 8080)) {
        start();
    }

private:
    void start() {
        ConnectionPtr conn(new Connection(acceptor.get_io_service()));
        acceptor.async_accept(conn->getSocket(), std::bind(
            &Server::handleAccept, this)); //conn,
          //      asio::placeholders::error));
    }

    void handleAccept(ConnectionPtr con, const asio::error_code& error) {
        if (!error) {
            con->start();
            start();
        }
    }

private:
    tcp::acceptor acceptor;
};


int main(int argc, char* argv[]) {
    Request re;
    re.Get("www.baidu.com");
    std::string s = re.ToStr();

    try {
        //example_servertime();
        example_server2();
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
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
