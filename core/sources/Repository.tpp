//
// Created by Tudor Coman on 03.04.2022.
//

#include "../headers/Repository.h"
#include "../../infra/headers/SQLRepository.h"

template<typename T>
result Repository<T>::_run_select(const std::string &sql) {
    return nontransaction(SQLRepository::getInstance()->conn).exec(sql);
}

template<typename T>
void Repository<T>::_run_working_query(const std::string &query) {
    work W(SQLRepository::getInstance()->conn);
    W.exec(query);
    W.commit();
}

template<typename T>
Repository<T>::Repository(std::string table, std::vector<TableField> fields): table(std::move(table)), fields(std::move(fields)), fetched_objects(false) {
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
    std::string where_clause = "WHERE ";
    for(const auto& it: filters) {
        auto x = std::find(getFields().begin(), getFields().end(), it.first);
        if(x == getFields().end()) {
            throw std::runtime_error("Illegal field");
        } else {
            where_clause += it.first + "=";
            switch(x->getTip()) {
                case TableField::DATE:
                case TableField::TEXT: where_clause += "'" + it.second + "'"; break;
                default: where_clause += it.second; break;
            }
            where_clause += " AND ";
        }
    }
    for(int i = 0; i < 5; i++) where_clause.pop_back(); // removing last " AND "
    return where_clause;
}