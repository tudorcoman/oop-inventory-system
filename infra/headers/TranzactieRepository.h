//
// Created by Tudor Coman on 05.04.2022.
//

#ifndef OOP_TRANZACTIEREPOSITORY_H
#define OOP_TRANZACTIEREPOSITORY_H


#include "../../core/headers/Tranzactie.h"
#include "../../core/headers/CrudRepository.h"
#include "DepoziteRepository.h"
#include "ProdusRepository.h"

class TranzactieRepository: public CrudRepository<Tranzactie, int> {
    void _fetch_objects() override;
    std::vector<Tranzactie> tranzactii;
    DepoziteRepository depoziteRepository;
    ProdusRepository produsRepository;
    friend class TranzactieController;
    std::vector<Tranzactie> _build_from_result (const result& res, bool execute_transactions);
public:
    TranzactieRepository(DepoziteRepository depoziteRepository, ProdusRepository produsRepository);

    bool opCreate(const Tranzactie& t, int depozit_id) override;
    std::vector<Tranzactie> opRetrieve(std::map<std::string, std::string> filters) override;
    bool opUpdate(const int& id, const Tranzactie& t, int depozit_id) override;
    bool opDelete(const int& id) override;
    Tranzactie getById(const int& id) override;
};


#endif //OOP_TRANZACTIEREPOSITORY_H
