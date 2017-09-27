#include "requests.h"
#include "asio.hpp"

asio::io_service service;

Request::Request() {

}

Request::~Request() {

}

void Request::Get(const std::string &url) const {
    //	"GET " << argv[2] << " HTTP/1.1\r\n";
}