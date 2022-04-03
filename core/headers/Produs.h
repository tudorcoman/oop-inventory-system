//
// Created by Tudor Coman on 16.03.2022.
//

#ifndef OOP_PRODUS_H
#define OOP_PRODUS_H

#include <string>
#include "JsonEntity.h"
#include "cpprest/json.h"

class Produs: public JsonEntity {
    int id{};
    std::string nume, categorie;
    double pretCumparare{}, pretVanzare{};
public:
    Produs() = default;

    explicit Produs(int id, std::string nume, std::string categorie, double pretCumparare, double pretVanzare);
    friend std::ostream& operator << (std::ostream& os, const Produs& p);

    [[nodiscard]] int getId() const;

    [[nodiscard]] const std::string &getNume() const;

    [[nodiscard]] const std::string &getCategorie() const;

    [[nodiscard]] double getPretCumparare() const;

    [[nodiscard]] double getPretVanzare() const;

    bool operator<(const Produs &other) const;

    bool operator==(const Produs &rhs) const;

    bool operator!=(const Produs &rhs) const;

    web::json::value getJson() const override;

    void fromJson(web::json::value obj) override;

    virtual ~Produs();
};

#endif //OOP_PRODUS_H
