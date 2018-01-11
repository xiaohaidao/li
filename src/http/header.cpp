//http://kb.cnblogs.com/page/92320/

#include "header.h"

#include "string_format.hpp"

static std::map<std::string, std::string> requests = {
    { "Accept", "text/plain, text/html" },
    { "Accept-Charset", "iso-8859-5" },
    { "Accept-Encoding", "compress, gzip" },
    { "Accept-Language", "en,zh" },
    { "Accept-Ranges", " bytes" },
    { "Authorization", "Basic QWxhZGRpbjpvcGVuIHNlc2FtZQ==" },
    { "Cache-Control", "no-cache" },
    { "Connection", " close" },
    { "Cookie", " $Version=1; Skin=new;" },
    { "Content-Length", "348" },
    { "Content-Type", "application/x-www-form-urlencoded" },
    { "Date", "Tue, 15 Nov 2010 08:12:31 GMT" },
    { "Expect", " 100-continue" },
    { "From", "user@email.com" },
    { "Host", "www.zcmhi.com" },
    { "If-Match", " 737060cd8c284d8af7ad3082f209582d" },
    { "If-Modified-Since", " Sat, 29 Oct 2010 19:43:31 GMT" },
    { "If-None-Match", "737060cd8c284d8af7ad3082f209582d" },
    { "If-Range", " 737060cd8c284d8af7ad3082f209582d" },
    { "If-Unmodified-Since", " Sat, 29 Oct 2010 19:43:31 GMT" },
    { "Max-Forwards", " 10" },
    { "Pragma", " no-cache" },
    { "Proxy-Authorization", " Basic QWxhZGRpbjpvcGVuIHNlc2FtZQ==" },
    { "Range", "bytes=500-999" },
    { "Referer", " http://www.zcmhi.com/archives/71.html" },
    { "TE", "railers,deflate;q=0.5" },
    { "Upgrade", " HTTP/2.0, SHTTP/1.3, IRC/6.9, RTA/x11" },
    { "User-Agent", "Mozilla/5.0 (Linux; X11)" },
    { "Via", "1.0 fred, 1.1 nowhere.com (Apache/1.1)" },
    { "Warning", " 199 Miscellaneous warning" },
};

static std::map<std::string, std::string> responses = {
    { "Accept-Ranges", "bytes" },
    { "Age", " 12" },
    { "Allow", " GET, HEAD" },
    { "Cache-Control", " no-cache" },
    { "Content-Encoding", " gzip" },
    { "Content-Language", " en,zh" },
    { "Content-Length", " 348" },
    { "Content-Location", " /index.htm" },
    { "Content-MD5", " Q2hlY2sgSW50ZWdyaXR5IQ==" },
    { "Content-Range", "bytes 21010-47021/47022" },
    { "Content-Type", " text/html; charset=utf-8" },
    { "Date", " Tue, 15 Nov 2010 08:12:31 GMT" },
    { "ETag", "737060cd8c284d8af7ad3082f209582d" },
    { "Expires", " Thu, 01 Dec 2010 16:00:00 GMT" },
    { "Last-Modified", " Tue, 15 Nov 2010 12:45:26 GMT" },
    { "Location", " http://www.zcmhi.com/archives/94.html" },
    { "Pragma", " no-cache" },
    { "Proxy-Authenticate", " Basic" },
    { "refresh", " 5; url=http://www.zcmhi.com/archives/94.html" },
    { "Retry-After", " 120" },
    { "Server", " Apache/1.3.27 (Unix) (Red-Hat/Linux)" },
    { "Set-Cookie", " UserID=JohnDoe; Max-Age=3600; Version=1" },
    { "Trailer", " Max-Forwards" },
    { "Transfer-Encoding", "chunked" },
    { "Vary", " *" },
    { "Via", " 1.0 fred, 1.1 nowhere.com (Apache/1.1)" },
    { "Warning", " 199 Miscellaneous warning" },
    { "WWW-Authenticate", " Basic" },
};

