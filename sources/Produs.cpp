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