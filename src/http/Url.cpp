
#include "Url.h"

#include <regex>
#include <sstream>

using std::regex;
using std::regex_search;
using std::smatch;

Url::Url(const std::string& str) : port_(0) {
    SetUrl(str);
}

Url::~Url() {}

std::string Url::Path() const {
    return path_;
}

std::string Url::Host() const {
    return host_;
}

int Url::Port() const {
    return 80;
}

bool Url::IsEmpty() const {
    return host_.size() == 0;
}

void Url::SetPath(const std::string& path) {
    if (path.size() > 0) {
        std::string tmp;
        tmp = path[0];
        tmp.compare("/") == 0 ? path_ = path : path_ = "/" + path;
    }
}

void Url::SetHost(const std::string& host) {
    host_ = host;
}

void Url::SetPort(int port = 80) {
    port_ = port;
}

void Url::SetUrl(const std::string& url) {
    regex host("^(https?://)?([^/:]+):?([^/]+)?");

    smatch result;
    if (!regex_search(url, result, host) || result.size() != 4 ||
            !result[2].matched) {

        return;
    }
    host_ = result[2];
    if (result[3].matched) {
        std::stringstream sst;
        sst << result[3];
        sst >> port_;
    }

    path_ = result.suffix().str();
    if (path_.size() == 0) {
        path_ = "/";
    }
}

std::string Url::ToString() const {
    std::stringstream sst;
    if (!!port_) {
        sst << ":" << port_;
    }
    return host_ + sst.str() + path_;
}
void Url::Clear() {
  host_.clear();
  path_.clear();
  port_ = 80;
}
