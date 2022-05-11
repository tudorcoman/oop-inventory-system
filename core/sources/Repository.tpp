//
// Created by Tudor Coman on 03.04.2022.
//

#include "../headers/Repository.h"
#include "../../infra/headers/SQLRepository.h"
#include "../headers/Exceptions.h"

template<typename T>
result Repository<T>::_run_select(const std::string &sql) {
    return getTransaction().exec(sql);
}

template<typename T>
void Repository<T>::_run_working_query(const std::string &query) {
    work W(SQLRepository::getInstance()->conn);
    W.exec(query);
    W.commit();
}

template<typename T>
Repository<T>::Repository(std::string table, std::vector<TableField> fields): table(std::move(table)), fields(std::move(fields)), created_work(false), fetched_objects(false) {
}

template<typename T>
const std::string &Repository<T>::getTable() const {
    return table;
}

template<typename T>
const std::vector<TableField> &Repository<T>::getFields() const {
    return fields;
}

template<typename T>
std::string Repository<T>::_build_where_clause(const std::map<std::string, std::string>& filters) const {
    std::vector<std::string> where_clauses;
    for(const auto& it: filters) {
        std::string where_clause;
        auto x = std::find(getFields().begin(), getFields().end(), it.first);
        if(x == getFields().end()) {
            throw IllegalFieldException();
        } else {
            where_clause += it.first + "=";
            switch(x->getTip()) {
                case TableField::DATE:
                case TableField::TEXT: where_clause += SQLRepository::getInstance()->conn.quote(it.second); break;
                default: where_clause += std::all_of(it.second.begin(), it.second.end(), ::isdigit) ? it.second : "-1"; break; // Thank you https://stackoverflow.com/questions/8888748/how-to-check-if-given-c-string-or-char-contains-only-digits
            }
            where_clauses.push_back(where_clause);
        }
    }

    std::string whereClause = boost::algorithm::join(where_clauses, " AND ");
    if (!whereClause.empty()) {
        whereClause = "WHERE " + whereClause;
    }
    return whereClause;
}

template<typename T>
nontransaction Repository<T>::getTransaction() const {
    return {SQLRepository::getInstance()->conn};
}

template<typename T>
void Repository<T>::prepareStatement(const std::string& name, const std::string& format) {
    SQLRepository::getInstance()->conn.prepare(name, format);
}

template<typename T>
template<typename... Args>
void Repository<T>::executePrepared(const std::string &name, Args... args) {
    work W(SQLRepository::getInstance()->conn);
    W.exec_prepared(name, args...);
    W.commit();
}
