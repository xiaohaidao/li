

#ifndef HEADER_H
#define HEADER_H

#include <map>

#include "Url.h"

namespace header {

class RequestHeader {
    public:
        explicit RequestHeader(const std::string &header);
        RequestHeader();

        bool HeaderToString(std::string &header, int &error_code);
        void SetUrl(const std::string &url);
        bool SetHeader(const std::string &header);
        bool SetHeader(const std::string &type, const std::string &text);
        std::string GetHost() const;
        std::string GetPath() const;

    private:
        void TranToUpper(std::string &text);
        void Trim(std::string &text);

        std::map<std::string, std::string> header_;
        Url url_;
};

class ResponseHeader {
    public:
        ResponseHeader(std::string header);
    private:
        std::map<std::string, std::string> header_;
};

} // namespace header

#endif // HEADER_H
