//
// Created by Tudor Coman on 16.03.2022.
//

#include <ostream>
#include "../headers/Tranzactie.h"

std::ostream &operator<<(std::ostream &os, const Tranzactie &tr) {
    const std::time_t t_c = std::chrono::system_clock::to_time_t(tr.timestamp);
    os << "[" << std::put_time(std::localtime(&t_c), "%F %T") << "] "
    << "Tranzactia de " << ((tr.tip == 0) ? "Intrare" : "Iesire") << " #" << tr.id << "\n";

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

//const std::chrono::time_point<std::chrono::system_clock> &Tranzactie::getTimestamp() const {
//    return timestamp;
//}

std::ostream& operator<<(std::ostream& os, const Tranzactie::Type tip) {
    switch(tip) {
        case Tranzactie::Type::IN: os << "intrare"; return os;
        case Tranzactie::Type::OUT: os << "iesire"; return os;
    }
    return os;
}