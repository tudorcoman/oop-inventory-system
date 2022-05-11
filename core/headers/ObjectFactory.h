//
// Created by Tudor Coman on 11.05.2022.
//

#ifndef OOP_OBJECTFACTORY_H
#define OOP_OBJECTFACTORY_H

#include <utility>

#include "Angajat.h"
#include "Repository.h"

class ObjectFactory {
public:
    static Angajat angajatFromResult(const result::const_iterator& it, std::shared_ptr<Angajat> manager) {
        return Angajat(it[1].as<std::string>(), it[2].as<std::string>(), it[3].as<long long>(), std::move(manager));
    }

    static Produs produsFromResult(const result::const_iterator& it) {
        auto id = it[0].as<int>();
        auto name = it[1].as<std::string>(),
                category = it[2].as<std::string>();
        auto pretCumparare = it[3].as<double>(),
                pretVanzare = it[4].as<double>();
        return Produs(id, name, category, pretCumparare, pretVanzare);
    }

    static Depozit depozitFromResult(const result::const_iterator& it, std::shared_ptr<Angajat> manager) {
        return {it[1].as<std::string>(), it[2].as<std::string>(), std::move(manager)};
    }
};
#endif //OOP_OBJECTFACTORY_H
