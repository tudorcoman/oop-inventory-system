//
// Created by Tudor Coman on 01.04.2022.
//

#ifndef OOP_UTILITIES_H
#define OOP_UTILITIES_H

#include <string>
#include <vector>

class Utilities {
public:
    static std::vector<std::string> string_split(std::string s, std::string delim) {
        std::vector<std::string> result;

        auto start = 0U;
        auto end = s.find(delim);

        while(end != std::string::npos) {
            result.push_back(s.substr(start, end - start));
            start = end + delim.length();
            end = s.find(delim, start);
        }

        result.push_back(s.substr(start, end));
        return result;
    }
};
#endif //OOP_UTILITIES_H
