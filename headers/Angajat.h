//
// Created by Tudor Coman on 16.03.2022.
//

#ifndef OOP_ANGAJAT_H
#define OOP_ANGAJAT_H

#include <string>
#include <memory>

class Angajat {
    std::string first_name, last_name;
    long long cnp;
    std::shared_ptr<Angajat> manager;
public:
    explicit Angajat(std::string firstName, std::string lastName, long long int cnp, std::shared_ptr<Angajat> manager);

    Angajat(const Angajat& other);

    friend std::ostream& operator << (std::ostream& os, const Angajat& a);

    Angajat& operator =(const Angajat& other);

    ~Angajat();

    void setManager(std::shared_ptr<Angajat> mgr);
};
#endif //OOP_ANGAJAT_H
