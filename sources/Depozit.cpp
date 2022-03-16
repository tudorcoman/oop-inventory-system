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
                 std::vector<Tranzactie> tranzactii, const Angajat& manager) : nume(std::move(nume)), adresa(std::move(adresa)), stoc(std::move(stoc)),
                                                       tranzactii(std::move(tranzactii)), manager(manager) {}

Depozit::Depozit(std::string nume, std::string adresa, const Angajat& manager) : nume(std::move(nume)), adresa(std::move(adresa)), manager(manager) {}

std::vector<Tranzactie> Depozit::getTranzactii(const Tranzactie::Type &tip) {
    std::vector<Tranzactie> ans;
    std::copy_if(tranzactii.begin(), tranzactii.end(),
                 std::back_inserter(ans),
                 [&](const Tranzactie& t) { return t.getTip() == tip; });
    return ans;
}

std::vector<Tranzactie> Depozit::getTranzactii(const Timestamp &left, const Timestamp &right) {
    std::vector<Tranzactie> ans;
    std::copy_if(tranzactii.begin(), tranzactii.end(),
                 std::back_inserter(ans),
                 [&](const Tranzactie& tranzactie) { return left <= tranzactie.getTimestamp() && tranzactie.getTimestamp() <= right; });
    return ans;
}


