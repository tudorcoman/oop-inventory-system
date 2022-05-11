//
// Created by Tudor Coman on 05.04.2022.
//

#include "../headers/DepoziteRepository.h"
#include "../../core/headers/ObjectFactory.h"

#include <utility>

bool DepoziteRepository::prepared_statements;
void DepoziteRepository::_fetch_objects() {
    if(!fetched_objects) {
        result res = CrudRepository<Depozit>::_run_select("SELECT * FROM " + getTable());
        for (result::const_iterator it = res.begin(); it != res.end(); it ++) {
            int id = it[0].as<int>(), mgr_id = it[3].as<int>();
            Depozit d = ObjectFactory::depozitFromResult(it, std::make_shared<Angajat>(angajatRepository.getById(mgr_id)));
            depozite.insert({id, d});
        }
        fetched_objects = true;
    }
}

DepoziteRepository::DepoziteRepository(const AngajatRepository& angajatRepository) : CrudRepository<Depozit>("depozite", std::vector<TableField> {TableField("id", TableField::INT),
                                                                                                                                            TableField("nume", TableField::TEXT),
                                                                                                                                            TableField("adresa", TableField::TEXT),
                                                                                                                                            TableField("manager", TableField::INT)}),
                                                                                angajatRepository(angajatRepository) {
    if(!prepared_statements) {
        prepared_statements = true;
        prepareStatement("depozite_create_insert", "INSERT INTO " + getTable() + " (nume, address, manager) VALUES ($1, $2, $3)");
        prepareStatement("depozite_create_select", "SELECT id FROM " + getTable() + " WHERE nume = $1 AND address = $2 AND manager = $3");
        prepareStatement("depozite_update", "UPDATE " + getTable() + " SET nume=$1, address=$2, manager=$3 WHERE id=$4");
    }
}

bool DepoziteRepository::opCreate(const Depozit &d) {
    if(!fetched_objects) {
        _fetch_objects();
    }
    try {
        const int mgr_id = angajatRepository.findAngajat(*d.getManager());
        executePrepared("depozite_create_insert", d.getNume(), d.getAdresa(), mgr_id);

        result r = getTransaction().exec_prepared("depozite_create_select", d.getNume(), d.getAdresa(), mgr_id);
        const int id = r.begin()[0].as<int>();
        depozite.insert({id, d});

        return true;
    } catch(const std::exception& e) {
        return false;
    }
}

std::vector<Depozit> DepoziteRepository::opRetrieve(std::map<std::string, std::string> filters) {
    if (!fetched_objects) {
        _fetch_objects();
    }

    const std::string where_clause = _build_where_clause(filters);
    const std::string query = "SELECT id FROM " + getTable() + " " + where_clause;
    const result res = CrudRepository::_run_select(query);

    std::vector<Depozit> answer;
    for (result::const_iterator it = res.begin(); it != res.end(); it ++) {
        int id = it[0].as<int>();
        answer.push_back(depozite.at(id));
    }

    return answer;
}

bool DepoziteRepository::opUpdate(const int &id, const Depozit &d) {
    if (!fetched_objects) {
        _fetch_objects();
    }

    const Depozit dep = getById(id);
    try {
        depozite.erase(depozite.find(id));
        depozite.insert({id, d});

        executePrepared("depozite_update", d.getNume(), d.getAdresa(), angajatRepository.findAngajat(*d.getManager()));
        return true;
    } catch(const std::exception& e) {
        return false;
    }
}

bool DepoziteRepository::opDelete(const int &id) {
    if(!fetched_objects) {
        _fetch_objects();
    }
    try {
        const std::string query = "DELETE FROM " + getTable() + " WHERE id = " + std::to_string(id);
        CrudRepository<Depozit>::_run_working_query(query);
        return true;
    } catch(const std::exception& e) {
        return false;
    }
}

Depozit DepoziteRepository::getById(const int &id) {
    if (!fetched_objects) {
        _fetch_objects();
    }
    if(!depozite.contains(id))
        throw IdNotFoundException("DepoziteRespository");
    return depozite.at(id);
}

int DepoziteRepository::findDepozit(const Depozit &d) {
    if(!fetched_objects) {
        _fetch_objects();
    }
    for (const auto& it: depozite)
        if (it.second.getAdresa() == d.getAdresa() && it.second.getNume() == d.getNume())
            return it.first;
    return -1;
}

DepoziteRepository &DepoziteRepository::operator=(const DepoziteRepository &other) {
    CrudRepository<Depozit>::operator =(other);
    depozite = other.depozite;
    angajatRepository = other.angajatRepository;
    return *this;
}

std::shared_ptr<Repository<Depozit>> DepoziteRepository::clone() const {
    return std::make_shared<DepoziteRepository>(*this);
}

DepoziteRepository::DepoziteRepository(const DepoziteRepository &other): CrudRepository<Depozit>(other) {
    depozite = other.depozite;
    angajatRepository = other.angajatRepository;
}
