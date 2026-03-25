#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <sstream>
#include <cctype>

class Utils {
public:
    static std::string toLower(std::string s) {
        for (char &c : s)
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        return s;
    }

    static std::string trim(const std::string &s) {
        std::size_t start = 0;
        while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start])))
            ++start;

        if (start == s.size()) return "";

        std::size_t end = s.size() - 1;
        while (end > start && std::isspace(static_cast<unsigned char>(s[end])))
            --end;

        return s.substr(start, end - start + 1);
    }

    static std::vector<std::string> split(const std::string &s, char delimiter) {
        std::vector<std::string> out;
        std::stringstream ss(s);
        std::string item;

        while (std::getline(ss, item, delimiter)) {
            if (!item.empty())
                out.push_back(item);
        }
        return out;
    }
};

#endif // UTILS_H