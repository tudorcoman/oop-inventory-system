//
// Created by Tudor Coman on 06.04.2022.
//

#ifndef OOP_TRANZACTIECONTROLLER_H
#define OOP_TRANZACTIECONTROLLER_H


#include "../../core/headers/Controller.h"
#include "../../infra/headers/TranzactieRepository.h"

class TranzactieController: public Controller {
    TranzactieRepository tranzactieRepository;
public:
    explicit TranzactieController(const std::string& path, const DepoziteRepository& depozitRepository, const ProdusRepository& produsRepository);
    void handle_get(const http_request& req) override;
    void handle_post(const http_request& req) override;
    void handle_delete(const http_request& req) override;
};


#endif //OOP_TRANZACTIECONTROLLER_H
