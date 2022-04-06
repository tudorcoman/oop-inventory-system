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

class Depozit: public JsonEntity {
    std::string nume;
    std::string adresa;
    std::map<Produs, double> stoc;
    std::vector<Tranzactie> tranzactii;
    std::shared_ptr<Angajat> manager;
public:
    Depozit();

    Depozit(std::string nume, std::string adresa, std::shared_ptr<Angajat> manager);

    explicit Depozit(std::string nume, std::string adresa, std::map<Produs, double> stoc,
                     std::vector<Tranzactie> tranzactii, std::shared_ptr<Angajat> manager);

    [[nodiscard]] const std::string &getNume() const;

    [[nodiscard]] const std::string &getAdresa() const;

    [[nodiscard]] const std::map<Produs, double> &getStoc() const;

    [[nodiscard]] const std::vector<Tranzactie> &getTranzactii() const;

    const std::shared_ptr<Angajat> &getManager() const;

    void executaTranzactie(const Tranzactie& t);

    friend std::ostream& operator << (std::ostream& os, const Depozit& d);

    std::vector<Tranzactie> getTranzactii(const Tranzactie::Type& tip) const;

    std::vector<Tranzactie> getTranzactii(const boost::posix_time::ptime &left, const boost::posix_time::ptime &right) const;

    web::json::value getJson() const override;
    void fromJson(web::json::value obj) override;

    void setManager(const std::shared_ptr<Angajat> &manager);
};

#endif //OOP_DEPOZIT_H
