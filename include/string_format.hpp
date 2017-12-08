#ifndef STRING_FORMAT
#define STRING_FORMAT

#include <cstdarg>
#include <regex>
#include <sstream>

inline int Trim(std::string &str) {
  std::regex re("^ *| *$");
  str = std::regex_replace(str, re, "");
  return 0;
}

inline int WTrim(std::wstring &str) {
  std::wregex re("^ *| *$");
  str = std::regex_replace(str, re, "");
  return 0;
}

/*
 *
 * Format stop is determined by "{num}", template Format stop is demermined by
 * number of args
 * Format args must be string, template formate args is int/string/double...
 *
 */

inline std::string Format(const char *str...) {
  va_list args;
  va_start(args, str);
  std::string re = str;

  char buff[10];
  std::string x =  va_arg(args, char *);
  for (size_t i = 0; i < 100000; x = va_arg(args, char *), ++i) {
    snprintf(buff, sizeof(buff), "\\{%d\\}", i);
    std::regex regular(buff);
    std::smatch march;
    std::regex_search(re, march, regular);
    if (march.empty()) { break; }
    re = std::regex_replace(re, regular, x);
  }
  va_end(args);
  return re;
}

inline std::wstring WFormat(const wchar_t *str...) {
  va_list args;
  va_start(args, str);
  std::wstring re = str;

  wchar_t buff[10];
  std::wstring x =  va_arg(args, wchar_t *);
  for (size_t i = 0; i < 100000; x = va_arg(args, wchar_t *), ++i) {
    swprintf(buff, sizeof(buff), L"\\{%d\\}", i);
    std::wregex regular(buff);
    std::wsmatch march;
    std::regex_search(re, march, regular);
    if (march.empty()) { break; }
    re = std::regex_replace(re, regular, x);
  }
  va_end(args);
  return re;
}

namespace _internal {

  template<class T>
  std::string _TFormat(size_t i, T &result) { return result; }

  template <class T1, class T2, class ...Args>
  inline std::string _TFormat(size_t i, T1 &result, const T2 &replace, Args... args) {
    std::stringstream ss;
    ss << replace;
    char buff[10];
    snprintf(buff, sizeof(buff), "\\{%d\\}", i);
    result = std::regex_replace(result, std::regex(buff), ss.str());
    return _TFormat(++i, result, args...);
  }

  template<class T>
  std::wstring _TWFormat(size_t i, T &result) { return result; }

  template <class T1, class T2, class ...Args>
  inline std::wstring _TWFormat(size_t i, T1 &result, const T2 &replace, Args... args) {
    std::wstringstream ss;
    ss << replace;
    wchar_t buff[10];
    swprintf(buff, sizeof(buff), L"\\{%d\\}", i);
    result = std::regex_replace(result, std::wregex(buff), ss.str());
    return _TWFormat(++i, result, args...);
  }

} // namespace _internal

template <class T, class ..._T>
inline std::string TFormat(T format, _T... args) {
  std::string r = format;
  return _internal::_TFormat(0, r, args...);
}

template <class T, class ..._T>
inline std::wstring TWFormat(T format, _T... args) {
  std::wstring r = format;
  return _internal::_TWFormat(0, r, args...);
}

#endif /* ifndef STRING_FORMAT */

