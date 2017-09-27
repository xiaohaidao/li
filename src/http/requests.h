
#ifndef REQUESTS_H
#define REQUESTS_H

#include <string>
#include "header.h"

//namespace header {
//class RequestHeader;
//}

class Request {
public:
    Request();
    ~Request();

    void SetHeader(const std::string header);

    void Get(const std::string &url) const;

private:
    header::RequestHeader header_;
};

#endif // REQUESTS_H