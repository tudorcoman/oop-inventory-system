//
// Created by Tudor Coman on 03.04.2022.
//

#ifndef OOP_PRODUSCONTROLLER_H
#define OOP_PRODUSCONTROLLER_H

#include "../../infra/headers/ProdusRepository.h"
#include "../../core/headers/Controller.h"
#include "HTTPListener.h"
#include "../../infra/headers/SQLRepository.h"

class ProdusController: public Controller {
    ProdusRepository produsRepository;
    friend class WebService;
public:
    explicit ProdusController(const std::string& path);
    void handle_get(const http_request& req) override;
    void handle_post(const http_request& req) override;
    void handle_put(const http_request& req) override;
    void handle_delete(const http_request& req) override;
    ProdusController& operator = (const ProdusController& other) = default;
    std::shared_ptr<Controller> clone() const override;
};


#endif //OOP_PRODUSCONTROLLER_H
