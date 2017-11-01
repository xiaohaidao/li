
#include "requests.h"
#include "asio.hpp"

/*
 *using asio::ip::tcp::socket;
 *using asio::ip::tcp::resolver;
 *using asio::ip::tcp::resolver::query;
 *using asio::ip::tcp::resolver::iterator;
 *
 *using ip::tcp::acceptor;
 *
 *
 */
/*
 * clients
 *asio::io_service service;
 *resolver re(service);
 *query que("www.boost.org", "http");
 *iterator iter = resolver.resolve(que);
 *iterator end;
 *
 *socket socket(service);
 *asio::connect(socket, resolver.resolve(qury)); // until one is successfully connected
 *socket.connect(endpoint); // when a specific endpoint is available
 *asio::async_connect(socket, iter, bind(&handle_connect, this, asio::placeholders::error));
 *void handle_connect(const error_code& error) {if (!error) {} else {}}
 *
 * server
 *aceptor accept(service, endpoint);
 *socket soc(service);
 *accept.accept(soc);
 */

Request::Request() {}

Request::~Request() {}


void Request::Open() {}

std::string Request::ToStr() {
    std::string st = FirstStr();
    std::string jjj;
    int error;
    if (header_.HeaderToString(jjj, error)) {
        st += jjj;
    }
    return st;
}

const std::string &Request::Text() {
    return text_;
}

std::string Request::FirstStr() const {
    std::string str;
    if (type_ == GET && !!header_.GetHost().size()) {
        str = "GET " + header_.GetPath() + " HTTP/1.1\r\n";
    }
    return str;
}

void Request::SetHeader(const std::string &header) {
    header_.SetHeader(header);
}

void Request::Get(const std::string &url) {
    header_.SetUrl(url);
    type_ = GET;
}
