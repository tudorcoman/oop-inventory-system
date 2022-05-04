//
// Created by Tudor Coman on 06.04.2022.
//

#include "../headers/TranzactieController.h"

TranzactieController::TranzactieController(const std::string &path, const DepoziteRepository &depozitRepository,
                                           const ProdusRepository &produsRepository): Controller(path), tranzactieRepository(depozitRepository, produsRepository) { }

void TranzactieController::handle_get(const http_request &req) {
    std::map<std::string, std::string> http_get_vars = uri::split_query(req.request_uri().query());
    auto output = Utilities::getJsonArray<Tranzactie>(&tranzactieRepository, http_get_vars);
    req.reply(status_codes::OK, output);
}

void TranzactieController::handle_post(const http_request &req) {
    pplx::task<web::json::value> jsonTask = req.extract_json();
    web::json::value json = jsonTask.get();
    const Produs produsul = tranzactieRepository.produsRepository.getById(json[U("product_id")].as_integer());
    Tranzactie t = Tranzactie(0, produsul, 0.0, Tranzactie::Type::IN, boost::posix_time::time_from_string("1970-01-01 00:00:00"));
    t.fromJson(json);

    int depozit_id = -1;
    try {
        depozit_id = json[U("depozit_id")].as_integer();
    } catch(const std::exception& e) {
        req.reply(status_codes::InternalError, "bad depozit id");
    }

    if (tranzactieRepository.opCreate(t, depozit_id)) {
        req.reply(status_codes::OK, "Tranzactie creat");
    } else {
        req.reply(status_codes::InternalError, "Tranzactia nu a putut fi creata");
    }
}

void TranzactieController::handle_delete(const http_request &req) {
    pplx::task<web::json::value> jsonTask = req.extract_json();
    web::json::value json = jsonTask.get();
    const Produs produsul = tranzactieRepository.produsRepository.getById(json[U("product_id")].as_integer());
    Tranzactie t = Tranzactie(0, produsul, 0.0, Tranzactie::Type::IN, boost::posix_time::time_from_string("1970-01-01 00:00:00"));
    t.fromJson(json);

    try {
        const Tranzactie tt = tranzactieRepository.getById(t.getId());
        if (t != tt)
            req.reply(status_codes::BadRequest, "Tranzactia aceasta nu exista");
        else {
            if (tranzactieRepository.opDelete(t.getId())) {
                req.reply(status_codes::OK, "Tranzactia a fost anulata");
            } else {
                req.reply(status_codes::InternalError, "Tranzactia nu a putut fi anulata");
            }
        }
    } catch(const std::exception& e) {
        req.reply(status_codes::NotFound, "Tranzactia cu id-ul respectiv nu exista");
    }
}

std::shared_ptr<Controller> TranzactieController::clone() const {
    return std::make_shared<TranzactieController>(*this);
}
