//
// Created by Tudor Coman on 16.03.2022.
//

#include "../headers/Depozit.h"
#include "../../infra/headers/Utilities.h"

void Depozit::executaTranzactie(const Tranzactie &t) {
    this->tranzactii.push_back(t);
    const int op = (t.getTip() == Tranzactie::Type::IN) ? 1 : -1;
    this->stoc[t.getProdus()] += op * t.getQuantity();
}

std::ostream &operator<<(std::ostream &os, const Depozit &d) {
    os << "Depozitul " << d.nume<< "\n"
       << "Adresa: " << d.adresa << "\n"
       << "Administrat de: " << d.manager << "\n";

    os << "Stocul la zi: " << "\n";
    for(const auto& it: d.stoc) {
        os << it.first;
        os << std::fixed << std::showpoint << std::setprecision(2);
        os << "(cantitate " <<  it.second << ")\n";
    }
    return os;
}

Depozit::Depozit(std::string nume, std::string adresa, std::map<Produs, double> stoc,
                 std::vector<Tranzactie> tranzactii, std::shared_ptr<Angajat> manager) : nume(std::move(nume)), adresa(std::move(adresa)), stoc(std::move(stoc)),
                                                       tranzactii(std::move(tranzactii)), manager(std::move(manager)) {}

Depozit::Depozit(std::string nume, std::string adresa, std::shared_ptr<Angajat> manager) : nume(std::move(nume)), adresa(std::move(adresa)), manager(std::move(manager)) {}

std::vector<Tranzactie> Depozit::getTranzactii(const Tranzactie::Type &tip) const {
    std::vector<Tranzactie> ans;
    std::copy_if(tranzactii.begin(), tranzactii.end(),
                 std::back_inserter(ans),
                 [&](const Tranzactie& t) { return t.getTip() == tip; });
    return ans;
}

std::vector<Tranzactie> Depozit::getTranzactii(const boost::posix_time::ptime &left, const boost::posix_time::ptime &right) const {
    std::vector<Tranzactie> ans;
    std::copy_if(tranzactii.begin(), tranzactii.end(),
                 std::back_inserter(ans),
                 [&](const Tranzactie& tranzactie) { return left <= tranzactie.getTimestamp() && tranzactie.getTimestamp() <= right; });
    return ans;
}

const std::string &Depozit::getNume() const {
    return nume;
}

const std::string &Depozit::getAdresa() const {
    return adresa;
}

const std::map<Produs, double> &Depozit::getStoc() const {
    return stoc;
}

const std::vector<Tranzactie> &Depozit::getTranzactii() const {
    return tranzactii;
}

const std::shared_ptr<Angajat> &Depozit::getManager() const {
    return manager;
}

web::json::value Depozit::getJson() const {
    using namespace web::json;
    value json;
    json[U("nume")] = value::string(U(nume));
    json[U("adresa")] = value::string(U(adresa));
    json[U("manager")] = manager->getJson();
    json[U("stoc")] = value::array();
    int i = 0;
    for(const auto& it: stoc) {
        value obiect = it.first.getJson();
        obiect[U("quantity")] = value::number(it.second);
        json[U("stoc")][i ++] = obiect;
    }
    json[U("tranzactii")] = value::array();
    i = 0;
    for (const auto& it: tranzactii) {
        json[U("tranzactii")][i ++] = it.getJson();
    }
    return json;
}

void Depozit::fromJson(web::json::value obj) {
    this->nume = obj[U("nume")].as_string();
    this->adresa = obj[U("adresa")].as_string();
}

Depozit::Depozit() {
    nume = "";
    adresa = "";
    manager = std::shared_ptr<Angajat>();
}

void Depozit::setManager(const std::shared_ptr<Angajat> &mgr) {
    this->manager = mgr;
}

Depozit &Depozit::operator=(Depozit other) {
    swap(*this, other);
    return *this;
}

void swap(Depozit &d1, Depozit &d2) {
    std::swap(d1.nume, d2.nume);
    std::swap(d1.adresa, d2.adresa);
    std::swap(d1.stoc, d2.stoc);
    std::swap(d1.tranzactii, d2.tranzactii);
    std::swap(d1.manager, d2.manager);
}

std::shared_ptr<JsonEntity> Depozit::clone() const {
    return std::make_shared<Depozit>(*this);
}

Depozit::Depozit(const Depozit &other) {
    nume = other.nume;
    adresa = other.adresa;
    stoc = other.stoc;
    tranzactii = other.tranzactii;
    manager = other.manager;
}



