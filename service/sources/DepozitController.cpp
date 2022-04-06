//
// Created by Tudor Coman on 05.04.2022.
//

#include "../headers/DepozitController.h"

#include <utility>

void DepozitController::handle_get(const http_request &req) {
    std::map<std::string, std::string> http_get_vars = uri::split_query(req.request_uri().query());
    auto output = Utilities::getJsonArray<Depozit>(&depoziteRepository, http_get_vars);
    req.reply(status_codes::OK, output);
}

void DepozitController::handle_post(const http_request &req) {
    pplx::task<web::json::value> jsonTask = req.extract_json();
    web::json::value json = jsonTask.get();
    Depozit d;
    d.fromJson(json);
    int mgr_id = -1;

    try {
        mgr_id = json[U("manager")].as_integer();
        Angajat manager = depoziteRepository.angajatRepository.getById(mgr_id);
        d.setManager(std::make_shared<Angajat>(manager));
    } catch(const std::exception& e) {
        req.reply(status_codes::InternalError, "bad manager id");
    }

    if (json.has_number_field("id")) {
        // up[date operation
        const int id = json[U("id")].as_integer();
        std::string message = "Depozitul cu id = " + std::to_string(id);
        if (depoziteRepository.opUpdate(id, d)) {
            req.reply(status_codes::OK, message + " a fost actualizat");
        } else {
            req.reply(status_codes::InternalError, message + " nu a putut fi actualizat");
        }
    } else {
        // create operation
        if (depoziteRepository.opCreate(d)) {
            req.reply(status_codes::OK, "Depozit creat");
        } else {
            req.reply(status_codes::InternalError, "Depozitul nu a putut fi creat");
        }
    }
}

void DepozitController::handle_delete(const http_request &req) {
    pplx::task<web::json::value> jsonTask = req.extract_json();
    web::json::value json = jsonTask.get();
    Depozit d;
    d.fromJson(json);
    int mgr_id = -1;

    try {
        mgr_id = json[U("manager")].as_integer();
        Angajat manager = depoziteRepository.angajatRepository.getById(mgr_id);
        d.setManager(std::make_shared<Angajat>(manager));
    } catch(const std::exception& e) {
        req.reply(status_codes::InternalError, "bad manager id");
    }

    int dep_id = depoziteRepository.findDepozit(d);
    if (dep_id != -1 && depoziteRepository.opDelete(dep_id)) {
        req.reply(status_codes::OK, "Depozitul a fost sters");
    } else {
        req.reply(status_codes::InternalError, "Depozitul nu a putut fi sters");
    }
}

DepozitController::DepozitController(const std::string &path, AngajatRepository angajatRepository): Controller(path), depoziteRepository(std::move(angajatRepository)) { }
