//
// Created by Tudor Coman on 16.03.2022.
//

#define _CRT_SECURE_NO_WARNINGS
#include <ostream>
#include "../headers/Tranzactie.h"
#include "../../infra/headers/Utilities.h"
#include <boost/date_time.hpp>

std::ostream &operator<<(std::ostream &os, const Tranzactie &tr) {
    os << "[" << Utilities::getStringFromDate(tr.timestamp) << "] "
    << "Tranzactia de " << ((tr.tip == 0) ? "Intrare" : "Iesire") << " #" << tr.id << "\n";

    os << std::fixed << std::showpoint << std::setprecision(2);
    os << tr.quantity << " x " << tr.produs.getNume() << "\n";
    return os;
}

const Produs &Tranzactie::getProdus() const {
    return produs;
}

double Tranzactie::getQuantity() const {
    return quantity;
}

Tranzactie::Type Tranzactie::getTip() const {
    return tip;
}

const boost::posix_time::ptime &Tranzactie::getTimestamp() const {
    return timestamp;
}

int Tranzactie::getId() const {
    return id;
}

web::json::value Tranzactie::getJson() const {
    using namespace web::json;
    value json;

    json[U("id")] = value::number(id);
    json[U("produs")] = produs.getJson();
    json[U("quantity")] = value::number(quantity);
    json[U("tip")] = value::string(tip == Tranzactie::Type::IN ? "IN" : "OUT");
    json[U("timestamp")] = value::string(Utilities::getStringFromDate(timestamp));
    return json;
}

void Tranzactie::fromJson(web::json::value obj) {
    if (obj.has_integer_field("id"))
        this->id = obj[U("id")].as_integer();
    this->quantity = obj[U("quantity")].as_double();
    this->tip = (obj[U("tip")].as_string() == "IN") ? Tranzactie::Type::IN : Tranzactie::Type::OUT;
    this->timestamp = boost::posix_time::time_from_string(obj[U("data_tranzactie")].as_string());
}

bool Tranzactie::operator==(const Tranzactie &rhs) const {
    std::cout << timestamp << " " << rhs.timestamp << std::endl;
    return id == rhs.id &&
           produs == rhs.produs &&
           quantity == rhs.quantity &&
           tip == rhs.tip &&
           timestamp == rhs.timestamp;
}

bool Tranzactie::operator!=(const Tranzactie &rhs) const {
    return !(rhs == *this);
}

std::shared_ptr<JsonEntity> Tranzactie::clone() const {
    return std::make_shared<Tranzactie>(*this);
}

std::ostream& operator<<(std::ostream& os, const Tranzactie::Type tip) {
    switch(tip) {
        case Tranzactie::Type::IN: os << "intrare"; return os;
        case Tranzactie::Type::OUT: os << "iesire"; return os;
    }
    return os;
}