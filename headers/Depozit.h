//
// Created by Tudor Coman on 16.03.2022.
//

#ifndef OOP_DEPOZIT_H
#define OOP_DEPOZIT_H

#include <string>
#include <vector>
#include <map>
#include "Produs.h"
#include "Tranzactie.h"
#include "Angajat.h"

using Timestamp = std::chrono::time_point<std::chrono::system_clock>;

class Depozit {
    std::string nume;
    std::string adresa;
    std::map<Produs, double> stoc;
    std::vector<Tranzactie> tranzactii;
    Angajat manager;
public:
    Depozit(std::string nume, std::string adresa, const Angajat& manager);

    explicit Depozit(std::string nume, std::string adresa, std::map<Produs, double> stoc,
                     std::vector<Tranzactie> tranzactii, const Angajat& manager);
    // momentan sunt functii neutilizate asa ca le-am comentat pentru a trece Cppcheck:
    /*[[nodiscard]] const std::string &getNume() const {
        return nume;
    }

    [[nodiscard]] const std::string &getAdresa() const {
        return adresa;
    }

    [[nodiscard]] const std::map<Produs, double> &getStoc() const {
        return stoc;
    }

    [[nodiscard]] const std::vector<Tranzactie> &getTranzactii() const {
        return tranzactii;
    }*/

    void executaTranzactie(const Tranzactie& t);

    friend std::ostream& operator << (std::ostream& os, const Depozit& d);

    std::vector<Tranzactie> getTranzactii(const Tranzactie::Type& tip) const;

    std::vector<Tranzactie> getTranzactii(const Timestamp& left, const Timestamp& right) const;

};

#endif //OOP_DEPOZIT_H
