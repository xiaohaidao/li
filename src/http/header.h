#ifndef HEADER_H
#define HEADER_H

#include <map>

#include "Url.h"

namespace header {

class RequestHeader {
  public:
    explicit RequestHeader(const std::string &header);
    RequestHeader();

    int Clear();

    /**
     * @brief GetHeader Get the header string
     *
     * @param header result
     *
     * @return 0 successful, 1 error.
     */
    int GetHeader(std::string &header);
    void SetUrl(const std::string &url);
    bool SetHeader(const std::string &header);
    bool SetHeader(const std::string &type, const std::string &text);
    std::string GetHost() const;
    std::string GetPath() const;

  private:
    std::map<std::string, std::string> header_;
    Url url_;
};

class ResponseHeader {
  public:
    ResponseHeader();
    int GetStatus();
    int Paser(std::string &str);
    int Clear();

  private:
    int PaserLine(const std::string &str);

    std::map<std::string, std::string> header_;
    int done_2_;
    int status_;
    bool fisrt_line_;
};

} // namespace header

#endif // HEADER_H
