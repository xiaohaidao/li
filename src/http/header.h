//http://kb.cnblogs.com/page/92320/

#include <map>

namespace header {
    std::map<std::string, std::string> requests = {
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
        { "If-Match", " “737060cd8c284d8af7ad3082f209582d”" },
        { "If-Modified-Since", " Sat, 29 Oct 2010 19:43:31 GMT" },
        { "If-None-Match", "“737060cd8c284d8af7ad3082f209582d”" },
        { "If-Range", " “737060cd8c284d8af7ad3082f209582d”" },
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

    std::map<std::string, std::string> responses = {
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
        { "ETag", "“737060cd8c284d8af7ad3082f209582d”" },
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

    enum UrlOpenType {
        GET,
        POST
    };

    class RequestHeader {
    public:
        RequestHeader(std::string header);

        RequestHeader() {
            SetOpenType();
            SetHeader("Accept", "*/*");
            SetHeader("Connection", "close");
        }

        bool HeaderToString(std::string &header, int &error_code) {
            header.clear();
            auto iter = header_.find("Host");
            if (iter == header_.end()) {
                error_code = 1; // host none error num
                return false;
            }
            // TODO(Muli): Transfer Get or Post open type
            for (auto i = header_.begin(); i != header_.end(); ++i) {
                header += (i->first + ": " + i->second + "\r\n");
            }
            header += "\r\n";
            return true;
        }

        bool SetHost(const std::string host) {
            return SetHeader("Host", host);
        }

        std::string GetHost() {
            auto iter = header_.find("Host");
            if (iter == header_.end()) {
                return "";
            }
            return header_["Host"];
        }

        void SetOpenType(UrlOpenType type = GET) {
            opentype_ = type;
        }

        UrlOpenType GetOpenType() {
            return opentype_;
        }

    private:
        bool SetHeader(const std::string &type, const std::string &text) {
            auto iter = requests.find(type);
            if (iter != requests.end()) {
                header_[type] = text;
                return true;
            }
            return false;
        }

        std::map<std::string, std::string> header_;
        UrlOpenType opentype_;
    };

    class ResponseHeader {
    public:
        ResponseHeader(std::string header);
    private:
        std::map<std::string, std::string> header_;
    };

} // namespace header 