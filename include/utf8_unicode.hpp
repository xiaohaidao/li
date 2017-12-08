#ifndef UTF8_UNICODE_H
#define UTF8_UNICODE_H

#include <codecvt>
#include <locale>
#include <string>

/*
 *
 * n |  Unicode(Hex)         | Utf-8(Binary)
 *---+-----------------------+------------------------------------------------------
 * 1 | 0000 0000 - 0000 007F |                                              0xxxxxxx
 * 2 | 0000 0080 - 0000 07FF |                                     110xxxxx 10xxxxxx
 * 3 | 0000 0800 - 0000 FFFF |                            1110xxxx 10xxxxxx 10xxxxxx
 * 4 | 0001 0000 - 0010 FFFF |                   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
 * 5 | 0020 0000 - 03FF FFFF |          111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 * 6 | 0400 0000 - 7FFF FFFF | 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 *
 */

inline std::wstring Utf8ToWstring(const std::string &str) {
  std::wstring_convert<std::codecvt_utf8<wchar_t> > convert;
  return convert.from_bytes(str);
}

inline std::string WstringToUtf8(const std::wstring &wstr) {
  std::wstring_convert<std::codecvt_utf8<wchar_t> > convert;
  return convert.to_bytes(wstr);
}

inline std::wstring Utf8ToUnicode(const std::string &str) {
  std::wstring result;
  for (size_t i = 0, count = -1; i < str.size(); ++i) {
    unsigned char a = (str[i] | 0x3F);
    if (0xBF != static_cast<unsigned char>(str[i] | 0x3F)) {
      unsigned char c = ~str[i];
      c |= c >> 1;
      c |= c >> 2;
      c |= c >> 4;
      c >>= 1;
      c &= str[i];
      result += c;
      count += 1;
    } else {
      result[count] = ((str[i] & 0x3F) | (result[count] << 6));
    }
  }
  return result;
}

inline std::string UnicodeToUtf8(const std::wstring &wstr) {
  std::string result;
  for (size_t i = 0; i < wstr.size(); ++i) {
    if (wstr[i] < 0x7F) {
      result += static_cast<unsigned char>(wstr[i] & 0x7F);
    } else if (wstr[i] <= 0x7FF) {
      result += static_cast<unsigned char>((wstr[i] >> 6) & 0x1F | 0xC0);
      result += static_cast<unsigned char>(wstr[i] & 0x3F | 0x80);
    } else if (wstr[i] <= 0xFFFF) {
      result += static_cast<unsigned char>((wstr[i] >> 6 * 2) & 0xF | 0xE0);
      result += static_cast<unsigned char>((wstr[i] >> 6) & 0x3F | 0x80);
      result += static_cast<unsigned char>(wstr[i] & 0x3F | 0x80);
    } else if (wstr[i] <= 0x10FFFF) {
      result += static_cast<unsigned char>((wstr[i] / (2 * 6 * 3)) & 0x7 | 0xF0);
      result += static_cast<unsigned char>((wstr[i] >> 6 * 2) & 0x3F | 0x80);
      result += static_cast<unsigned char>((wstr[i] >> 6) & 0x3F | 0x80);
      result += static_cast<unsigned char>(wstr[i] & 0x3F | 0x80);
    } else if (wstr[i] <= 0x3FFFFFF) {
      result += static_cast<unsigned char>((wstr[i] / (2 * 6 * 4)) & 0x3 | 0xF8);
      result += static_cast<unsigned char>((wstr[i] / (2 * 6 * 3)) & 0x3F | 0x80);
      result += static_cast<unsigned char>((wstr[i] >> 6 * 2) & 0x3F | 0x80);
      result += static_cast<unsigned char>((wstr[i] >> 6) & 0x3F | 0x80);
      result += static_cast<unsigned char>(wstr[i] & 0x3F | 0x80);
    } else if (wstr[i] <= 0x7FFFFFFF) {
      result += static_cast<unsigned char>((wstr[i] / (2 * 6 * 5)) & 0x1 | 0xFC);
      result += static_cast<unsigned char>((wstr[i] / (2 * 6 * 4)) & 0x3F | 0x80);
      result += static_cast<unsigned char>((wstr[i] / (2 * 6 * 3)) & 0x3F | 0x80);
      result += static_cast<unsigned char>((wstr[i] >> 6 * 2) & 0x3F | 0x80);
      result += static_cast<unsigned char>((wstr[i] >> 6) & 0x3F | 0x80);
      result += static_cast<unsigned char>(wstr[i] & 0x3F | 0x80);
    }
  }
  return result;
}

#endif /* ifndef UTF8_UNICODE_H */
