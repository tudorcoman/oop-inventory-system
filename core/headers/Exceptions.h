//
// Created by Tudor Coman on 05.05.2022.
//

#ifndef OOP_EXCEPTIONS_H
#define OOP_EXCEPTIONS_H

#include <stdexcept>

class IllegalFieldException: public std::runtime_error {
public:
    explicit IllegalFieldException(): runtime_error("Illegal Field") { };
};

class IllegalArgumentException: public std::runtime_error {
public:
    explicit IllegalArgumentException(): runtime_error("Illegal Value") { };
};

class IdNotFoundException: public std::runtime_error {
public:
    explicit IdNotFoundException(const std::string& str): runtime_error("ID (" + str + ") not found") { }
};



#endif //OOP_EXCEPTIONS_H
