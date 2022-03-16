//
// Created by Tudor Coman on 16.03.2022.
//

#include "../headers/Depozit.h"

void Depozit::executaTranzactie(const Tranzactie &t) {
    this->tranzactii.push_back(t);
    const int op = (t.getTip() == Tranzactie::Type::IN) ? 1 : -1;
    this->stoc[t.getProdus()] += op * t.getQuantity();
}

std::ostream &operator<<(std::ostream &os, const Depozit &d) {
    os << "Depozitul " << d.nume<< "\n"
       << "Adresa: " << d.adresa << "\n";

    os << "Stocul la zi: " << "\n";
    for(const auto& it: d.stoc) {
        os << it.first;
        os << std::fixed << std::showpoint << std::setprecision(2);
        os << "(cantitate " <<  it.second << ")\n";
    }
    return os;
}

Depozit::Depozit(std::string nume, std::string adresa, std::map<Produs, double> stoc,
                 std::vector<Tranzactie> tranzactii) : nume(std::move(nume)), adresa(std::move(adresa)), stoc(std::move(stoc)),
                                                       tranzactii(std::move(tranzactii)) {}

Depozit::Depozit(std::string nume, std::string adresa) : nume(std::move(nume)), adresa(std::move(adresa)) {}


