//
// Created by Tudor Coman on 16.03.2022.
//

#include "../headers/Produs.h"
#include <ostream>

Produs::Produs(int id, std::string nume, std::string categorie, double pretCumparare, double pretVanzare) : id(id), nume(std::move(nume)), categorie(std::move(categorie)), pretCumparare(pretCumparare), pretVanzare(pretVanzare) {}

std::ostream &operator<<(std::ostream &os, const Produs &p) {
    os << "Produs #" << p.id << ": " << p.nume << "\n"
       << "Categoria: " << p.categorie << "\n"
       << "Pretul de cumparare: " << p.pretCumparare << " RON\n"
       << "Pretul de vanzare: " << p.pretVanzare << " RON\n";
    return os;
}

const std::string &Produs::getNume() const {
    return nume;
}

double Produs::getPretCumparare() const {
    return pretCumparare;
}

double Produs::getPretVanzare() const {
    return pretVanzare;
}

bool Produs::operator<(const Produs &other) const {
    return id < other.id;
}

const std::string &Produs::getCategorie() const {
    return categorie;
}

int Produs::getId() const {
    return id;
}

bool Produs::operator==(const Produs &rhs) const {
    return id == rhs.id &&
           nume == rhs.nume &&
           categorie == rhs.categorie &&
           pretCumparare == rhs.pretCumparare &&
           pretVanzare == rhs.pretVanzare;
}

bool Produs::operator!=(const Produs &rhs) const {
    return !(rhs == *this);
}

web::json::value Produs::getJson() const {
    using namespace web::json;
    value json;
    json[U("id")] = value::number(id);
    json[U("name")] = value::string(U(nume));
    json[U("category")] = value::string(U(categorie));
    json[U("buying_price")] = value::number(pretCumparare);
    json[U("selling_price")] = value::number(pretVanzare);
    return json;
}

void Produs::fromJson(web::json::value obj) {
    if (obj.has_integer_field("id"))
        this->id = obj[U("id")].as_integer();
    else
        this->id = 0;
    this->nume = obj[U("name")].as_string();
    this->categorie = obj[U("category")].as_string();
    this->pretCumparare = obj[U("buying_price")].as_double();
    this->pretVanzare = obj[U("selling_price")].as_double();
}


Produs &Produs::operator=(Produs other) {
    swap(*this, other);
    return *this;
}

void swap(Produs &p1, Produs &p2) {
    std::swap(p1.id, p2.id);
    std::swap(p1.nume, p2.nume);
    std::swap(p1.categorie, p2.categorie);
    std::swap(p1.pretCumparare, p2.pretCumparare);
    std::swap(p1.pretVanzare, p2.pretVanzare);
}

std::shared_ptr<JsonEntity> Produs::clone() const {
    return std::make_shared<Produs>(*this);
}
