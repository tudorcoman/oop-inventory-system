//
// Created by Tudor Coman on 02.04.2022.
//

#include "../headers/AngajatController.h"

void AngajatController::handle_get(const http_request &req) {
    std::map<std::string, std::string> http_get_vars = uri::split_query(req.request_uri().query());
    auto output = Utilities::getJsonArray<Angajat>(&angajatRepository, http_get_vars);
    req.reply(status_codes::OK, output);
}

void AngajatController::handle_post(const http_request &req) {
    pplx::task<web::json::value> jsonTask = req.extract_json();
    web::json::value json = jsonTask.get();
    Angajat a;
    a.fromJson(json);
    int mgr_id = -1;

    try {
        mgr_id = json[U("manager_id")].as_integer();
        Angajat manager = angajatRepository.getById(mgr_id);
        a.setManager(std::make_shared<Angajat>(manager));
    } catch(const std::exception& e) {
        req.reply(status_codes::BadRequest, "bad manager id");
    }

    if (json.has_number_field("id")) {
        // update operation
        const int id = json[U("id")].as_integer();
        std::string message = "Angajatul cu id = " + std::to_string(id);
        if (angajatRepository.opUpdate(id, a, mgr_id)) {
            req.reply(status_codes::OK, message + " a fost actualizat");
        } else {
            throw InternalErrorException(message + " nu a putut fi actualizat");
        }
    } else {
        // create operation
        if (angajatRepository.opCreate(a, mgr_id)) {
            req.reply(status_codes::OK, "Angajat creat");
        } else {
            throw InternalErrorException("Angajatul nu a putut fi actualizat");
        }
    }
}

void AngajatController::handle_delete(const http_request &req) {
    pplx::task<web::json::value> jsonTask = req.extract_json();
    web::json::value json = jsonTask.get();
    Angajat a;
    a.fromJson(json);
    int mgr_id;

    try {
        mgr_id = json[U("manager_id")].as_integer();
        Angajat manager = angajatRepository.getById(mgr_id);
        a.setManager(std::make_shared<Angajat>(manager));
    } catch(const std::exception& e) {
        req.reply(status_codes::BadRequest, "bad manager id");
    }

    int ang_id = angajatRepository.findAngajat(a);
    if (ang_id != -1 && angajatRepository.opDelete(ang_id)) {
        req.reply(status_codes::OK, "Angajatul a fost sters");
    } else {
        throw InternalErrorException("Angajatul nu a putut fi sters");
    }
}

AngajatController::AngajatController(const std::string& path): Controller(path) { }

std::shared_ptr<Controller> AngajatController::clone() const {
    return std::make_shared<AngajatController>(*this);
}
