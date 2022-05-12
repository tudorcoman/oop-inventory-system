//
// Created by Tudor Coman on 05.05.2022.
//

#ifndef OOP_EXCEPTIONS_H
#define OOP_EXCEPTIONS_H

#include <stdexcept>

class ApplicationException: public std::runtime_error {
public:
    explicit ApplicationException(const std::string& str): runtime_error(str) { }
};

class IncorrectUrlPathException: public ApplicationException {
public:
    explicit IncorrectUrlPathException(): ApplicationException("URL Path is not correct") { }
};

class UrlPathNotFoundException: public ApplicationException {
public:
    explicit UrlPathNotFoundException(): ApplicationException("URL Path is not handled") { }
};

class InternalErrorException: public ApplicationException {
public:
    explicit InternalErrorException(const std::string& str): ApplicationException(str) { }
};

class IllegalFieldException: public ApplicationException {
public:
    explicit IllegalFieldException(): ApplicationException("Illegal Field") { };
};

class IllegalArgumentException: public ApplicationException {
public:
    explicit IllegalArgumentException(): ApplicationException("Illegal Value") { };
};

class IdNotFoundException: public ApplicationException {
public:
    explicit IdNotFoundException(const std::string& str): ApplicationException("ID (" + str + ") not found") { }
};



#endif //OOP_EXCEPTIONS_H