//static std::map<int, std::string>  status_code = {
//    // ① 客户方错误
//    {100, "继续"},
//    {101, "交换协议"},
//    //② 成功
//    {200, "OK"},
//    {201, "已创建"},
//    {202, "接收"},
//    {203, "非认证信息"},
//    {204, "无内容"},
//    {205, "重置内容" },
//    {206, "部分内容" },
//    // ③ 重定向
//    {300, "多路选择"},
//    {301, "永久转移" },
//    {302, "暂时转移" },
//    {303, "参见其它" },
//    {304, "未修改（Not Modified）" },
//    {305, "使用代理" },
//    // ④ 客户方错误
//    {400, "错误请求（Bad Request）"},
//    {401, "未认证" },
//    {402, "需要付费" },
//    {403, "禁止（Forbidden）" },
//    {404, "未找到（Not Found）" },
//    {405, "方法不允许" },
//    {406, "不接受" },
//    {407, "需要代理认证" },
//    {408, "请求超时" },
//    {409, "冲突" },
//    {410, "失败" },
//    {411, "需要长度" },
//    {412, "条件失败" },
//    {413, "请求实体太大" },
//    {414, "请求URI太长" },
//    {415, "不支持媒体类型" },
//    // ⑤ 服务器错误
//    {500, "服务器内部错误"},
//    {501, "未实现（Not Implemented）" },
//    {502, "网关失败" },
//    {504, "网关超时" },
//    {505, "HTTP版本不支持"}
//};

namespace header {

using std::regex;
using std::regex_match;
using std::regex_search;
using std::regex_replace;
using std::smatch;

void TranToUpper(std::string &text) {
    std::transform(text.begin(), text.end(), text.begin(), tolower);
    std::transform(text.begin(), text.begin() + 1, text.begin(), toupper);
}

RequestHeader::RequestHeader(const std::string &header) {
    SetHeader(header);
}

RequestHeader::RequestHeader() {
    SetHeader("Accept", "*/*");
    SetHeader("Connection", "close");
}

int RequestHeader::Clear() {
  header_.clear();
  url_.Clear();
}

int RequestHeader::GetHeader(std::string &header) {
    header.clear();
    auto iter = header_.find("Host");
    if (iter == header_.end()) {
        return 1;
    }
    for (auto i = header_.begin(); i != header_.end(); ++i) {
        header += (i->first + ": " + i->second + "\r\n");
    }
    header += "\r\n";
    return 0;
}

void RequestHeader::SetUrl(const std::string &url) {
    url_.SetUrl(url);
    header_["Host"] = url_.Host();
}

bool RequestHeader::SetHeader(const std::string &header) {
    std::regex split("(\\b[^:]*):([^\\r\\n]*)");

    std::smatch result;
    std::string h_type, h_text, text = header;
    do {
        std::regex_search(text, result, split);
        text = result.suffix();
        h_type = result[1].str();
        h_text = result[2].str();
        Trim(h_type); Trim(h_text);
        TranToUpper(h_type);
        SetHeader(h_type, h_text);
    } while (result[1].matched || result[2].matched);
    return true;
}

bool RequestHeader::SetHeader(const std::string &type, const std::string &text) {
    auto iter = requests.find(type);
    if (iter != requests.end()) {
        header_[type] = text;
        return true;
    }
    return false;
}

std::string RequestHeader::GetHost() const {
    auto iter = header_.find("Host");
    if (iter == header_.end()) {
        return "";
    }
    return iter->second;
}

std::string RequestHeader::GetPath() const {
    return url_.Path();
}

////////////////////////////////////////////////////////////////////////////////

ResponseHeader::ResponseHeader() : done_2_(0), status_(0), first_line_(true) {

}

int ResponseHeader::GetStatus() {
  return status_;
}

int ResponseHeader::Clear() {
  done_2_ = 0;
  status_ = 0;
  header_.clear();
  first_line_ = true;
}

int ResponseHeader::Paser(std::string &str) {
  std::regex ret("\\r\\n");
  std::smatch result;
  std::regex_search(str, result, ret);
  for (; !result.empty() || done_2_ == 2; std::regex_search(str, result, ret)) {
    if (result.prefix.size() == 0) {
      ++done_2_;
    } else {
      PaserLine(result.prefix);
    }
    str = result.suffix;
  }
}

int ResponseHeader::PaserLine(const std::string &str) {
  smatch result;
  if (first_line_) {
    regex first_line("HTTP/1.[0,1] ([0-9]\\+) .*");
    regex_search(str, result, first_line);

    if (!result.empty()) {
      first_line_ = false;
      if (result[1].match()) {
        status_ = result[1];
      }
      return 0;
    }
  }
  regex paser(" *([^ ]*):(.*)");
  regex_search(str, result, paser);
  header_[TranToUpper(Trim(result[1]))] = Trim(result[2]);
}


} // namespace header

