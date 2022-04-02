//
// Created by Tudor Coman on 16.03.2022.
//

#ifndef OOP_ANGAJAT_H
#define OOP_ANGAJAT_H

#include <string>
#include <memory>
#include "cpprest/json.h"

class Angajat {
    std::string first_name, last_name;
    long long cnp;
    std::shared_ptr<Angajat> manager;

    web::json::value getSimpleJson() const;
public:
    explicit Angajat(std::string firstName, std::string lastName, long long int cnp, std::shared_ptr<Angajat> manager);

    Angajat(const Angajat& other);

    friend std::ostream& operator << (std::ostream& os, const Angajat& a);

    Angajat& operator =(const Angajat& other);

    ~Angajat();

    bool operator==(const Angajat &rhs) const;

    bool operator!=(const Angajat &rhs) const;

    void setManager(std::shared_ptr<Angajat> mgr);

    [[nodiscard]] web::json::value getJson() const;

    static Angajat fromJson(web::json::value obj);

    [[nodiscard]] const std::string &getFirstName() const;

    [[nodiscard]] const std::string &getLastName() const;

    [[nodiscard]] long long int getCnp() const;

    [[nodiscard]] const std::shared_ptr<Angajat> &getManager() const;
};
#endif //OOP_ANGAJAT_H
