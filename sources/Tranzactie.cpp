//
// Created by Tudor Coman on 16.03.2022.
//

#include <ostream>
#include "../headers/Tranzactie.h"

std::ostream &operator<<(std::ostream &os, const Tranzactie &tr) {
    os << "Tranzactia de " << tr.tip << " #" << tr.id << "\n";
    os << std::fixed << std::showpoint << std::setprecision(2);
    os << tr.quantity << " x " << tr.produs.getNume() << "\n";
    //os << "Detalii produs: " << tr.produs;
    return os;
}

const Produs &Tranzactie::getProdus() const {
    return produs;
}

double Tranzactie::getQuantity() const {
    return quantity;
}

Tranzactie::Type Tranzactie::getTip() const {
    return tip;
}

std::ostream& operator<<(std::ostream& os, const Tranzactie::Type tip) {
    switch(tip) {
        case Tranzactie::Type::IN: os << "intrare"; return os;
        case Tranzactie::Type::OUT: os << "iesire"; return os;
    }
    return os;
}