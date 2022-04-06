//
// Created by Tudor Coman on 05.04.2022.
//

#ifndef OOP_DEPOZITCONTROLLER_H
#define OOP_DEPOZITCONTROLLER_H

#include "../../core/headers/Controller.h"
#include "../../infra/headers/DepoziteRepository.h"

class DepozitController: public Controller {
    DepoziteRepository depoziteRepository;
    friend class WebService;
public:
    explicit DepozitController(const std::string& path, AngajatRepository angajatRepository);
    void handle_get(const http_request& req) override;
    void handle_post(const http_request& req) override;
    void handle_delete(const http_request& req) override;
};


#endif //OOP_DEPOZITCONTROLLER_H
