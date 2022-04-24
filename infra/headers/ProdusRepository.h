//
// Created by Tudor Coman on 03.04.2022.
//

#ifndef OOP_PRODUSREPOSITORY_H
#define OOP_PRODUSREPOSITORY_H

#include "../../core/headers/Produs.h"
#include "../../core/headers/CrudRepository.h"

class ProdusRepository: public CrudRepository<Produs> {
    void _fetch_objects() override;
    static std::vector<Produs> _build_from_result(const result& res);
    std::vector<Produs> produse;
public:
    ProdusRepository();

    bool opCreate(const Produs& p) override;
    std::vector<Produs> opRetrieve(std::map<std::string, std::string> filters) override;
    bool opUpdate(const int& id, const Produs& p) override;
    bool opDelete(const int& id) override;
    Produs getById(const int& id) override;

    ProdusRepository& operator = (const ProdusRepository& other);
    ~ProdusRepository() = default;
};


#endif //OOP_PRODUSREPOSITORY_H
