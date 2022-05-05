//
// Created by Tudor Coman on 03.04.2022.
//

#include "../headers/AngajatRepository.h"

void AngajatRepository::_fetch_objects() {
    if (!fetched_objects) {
        result res = CrudRepository<Angajat, int>::_run_select("SELECT * FROM " + getTable());
        for (result::const_iterator it = res.begin(); it != res.end(); it++) {
            int id = it[0].as<int>();
            int mgr_id = (it[4].is_null()) ? 0 : it[4].as<int>();
            if (mgr_id == 0) {
                Angajat a(it[1].as<std::string>(), it[2].as<std::string>(), it[3].as<long long>(),
                          std::shared_ptr<Angajat>());
                angajati.insert({id, a});
            }
        }
        for (result::const_iterator it = res.begin(); it != res.end(); it++) {
            const int id = it[0].as<int>();
            const int mgr_id = (it[4].is_null()) ? 0 : it[4].as<int>();
            if (mgr_id != 0) {
                Angajat a(it[1].as<std::string>(), it[2].as<std::string>(), it[3].as<long long>(),
                          std::make_shared<Angajat>(angajati.at(mgr_id)));
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
        char buffer[1024];

        const std::string query_format = "INSERT INTO " + getTable() + " (first_name, last_name, cnp, manager_id) VALUES ('%s', '%s', %lld, %d)";
        sprintf(buffer, query_format.c_str(), a.getFirstName().c_str(), a.getLastName().c_str(), a.getCnp(), manager_id);
        const std::string query = std::string(buffer);

        buffer[0] = 0;
        const std::string afla_id_query_format = "SELECT id FROM "+ getTable() +" WHERE cnp = %lld AND first_name = '%s' AND last_name = '%s'";
        sprintf(buffer, afla_id_query_format.c_str(), a.getCnp(), a.getFirstName().c_str(), a.getLastName().c_str());
        const std::string afla_id_query = std::string(buffer);

        CrudRepository<Angajat, int>::_run_working_query(query);

        result r = CrudRepository::_run_select(afla_id_query);
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
        char buffer[1024];
        angajati.erase(angajati.find(id));
        angajati.insert({id, a});

        const std::string query_format = "UPDATE " + getTable() + " SET first_name='%s', last_name='%s', cnp=%lld, manager_id=%d WHERE id=%d";
        sprintf(buffer, query_format.c_str(), a.getFirstName().c_str(), a.getLastName().c_str(), a.getCnp(), manager_id, id);
        const std::string query = std::string(buffer);

        CrudRepository<Angajat, int>::_run_working_query(query);

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
                                                                         TableField("manager_id", TableField::INT)}) {}

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
