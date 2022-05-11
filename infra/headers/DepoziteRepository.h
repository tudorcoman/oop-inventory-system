//
// Created by Tudor Coman on 05.04.2022.
//

#ifndef OOP_DEPOZITEREPOSITORY_H
#define OOP_DEPOZITEREPOSITORY_H


#include "../../core/headers/Depozit.h"
#include "../../core/headers/CrudRepository.h"
#include "AngajatRepository.h"

class DepoziteRepository: public CrudRepository<Depozit> {
    void _fetch_objects() override;
    std::map<int, Depozit> depozite;
    AngajatRepository angajatRepository;
    static bool prepared_statements;
    friend class DepozitController;
public:
    explicit DepoziteRepository(const AngajatRepository&  angajatRepository);
    DepoziteRepository(const DepoziteRepository& other);

    [[nodiscard]] std::shared_ptr<Repository<Depozit>> clone() const override;
    bool opCreate(const Depozit& d) override;
    std::vector<Depozit> opRetrieve(std::map<std::string, std::string> filters) override;
    bool opUpdate(const int& id, const Depozit& d) override;
    bool opDelete(const int& id) override;
    Depozit getById(const int& id) override;
    int findDepozit(const Depozit& d);

    DepoziteRepository& operator = (const DepoziteRepository& other);
    ~DepoziteRepository() = default;
};


#endif //OOP_DEPOZITEREPOSITORY_H
