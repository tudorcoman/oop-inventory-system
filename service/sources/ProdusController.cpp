//
// Created by Tudor Coman on 03.04.2022.
//

#include "../headers/ProdusController.h"

ProdusController::ProdusController(const std::string &path) : Controller(path) { }

void ProdusController::handle_get(const http_request &req) {
    std::map<utility::string_t, utility::string_t> http_get_vars = uri::split_query(req.request_uri().query());
    auto output = Utilities::getJsonArray<Produs>(&produsRepository, http_get_vars);
    req.reply(status_codes::OK, output);
}

void ProdusController::handle_post(const http_request &req) {
    pplx::task<web::json::value> jsonTask = req.extract_json();
    web::json::value json = jsonTask.get();
    Produs p;
    p.fromJson(json);

    try {
        produsRepository.opCreate(p);
        req.reply(status_codes::OK, "Produsul a fost creat");
    } catch(const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        throw InternalErrorException("Produsul nu a putut fi creat");
    }
}

void ProdusController::handle_put(const http_request &req) {
    pplx::task<web::json::value> jsonTask = req.extract_json();
    web::json::value json = jsonTask.get();
    Produs p;
    p.fromJson(json);

    try {
        produsRepository.opUpdate(p.getId(), p);
        req.reply(status_codes::OK, "Produsul a fost actualizat");
    } catch(const IdNotFoundException& e) {
        std::cerr << e.what() << std::endl;
        throw InternalErrorException("Produsul care trebuie actualizat nu exista");
    }
}

void ProdusController::handle_delete(const http_request &req) {
    pplx::task<web::json::value> jsonTask = req.extract_json();
    web::json::value json = jsonTask.get();
    Produs p;
    p.fromJson(json);

    try {
        const Produs pp = produsRepository.getById(p.getId());
        if (p != pp)
            req.reply(status_codes::BadRequest, "Produsul acesta nu exista");
        else {
            if (produsRepository.opDelete(p.getId())) {
                req.reply(status_codes::OK, "Produsul a fost sters");
            } else {
                req.reply(status_codes::InternalError, "Produsul nu a putut fi sters");
            }
        }
    } catch(const IdNotFoundException& e) {
        std::cerr << e.what() << std::endl;
        req.reply(status_codes::NotFound, "Produsul cu id-ul respectiv nu exista");
    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        req.reply(status_codes::InternalError, "Produsul nu a putut fi sters");
    }
}

std::shared_ptr<Controller> ProdusController::clone() const {
    return std::make_shared<ProdusController>(*this);
}
