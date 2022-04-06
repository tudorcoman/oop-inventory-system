//
// Created by Tudor Coman on 02.04.2022.
//

#ifndef OOP_ANGAJATCONTROLLER_H
#define OOP_ANGAJATCONTROLLER_H

#include "../../core/headers/Controller.h"
#include "HTTPListener.h"
#include "../../infra/headers/SQLRepository.h"
#include "../../infra/headers/AngajatRepository.h"

class AngajatController: public Controller {
    AngajatRepository angajatRepository;
    friend class WebService;
public:
    explicit AngajatController(const std::string& path);
    void handle_get(const http_request& req) override;
    void handle_post(const http_request& req) override;
    void handle_delete(const http_request& req) override;
};


#endif //OOP_ANGAJATCONTROLLER_H
