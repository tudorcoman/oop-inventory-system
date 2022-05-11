//
// Created by Tudor Coman on 03.04.2022.
//

#include "../headers/AngajatRepository.h"
#include "../../core/headers/ObjectFactory.h"

void AngajatRepository::_fetch_objects() {
    if (!fetched_objects) {
        result res = CrudRepository<Angajat, int>::_run_select("SELECT * FROM " + getTable());
        for (result::const_iterator it = res.begin(); it != res.end(); it++) {
            int id = it[0].as<int>();
            int mgr_id = (it[4].is_null()) ? 0 : it[4].as<int>();
            if (mgr_id == 0) {
                Angajat a(ObjectFactory::angajatFromResult(it, std::shared_ptr<Angajat>()));
                angajati.insert({id, a});
            }
        }
        for (result::const_iterator it = res.begin(); it != res.end(); it++) {
            const int id = it[0].as<int>();
            const int mgr_id = (it[4].is_null()) ? 0 : it[4].as<int>();
            if (mgr_id != 0) {
                Angajat a(ObjectFactory::angajatFromResult(it, std::make_shared<Angajat>(angajati.at(mgr_id))));
                angajati.insert({id, a});
            }
        }
        fetched_objects = true;
    }
}

bool AngajatRepository::opCreate(const Angajat &a, int manager_id) {
    if(!fetched_objects) {
        _fetch_objects();
    }
    try {
        executePrepared("angajati_create_insert", a.getFirstName(), a.getLastName(), a.getCnp(), manager_id);

        result r = getTransaction().exec_prepared("angajati_create_select", a.getCnp(), a.getFirstName(), a.getLastName());
        const int id = r.begin()[0].as<int>();
        angajati.insert({id, a});

        return true;
    } catch(const std::exception& e) {
        return false;
    }
}

std::vector<Angajat> AngajatRepository::opRetrieve(std::map<std::string, std::string> filters) {
    if(!fetched_objects) {
        _fetch_objects();
    }
    const std::string where_clause = _build_where_clause(filters);
    const std::string query = "SELECT id FROM " + getTable() + " " + where_clause;
    const result res = CrudRepository::_run_select(query);

    std::vector<Angajat> answer;
    for (result::const_iterator it = res.begin(); it != res.end(); it++) {
        int id = it[0].as<int>();
        answer.push_back(angajati.at(id));
    }

    return answer;
}

bool AngajatRepository::opUpdate(const int &id, const Angajat &a, int manager_id) {
    if(!fetched_objects) {
        _fetch_objects();
    }
    try {
        angajati.erase(angajati.find(id));
        angajati.insert({id, a});

        executePrepared("angajati_update", a.getFirstName(), a.getLastName(), a.getCnp(), manager_id, id);
        return true;
    } catch(const std::exception& e) {
        return false;
    }
}


bool AngajatRepository::opDelete(const int &id) {
    if(!fetched_objects) {
        _fetch_objects();
    }
    try {
        const std::string query = "DELETE FROM " + getTable() + " WHERE id = " + std::to_string(id);
        CrudRepository<Angajat, int>::_run_working_query(query);
        return true;
    } catch(const std::exception& e) {
        return false;
    }
}

AngajatRepository::AngajatRepository() :
        CrudRepository<Angajat, int>("angajati", std::vector<TableField>{TableField("id", TableField::INT),
                                                                         TableField("first_name", TableField::TEXT),
                                                                         TableField("last_name", TableField::TEXT),
                                                                         TableField("cnp", TableField::LONG),
                                                                         TableField("manager_id", TableField::INT)}) {
    prepareStatement("angajati_create_insert", "INSERT INTO " + getTable() + " (first_name, last_name, cnp, manager_id) VALUES ($1, $2, $3, $4)");
    prepareStatement("angajati_create_select", "SELECT id FROM "+ getTable() +" WHERE cnp = $1 AND first_name = $2 AND last_name = $3");
    prepareStatement("angajati_update", "UPDATE " + getTable() + " SET first_name=$1, last_name=$2, cnp=$3, manager_id=$4 WHERE id=$5");
}

Angajat AngajatRepository::getById(const int &id) {
    if(!fetched_objects) {
        _fetch_objects();
    }
    if (!angajati.contains(id))
        throw IdNotFoundException("AngajatRepository");
    return angajati.at(id);
}

int AngajatRepository::findAngajat(const Angajat &a) {
    if(!fetched_objects) {
        _fetch_objects();
    }
    for (const auto& it: angajati)
        if (it.second == a)
            return it.first;
    return -1;
}

std::shared_ptr<Repository<Angajat>> AngajatRepository::clone() const {
    return std::make_shared<AngajatRepository>(*this);
}
