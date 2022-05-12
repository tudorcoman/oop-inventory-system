//
// Created by Tudor Coman on 01.04.2022.
//

#ifndef OOP_HTTPLISTENER_H
#define OOP_HTTPLISTENER_H

#include <cpprest/http_client.h>
#include <cpprest/http_listener.h>
#include <cpprest/filestream.h>
#include <string>
#include "../../infra/headers/Utilities.h"

using namespace web;
using namespace web::http;

class HTTPListener {
    experimental::listener::http_listener listener;
    std::string host;
    std::string url;
    std::map<std::pair<std::string, method>, std::function<void(http_request)>> handlers;
    void handle_req(const http_request &message, const std::string& type, const std::map<std::string, method>& matching) const;
    void handle_request(const http_request& message, const method& metoda) const;
public:
    class RequestHandler {
        std::vector<std::tuple<method, std::string, std::function<void(http_request)>>> records;

    public:
        RequestHandler() = default;
        RequestHandler support(const method& method, const std::string& s, std::function<void(http_request)> handler);
        RequestHandler support(const RequestHandler& requestHandler);
        [[nodiscard]] const std::vector<std::tuple<method, std::string, std::function<void(http_request)>>> &getRecords() const;
    };

    HTTPListener(const std::string& host, const std::string &url, const RequestHandler& requestHandler);
    pplx::task<void> open();
};

#endif //OOP_HTTPLISTENER_H
