#include <string>

#ifndef UTIL_H
#define UTIL_H

const char* ws_chars = " \t\n\r\f\v";

// trim from end of string (right)
inline std::string& rtrim(std::string& s, const char* t = ws_chars)
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

// trim from beginning of string (left)
inline std::string& ltrim(std::string& s, const char* t = ws_chars)
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

// trim from both ends of string (right then left)
inline std::string& trim(std::string& s, const char* t = ws_chars)
{
    return ltrim(rtrim(s, t), t);
}

inline std::string convert_to_str(const char* text, int size) {
    std::string str = "";
    for (int i = 0; i < size; i++) {
        str += text[i];
    }

    return str;
}

#endif