
#ifndef URL_H
#define URL_H

#include <string>

class Url {
public:
    explicit Url(const std::string& str);
    Url() {}
    ~Url();

    std::string Path() const;
    std::string Host() const;
    int Port() const;

    bool IsEmpty() const;

    void SetUrl(const std::string& url);

    void SetPath(const std::string& path);
    void SetHost(const std::string& host);
    void SetPort(int port);

    std::string ToString() const;

private:
    std::string host_;
    std::string path_;
    int port_;

};

#endif // URL_H
