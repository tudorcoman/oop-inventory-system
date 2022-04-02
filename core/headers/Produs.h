//
// Created by Tudor Coman on 16.03.2022.
//

#ifndef OOP_PRODUS_H
#define OOP_PRODUS_H

#include <string>

class Produs {
    int id{};
    std::string nume, categorie;
    double pretCumparare{}, pretVanzare{};
public:
    Produs() = default;

    explicit Produs(int id, std::string nume, std::string categorie, double pretCumparare, double pretVanzare);
    friend std::ostream& operator << (std::ostream& os, const Produs& p);

    /*
    [[nodiscard]] int getId() const {
        return id;
    }
    */

    [[nodiscard]] const std::string &getNume() const;

    /*
    [[nodiscard]] const std::string &getCategorie() const {
        return categorie;
    }
    */

    [[nodiscard]] double getPretCumparare() const;

    [[nodiscard]] double getPretVanzare() const;

    bool operator<(const Produs &other) const;
};

#endif //OOP_PRODUS_H
