
#ifndef REQUESTS_H
#define REQUESTS_H

#include <string>
#include "header.h"

class Request {
public:
    Request();
    ~Request();

    void Open();
    const std::string &Text();
    void SetHeader(const std::string &header);
    void Get(const std::string &url);

private:
    enum type {
        GET,
        POST,
    };
    std::string ToStr();
    std::string FirstStr() const;

    header::RequestHeader header_;
    std::string text_;
    type type_;
};

#endif // REQUESTS_H
