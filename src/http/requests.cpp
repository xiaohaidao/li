#include "requests.h"
//#include <future>
#include <functional>

using asio::ip::tcp;

asio::io_service Request::IO;

Request::Request() : socket_(IO) {}

Request::~Request() {}

int Request::Open() {
    text_.clear();

    tcp::resolver res(IO);
    tcp::resolver::query qu(GetHost(), "http");
    tcp::resolver::iterator endp = res.resolve(qu);
    tcp::resolver::iterator end;
    try {
        asio::error_code error = asio::error::host_not_found;;
        while(error && endp != end) {
            socket_.close();
            socket_.connect(*endp++, error);
        }
        if (error) {
            return 1;
        }

        text_.clear();
        socket_.write_some(asio::buffer(GetHeader()));
        for (;;) {
            char buff[512];
            //size_t len = asio::read(socket_,asio::buffer(buff), error);
            size_t len = socket_.read_some(asio::buffer(buff), error);

            if (error == asio::error::eof)
                break;
            else if (error) {
                break;
            }
            text_ += std::string(buff, len);
        }
    }
    catch (std::exception &e) {
        return 1;
        socket_.close();
    }
    socket_.close();
    return 0;
}

std::string Request::GetHost() const {
    return header_.GetHost();
}

std::string Request::GetHeader() {
    std::string st = FirstStr();
    std::string jjj;
    if (!header_.GetHeader(jjj)) {
        st += jjj;
    }
    return st;
}

const std::string &Request::Text() const {
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

void Request::SetUrl(const std::string &url) {
    header_.SetUrl(url);
    type_ = GET;
}
