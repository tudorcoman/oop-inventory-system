//
// Created by Tudor Coman on 05.04.2022.
//

#include "../headers/TranzactieRepository.h"
#include "../../infra/headers/Utilities.h"

bool TranzactieRepository::prepared_statements;
TranzactieRepository::TranzactieRepository(DepoziteRepository depoziteRepository, ProdusRepository produsRepository): CrudRepository<Tranzactie, int>("tranzactii", std::vector<TableField>{TableField("id", TableField::INT),
                                                                                                                                                    TableField("product_id", TableField::INT),
                                                                                                                                                    TableField("depozit_id", TableField::INT),
                                                                                                                                                    TableField("quantity", TableField::DOUBLE),
                                                                                                                                                    TableField("tip", TableField::TEXT),
                                                                                                                                                    TableField("data_tranzactie", TableField::DATE)}),
                                                                                   depoziteRepository(std::move(depoziteRepository)), produsRepository(std::move(produsRepository)) {
    if(!prepared_statements) {
        prepared_statements = true;
        prepareStatement("tranzactii_create_insert", "INSERT INTO " + getTable() +
                                                     " (product_id, depozit_id, quantity, tip, data_tranzactie) VALUES ($1, $2, $3, $4, $5::timestamp)");
        prepareStatement("tranzactii_create_select", "SELECT id FROM " + getTable() +
                                                     " WHERE product_id=$1 AND depozit_id=$2 AND quantity=$3 AND tip=$4 AND data_tranzactie = $5::timestamp");
    }
}

void TranzactieRepository::_fetch_objects() {
    if(!fetched_objects) {
        result res = CrudRepository<Tranzactie, int>::_run_select("SELECT * FROM " + getTable());
        tranzactii = _build_from_result(res, true);
        fetched_objects = true;
    }
}

bool TranzactieRepository::opCreate(const Tranzactie &t, int depozit_id) {
    if(!fetched_objects) {
        _fetch_objects();
    }

    try {
        if (getById(t.getId()) == t) {
            return false;
        }
    } catch(const std::runtime_error& e) {
        if(std::string(e.what()) != "ID does not exist")
            return false;
    }

    try {
        const std::string transaction_type = t.getTip() == Tranzactie::Type::IN ? "IN" : "OUT";
        executePrepared("tranzactii_create_insert", t.getProdus().getId(), depozit_id, t.getQuantity(),transaction_type, Utilities::getStringFromDate(t.getTimestamp()));

        result r = getTransaction().exec_prepared("tranzactii_create_select", t.getProdus().getId(), depozit_id, t.getQuantity(), transaction_type, Utilities::getStringFromDate(t.getTimestamp()));
        const int id = r.begin()[0].as<int>();

        Tranzactie t_noua = Tranzactie(id, t.getProdus(), t.getQuantity(), t.getTip(), t.getTimestamp());

        tranzactii.push_back(t_noua);
        Depozit d = depoziteRepository.getById(depozit_id);
        d.executaTranzactie(t_noua);
        depoziteRepository.opUpdate(depozit_id, d);

        return true;
    } catch(const std::exception& e) {
        return false;
    }
}

std::vector<Tranzactie> TranzactieRepository::opRetrieve(std::map<std::string, std::string> filters) {
    if(!fetched_objects) {
        _fetch_objects();
    }

    const std::string where_clause = _build_where_clause(filters);
    const std::string query = "SELECT * FROM " + getTable() + " " + where_clause;
    const result res = CrudRepository::_run_select(query);
    return _build_from_result(res, false);
}

bool TranzactieRepository::opUpdate(const int&, const Tranzactie&, int) {
    // tranzactiile sunt read-only
    return false;
}

bool TranzactieRepository::opDelete(const int &id) {
    if (!fetched_objects) {
        _fetch_objects();
    }

    try {
        // o tranzactie nu poate fi stearsa. in schimb putem adauga o noua tranzactie antagonica (care sa o anuleze pe cea existenta)

        const Tranzactie initial = getById(id);
        const Tranzactie antagonic = Tranzactie(id, initial.getProdus(), initial.getQuantity(),
                                                (initial.getTip() == Tranzactie::Type::IN ? Tranzactie::Type::OUT
                                                                                          : Tranzactie::Type::IN),
                                                initial.getTimestamp());

        // ne trebuie sa stim depozitul ca sa executam tranzactia nou creata
        const std::string query = "SELECT depozit_id FROM " + getTable() + " WHERE id = " + std::to_string(id);
        const result res = _run_select(query);
        const int depozit_id = res.begin()[0].as<int>();
        return opCreate(antagonic, depozit_id);
    } catch(const std::exception& e) {
        return false;
    }
}

Tranzactie TranzactieRepository::getById(const int &id) {
    if (!fetched_objects) {
        _fetch_objects();
    }

    for (const auto& tr: tranzactii) {
        if(tr.getId() == id)
            return tr;
    }

    throw IdNotFoundException("TranzactieRepository");
}

std::vector<Tranzactie> TranzactieRepository::_build_from_result(const result &res, bool execute_transactions) {
    std::vector<Tranzactie> ans;
    for(result::const_iterator it = res.begin(); it != res.end(); it ++) {
        int id = it[0].as<int>();
        Produs p = produsRepository.getById(it[1].as<int>());
        Depozit d = depoziteRepository.getById(it[2].as<int>());
        auto q = it[3].as<double>();
        Tranzactie::Type tip = (it[4].as<std::string>() == "IN") ? Tranzactie::Type::IN : Tranzactie::Type::OUT;
        auto timestamp = boost::posix_time::time_from_string(it[5].as<std::string>());
        Tranzactie t = Tranzactie(id, p, q, tip, timestamp);
        ans.push_back(t);
        if (execute_transactions) {
            d.executaTranzactie(t);
        }
    }
    return ans;
}

std::shared_ptr<Repository<Tranzactie>> TranzactieRepository::clone() const {
    return std::make_shared<TranzactieRepository>(*this);
}
