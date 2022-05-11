//
// Created by Tudor Coman on 01.04.2022.
//

#include "../headers/HTTPListener.h"
#include <functional>

using namespace std::placeholders;

void HTTPListener::handle_request(const http_request& message, const method& metoda) const {
    const std::vector<std::string> path_tokens = Utilities::string_split(message.to_string(), " ");
    if (path_tokens.size() < 2) {
        throw IncorrectUrlPathException();
    }

    const std::string cale = Utilities::string_split(path_tokens[1], url)[1];
    std::cout << cale << std::endl;
    const std::pair<std::string, method> key = make_pair(cale.substr(0, cale.find("?")), metoda);
    if(!handlers.contains(key)) {
        message.reply(status_codes::NotFound, "Calea ceruta nu a putut fi gasita");
        throw UrlPathNotFoundException();
    }

    try {
        handlers.at(key)(message);
    } catch(const InternalErrorException& e) {
        message.reply(status_codes::InternalError, e.what());
    }
}

void HTTPListener::handle_req(const http_request &message, const std::string& type, const std::map<std::string, method>& matching) const {
    handle_request(message, matching.at(type));
}

HTTPListener::HTTPListener(const std::string &host, const std::string &url, const RequestHandler& requestHandler) : host(host), url(url) {
    const std::map<std::string, method> matching = {
            {"GET", methods::GET},
            {"POST", methods::POST},
            {"PUT", methods::PUT},
            {"DELETE", methods::DEL}
    };

    for (const auto& it: requestHandler.getRecords()) {
        handlers[make_pair(std::get<1>(it), std::get<0>(it))] = std::get<2>(it);
    }

    listener = web::http::experimental::listener::http_listener(U(host + url));

    for(const auto& tip: matching) {
        auto fp = std::bind(&HTTPListener::handle_req, this, _1, tip.first, matching);
        listener.support(tip.second, fp);
    }
}

pplx::task<void> HTTPListener::open() {
    return listener.open();
}


HTTPListener::RequestHandler HTTPListener::RequestHandler::support(const method &method, const std::string &s, std::function<void(http_request)> handler) {
    records.emplace_back(method, s, handler);
    return *this;
}

const std::vector<std::tuple<method, std::string, std::function<void(http_request)>>> &
HTTPListener::RequestHandler::getRecords() const {
    return records;
}

HTTPListener::RequestHandler
HTTPListener::RequestHandler::support(const HTTPListener::RequestHandler &requestHandler) {
    for (const auto& it: requestHandler.records) {
        this->records.emplace_back(std::get<0>(it), std::get<1>(it), std::get<2>(it));
    }
    return *this;
}
