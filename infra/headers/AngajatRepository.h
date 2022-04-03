//
// Created by Tudor Coman on 03.04.2022.
//

#ifndef OOP_ANGAJATREPOSITORY_H
#define OOP_ANGAJATREPOSITORY_H


#include "../../core/headers/Angajat.h"
#include "../../core/headers/CrudRepository.h"

class AngajatRepository: public CrudRepository<Angajat, int> {
    void _fetch_objects() override;
    std::map<int, Angajat> angajati;
public:
    AngajatRepository();

    bool opCreate(const Angajat& a, int manager_id) override;
    std::vector<Angajat> opRetrieve(std::map<std::string, std::string> filters) override;
    bool opUpdate(const int& id, const Angajat& a, int manager_id) override;
    bool opDelete(const int& id) override;
    Angajat getById(const int& id) override;
    [[nodiscard]] int findAngajat(const Angajat& a);
};


#endif //OOP_ANGAJATREPOSITORY_H
