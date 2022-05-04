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

    [[nodiscard]] std::shared_ptr<JsonEntity> clone() const override;

    [[nodiscard]] const std::string &getNume() const;

    [[nodiscard]] const std::string &getAdresa() const;

    [[nodiscard]] const std::map<Produs, double> &getStoc() const;

    [[nodiscard]] const std::vector<Tranzactie> &getTranzactii() const;

    [[nodiscard]] const std::shared_ptr<Angajat> &getManager() const;

    friend void swap(Depozit& d1, Depozit& d2);
    Depozit& operator = (Depozit other);

    void executaTranzactie(const Tranzactie& t);

    friend std::ostream& operator << (std::ostream& os, const Depozit& d);

    [[nodiscard]] std::vector<Tranzactie> getTranzactii(const Tranzactie::Type& tip) const;

    [[nodiscard]] std::vector<Tranzactie> getTranzactii(const boost::posix_time::ptime &left, const boost::posix_time::ptime &right) const;

    [[nodiscard]] web::json::value getJson() const override;
    void fromJson(web::json::value obj) override;

    void setManager(const std::shared_ptr<Angajat> &manager);

    ~Depozit() override = default;
};

#endif //OOP_DEPOZIT_H
