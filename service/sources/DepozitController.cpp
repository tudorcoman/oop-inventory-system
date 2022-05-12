//
// Created by Tudor Coman on 05.04.2022.
//

#include "../headers/DepozitController.h"

#include <utility>

void DepozitController::handle_get(const http_request &req) {
    std::map<utility::string_t, utility::string_t> http_get_vars = uri::split_query(req.request_uri().query());
    auto output = Utilities::getJsonArray<Depozit>(&depoziteRepository, http_get_vars);
    req.reply(status_codes::OK, output);
}

void DepozitController::handle_post(const http_request &req) {
    pplx::task<web::json::value> jsonTask = req.extract_json();
    web::json::value json = jsonTask.get();
    Depozit d;
    d.fromJson(json);

    try {
        int mgr_id = json[U("manager")].as_integer();
        Angajat manager = depoziteRepository.angajatRepository.getById(mgr_id);
        d.setManager(std::make_shared<Angajat>(manager));
    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        req.reply(status_codes::BadRequest, "bad manager id");
    }

    if (depoziteRepository.opCreate(d)) {
        req.reply(status_codes::OK, "Depozit creat");
    } else {
        throw InternalErrorException("Depozitul nu a putut fi creat");
    }
}

void DepozitController::handle_put(const http_request &req) {
    pplx::task<web::json::value> jsonTask = req.extract_json();
    web::json::value json = jsonTask.get();
    Depozit d;
    d.fromJson(json);

    try {
        int mgr_id = json[U("manager")].as_integer();
        Angajat manager = depoziteRepository.angajatRepository.getById(mgr_id);
        d.setManager(std::make_shared<Angajat>(manager));
    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        req.reply(status_codes::BadRequest, "bad manager id");
    }

    const int id = json[U("id")].as_integer();
    std::string message = "Depozitul cu id = " + std::to_string(id);
    if (depoziteRepository.opUpdate(id, d)) {
        req.reply(status_codes::OK, message + " a fost actualizat");
    } else {
        throw InternalErrorException(message + " nu a putut fi actualizat");
    }
}

void DepozitController::handle_delete(const http_request &req) {
    pplx::task<web::json::value> jsonTask = req.extract_json();
    web::json::value json = jsonTask.get();
    Depozit d;
    d.fromJson(json);

    try {
        int mgr_id = json[U("manager")].as_integer();
        Angajat manager = depoziteRepository.angajatRepository.getById(mgr_id);
        d.setManager(std::make_shared<Angajat>(manager));
    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        req.reply(status_codes::BadRequest, "bad manager id");
    }

    int dep_id = depoziteRepository.findDepozit(d);
    if (dep_id != -1 && depoziteRepository.opDelete(dep_id)) {
        req.reply(status_codes::OK, "Depozitul a fost sters");
    } else {
        throw InternalErrorException("Depozitul nu a putut fi sters");
    }
}

DepozitController::DepozitController(const std::string &path, const AngajatRepository& angajatRepository): Controller(path), depoziteRepository(angajatRepository) { }

std::shared_ptr<Controller> DepozitController::clone() const {
    return std::make_shared<DepozitController>(*this);
}
