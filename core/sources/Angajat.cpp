//
// Created by Tudor Coman on 16.03.2022.
//

#include "../headers/Angajat.h"
#include <iostream>
#include <memory>
#include <utility>
#include "cpprest/json.h"
#include "../../infra/headers/Utilities.h"

Angajat::Angajat(const Angajat &other) : first_name(other.first_name), last_name(other.last_name), cnp(other.cnp), manager(other.manager) {
    if (Utilities::containsNonAlphaChars(first_name) || Utilities::containsNonAlphaChars(last_name)) {
        throw IllegalArgumentException();
    }
}

std::ostream &operator<<(std::ostream &os, const Angajat &a) {
    os << "Angajat " << a.first_name << " " << a.last_name << "\n"
       << "CNP " << a.cnp << "\n";

    if (a.manager != nullptr) {
        os << "Manager: " << a.manager->first_name << " " << a.manager->last_name << "\n";
    }
    return os;
}

Angajat &Angajat::operator=(Angajat other) {
    std::swap(*this, other);
    return *this;
}

void Angajat::setManager(std::shared_ptr<Angajat> mgr) {
    this->manager = std::move(mgr);
}

web::json::value Angajat::getSimpleJson() const {
    using namespace web::json;
    value json;
    json[U("first_name")] = value::string(utility::conversions::to_string_t(first_name));
    json[U("last_name")] = value::string(utility::conversions::to_string_t(last_name));
    json[U("cnp")] = value::number((int64_t) cnp);
    return json;
}

web::json::value Angajat::getJson() const {
    web::json::value json = getSimpleJson();
    if (manager) {
        json[U("manager")] = manager->getSimpleJson();
    } else {
        json[U("manager")] = web::json::value();
    }
    return json;
}

void Angajat::fromJson(web::json::value obj) {
    this->first_name = utility::conversions::to_utf8string(obj[U("first_name")].as_string());
    this->last_name = utility::conversions::to_utf8string(obj[U("last_name")].as_string());
    this->cnp = obj[U("cnp")].as_number().to_int64();
}

bool Angajat::operator==(const Angajat &rhs) const {
    return first_name == rhs.first_name &&
           last_name == rhs.last_name &&
           cnp == rhs.cnp;
}

bool Angajat::operator!=(const Angajat &rhs) const {
    return !(rhs == *this);
}

const std::string &Angajat::getFirstName() const {
    return first_name;
}

const std::string &Angajat::getLastName() const {
    return last_name;
}

long long int Angajat::getCnp() const {
    return cnp;
}

const std::shared_ptr<Angajat> &Angajat::getManager() const {
    return manager;
}

Angajat::Angajat(std::string firstName, std::string lastName, long long int cnp, std::shared_ptr<Angajat> manager ):
    first_name(std::move(firstName)), last_name(std::move(lastName)), cnp(cnp), manager(std::move(manager)) { }

void swap(Angajat& obj1, Angajat& obj2) {
    std::swap(obj1.first_name, obj2.first_name);
    std::swap(obj1.last_name, obj2.last_name);
    std::swap(obj1.cnp, obj2.cnp);
    std::swap(obj1.manager, obj2.manager);
}

std::shared_ptr<JsonEntity> Angajat::clone() const {
    return std::make_shared<Angajat>(*this);
}
