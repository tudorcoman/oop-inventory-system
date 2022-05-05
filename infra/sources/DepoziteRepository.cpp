//
// Created by Tudor Coman on 05.04.2022.
//

#include "../headers/DepoziteRepository.h"

#include <utility>

void DepoziteRepository::_fetch_objects() {
    if(!fetched_objects) {
        result res = CrudRepository<Depozit>::_run_select("SELECT * FROM " + getTable());
        for (result::const_iterator it = res.begin(); it != res.end(); it ++) {
            int id = it[0].as<int>();
            auto nume = it[1].as<std::string>(), adresa = it[2].as<std::string>();
            int mgr_id = it[3].as<int>();
            Depozit d = Depozit(nume, adresa, std::make_shared<Angajat>(angajatRepository.getById(mgr_id)));
            depozite.insert({id, d});
        }
        fetched_objects = true;
    }
}

DepoziteRepository::DepoziteRepository(const AngajatRepository& angajatRepository) : CrudRepository<Depozit>("depozite", std::vector<TableField> {TableField("id", TableField::INT),
                                                                                                                                            TableField("nume", TableField::TEXT),
                                                                                                                                            TableField("adresa", TableField::TEXT),
                                                                                                                                            TableField("manager", TableField::INT)}),
                                                                                angajatRepository(angajatRepository) { }

bool DepoziteRepository::opCreate(const Depozit &d) {
    if(!fetched_objects) {
        _fetch_objects();
    }
    try {
        char buffer[1024];

        const int mgr_id = angajatRepository.findAngajat(*d.getManager());
        const std::string query_format =
                "INSERT INTO " + getTable() + " (nume, adresa, manager) VALUES ('%s', '%s', %d)";
        sprintf(buffer, query_format.c_str(), d.getNume().c_str(), d.getAdresa().c_str(), mgr_id);
        const std::string query(buffer);

        buffer[0] = 0;
        const std::string afla_id_query_format =
                "SELECT id FROM " + getTable() + " WHERE nume = '%s' AND adresa = '%s' AND manager = %d";
        sprintf(buffer, afla_id_query_format.c_str(), d.getNume().c_str(), d.getAdresa().c_str(), mgr_id);
        const std::string afla_id_query = std::string(buffer);

        CrudRepository<Depozit>::_run_working_query(query);

        result r = CrudRepository::_run_select(afla_id_query);
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

    try {
        char buffer[1024];
        depozite.erase(depozite.find(id));
        depozite.insert({id, d});

        const std::string query_format = "UPDATE " + getTable() + " SET nume='%s', adresa='%s', manager=%d WHERE id=%d";
        sprintf(buffer, query_format.c_str(), d.getNume().c_str(), d.getAdresa().c_str(), angajatRepository.findAngajat(*d.getManager()));
        const std::string query(buffer);

        CrudRepository<Depozit>::_run_working_query(query);

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
