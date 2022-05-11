//
// Created by Tudor Coman on 03.04.2022.
//

#include "../headers/ProdusRepository.h"
#include "../../core/headers/ObjectFactory.h"

std::vector<Produs> ProdusRepository::_build_from_result(const result &res) {
    std::vector<Produs> prd;
    for(result::const_iterator it = res.begin(); it != res.end(); it ++) {
        prd.push_back(ObjectFactory::produsFromResult(it));
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
                                                                  TableField("selling_price", TableField::DOUBLE)}) {
    prepareStatement("produse_create_insert", "INSERT INTO " + getTable() + " (name, category, buying_price, selling_price) VALUES ($1, $2, $3, $4)");
    prepareStatement("produse_create_select", "SELECT id FROM " + getTable() + " WHERE name=$1 AND category=$2 AND buying_price=$3 AND selling_price=$4");
    prepareStatement("produse_update", "UPDATE " + getTable() + " SET name=$1, category=$2, buying_price=$3, selling_price=$4 WHERE id=$5");
}

bool ProdusRepository::opCreate(const Produs &p) {
    if(!fetched_objects) {
        _fetch_objects();
    }
    try {
        executePrepared("produse_create_insert", p.getNume(), p.getCategorie(), p.getPretCumparare(), p.getPretVanzare());

        result r = getTransaction().exec_prepared("produse_create_select", p.getNume(), p.getCategorie(), p.getPretCumparare(), p.getPretVanzare());
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

    const Produs prod = getById(id);
    try {
        std::remove(produse.begin(), produse.end(), prod);
        Produs new_product = Produs(id, p.getNume(), p.getCategorie(), p.getPretCumparare(), p.getPretVanzare());
        produse.push_back(new_product);

        executePrepared("produse_update", new_product.getNume(), new_product.getCategorie(), new_product.getPretCumparare(), new_product.getPretVanzare());
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
    throw IdNotFoundException("ProdusRepository");
}

ProdusRepository &ProdusRepository::operator=(const ProdusRepository &other) {
    CrudRepository<Produs>::operator = (other);
    produse = other.produse;
    return *this;
}

std::shared_ptr<Repository<Produs>> ProdusRepository::clone() const {
    return std::make_shared<ProdusRepository>(*this);
}
