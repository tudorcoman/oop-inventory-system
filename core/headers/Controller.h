//
// Created by Tudor Coman on 02.04.2022.
//

#ifndef OOP_CONTROLLER_H
#define OOP_CONTROLLER_H

#include "../../service/headers/HTTPListener.h"
#include "../../core/headers/Repository.h"

using namespace std::placeholders;

class Controller {
    std::string path;
public:
    Controller(const std::string &path) : path(path) {}

    virtual HTTPListener::RequestHandler getRequestHandler() {
        std::function<void(http_request)> fp_get = std::bind(&Controller::handle_get, this, _1);

        std::function<void(http_request)> fp_post = std::bind(&Controller::handle_post, this, _1);
        std::function<void(http_request)> fp_delete = std::bind(&Controller::handle_delete,this, _1);

        return HTTPListener::RequestHandler()
                        .support(methods::GET, path, fp_get)
                        .support(methods::POST, path, fp_post)
                        .support(methods::DEL, path, fp_delete);
    }

    virtual void handle_get(const http_request& req) = 0;
    virtual void handle_post(const http_request& req) = 0;
    virtual void handle_delete(const http_request& req) = 0;
};


#endif //OOP_CONTROLLER_H
