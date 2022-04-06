//
// Created by Tudor Coman on 03.04.2022.
//

#include "../headers/ProdusRepository.h"

std::vector<Produs> ProdusRepository::_build_from_result(const result &res) {
    std::vector<Produs> prd;
    for(result::const_iterator it = res.begin(); it != res.end(); it ++) {
        int id = it[0].as<int>();
        auto name = it[1].as<std::string>(),
             category = it[2].as<std::string>();
        auto pretCumparare = it[3].as<double>(),
             pretVanzare = it[4].as<double>();
        prd.emplace_back(id, name, category, pretCumparare, pretVanzare);
    }
    return prd;
}

void ProdusRepository::_fetch_objects() {
    if(!fetched_objects) {
        result res = CrudRepository<Produs>::_run_select("SELECT * FROM " + getTable());
        produse = _build_from_result(res);
        fetched_objects = true;
    }
}

ProdusRepository::ProdusRepository():
        CrudRepository<Produs>("produse", std::vector<TableField>{TableField("id", TableField::INT),
                                                                  TableField("name", TableField::TEXT),
                                                                  TableField("category", TableField::TEXT),
                                                                  TableField("buying_price", TableField::DOUBLE),
                                                                  TableField("selling_price", TableField::DOUBLE)}) {}

bool ProdusRepository::opCreate(const Produs &p) {
    if(!fetched_objects) {
        _fetch_objects();
    }
    try {
        char buffer[1024];

        const std::string query_format = "INSERT INTO " + getTable() + " (name, category, buying_price, selling_price) VALUES ('%s', '%s', %lf, %lf)";
        sprintf(buffer, query_format.c_str(), p.getNume().c_str(), p.getCategorie().c_str(), p.getPretCumparare(), p.getPretVanzare());
        const std::string query(buffer);

        buffer[0] = 0;
        const std::string afla_id_query_format = "SELECT id FROM " + getTable() + " WHERE name='%s' AND category='%s' AND buying_price=%lf AND selling_price=%lf";
        sprintf(buffer, afla_id_query_format.c_str(), p.getNume().c_str(), p.getCategorie().c_str(), p.getPretCumparare(), p.getPretVanzare());
        const std::string afla_id_query(buffer);

        CrudRepository<Produs>::_run_working_query(query);

        result r = CrudRepository::_run_select(afla_id_query);
        const int id = r.begin()[0].as<int>();

        Produs nou = Produs(id, p.getNume(), p.getCategorie(), p.getPretCumparare(), p.getPretVanzare());
        produse.push_back(nou);

        return true;
    } catch(const std::exception& e) {
        return false;
    }
}

std::vector<Produs> ProdusRepository::opRetrieve(std::map<std::string, std::string> filters) {
    if(!fetched_objects) {
        _fetch_objects();
    }
    const std::string where_clause = _build_where_clause(filters);
    const std::string query = "SELECT * FROM " + getTable() + " " + where_clause;
    const result res = CrudRepository::_run_select(query);
    return _build_from_result(res);
}

bool ProdusRepository::opUpdate(const int &id, const Produs &p) {
    if(!fetched_objects) {
        _fetch_objects();
    }

    try {
        char buffer[1024];
        std::remove(produse.begin(), produse.end(), getById(id));
        Produs new_product = Produs(id, p.getNume(), p.getCategorie(), p.getPretCumparare(), p.getPretVanzare());
        produse.push_back(new_product);

        const std::string query_format = "UPDATE " + getTable() + " SET name='%s', category='%s', buying_price=%lf, selling_price=%lf WHERE id=%d";
        sprintf(buffer, query_format.c_str(), new_product.getNume().c_str(), new_product.getCategorie().c_str(), new_product.getPretCumparare(), new_product.getPretVanzare());
        const std::string query(buffer);

        CrudRepository<Produs>::_run_working_query(query);
        return true;
     } catch(const std::exception& e) {
        return false;
    }
}

bool ProdusRepository::opDelete(const int &id) {
    if(!fetched_objects) {
        _fetch_objects();
    }

    try {
        const std::string query = "DELETE FROM " + getTable() + " WHERE id = " + std::to_string(id);
        CrudRepository<Produs>::_run_working_query(query);
        return true;
    } catch(const std::exception& e) {
        return false;
    }
}

Produs ProdusRepository::getById(const int &id) {
    if(!fetched_objects) {
        _fetch_objects();
    }
    for (const auto& produs: produse) {
        if(produs.getId() == id) {
            return produs;
        }
    }
    throw std::runtime_error("ID does not exist");
}
