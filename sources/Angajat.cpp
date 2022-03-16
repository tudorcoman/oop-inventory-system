//
// Created by Tudor Coman on 16.03.2022.
//

#include "../headers/Angajat.h"
#include <iostream>
#include <memory>

Angajat::Angajat(const Angajat &other) : first_name(other.first_name), last_name(other.last_name), cnp(other.cnp), manager(other.manager) {
    std::cout << "constr copiere Angajat " << first_name + " " + last_name << "\n";
}

Angajat::Angajat(std::string firstName, std::string lastName, long long int cnp, std::shared_ptr<Angajat> manager) : first_name(std::move(firstName)), last_name(std::move(lastName)), cnp(cnp), manager(std::move(manager)) {}

std::ostream &operator<<(std::ostream &os, const Angajat &a) {
    os << "Angajat " << a.first_name << " " << a.last_name << "\n"
       << "CNP " << a.cnp << "\n";

    if (a.manager != nullptr) {
        os << "Manager: " << a.manager->first_name << " " << a.manager->last_name << "\n";
    }
    return os;
}

Angajat &Angajat::operator=(const Angajat &other) {
    first_name = other.first_name;
    last_name = other.last_name;
    manager = other.manager;
    cnp = other.cnp;
    std::cout << "op= Angajat " << first_name + " " + last_name << "\n";
    return *this;
}

Angajat::~Angajat() {
    std::cout << "destr Angajat " << first_name + " " + last_name << "\n";
}

void Angajat::setManager(std::shared_ptr<Angajat> mgr) {
    this->manager = std::move(mgr);
}