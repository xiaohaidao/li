
#ifndef REQUESTS_H
#define REQUESTS_H

#include <string>
#include "header.h"
#include "asio.hpp"

class Request {
public:
    Request();
    ~Request();

    int Open();
    const std::string &Text() const;
    void SetHeader(const std::string &header);
    void SetUrl(const std::string &url);
    std::string GetHeader();
    std::string GetHost() const;

private:
    enum type {
        GET,
        POST,
    };
    std::string FirstStr() const;

    header::RequestHeader header_;
    std::string text_;
    type type_;
    static asio::io_service IO;
    asio::ip::tcp::socket socket_;
};

#endif // REQUESTS_H
