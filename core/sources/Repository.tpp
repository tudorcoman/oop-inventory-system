//
// Created by Tudor Coman on 03.04.2022.
//

#include "../headers/Repository.h"

template<typename T, typename... Types>
result Repository<T, Types...>::_run_select(const std::string &sql) {
    return nontransaction(SQLRepository::getInstance()->conn).exec(sql);
}

template<typename T, typename... Types>
void Repository<T, Types...>::_run_working_query(const std::string &query) {
    work W(SQLRepository::getInstance()->conn);
    W.exec(query);
    W.commit();
}

template<typename T, typename... Types>
Repository<T, Types...>::Repository(std::string table, std::vector<TableField> fields): table(std::move(table)), fields(std::move(fields)), fetched_objects(false) {
}

template<typename T, typename... Types>
const std::string &Repository<T, Types...>::getTable() const {
    return table;
}

template<typename T, typename... Types>
const std::vector<TableField> &Repository<T, Types...>::getFields() const {
    return fields;
}

template<typename T, typename... Types>
std::string Repository<T, Types...>::_build_where_clause(const std::map<std::string, std::string>& filters) const {
    std::string where_clause = "WHERE ";
    for(const auto& it: filters) {
        auto x = std::find(getFields().begin(), getFields().end(), it.first);
        if(x == getFields().end()) {
            throw std::runtime_error("Illegal field");
        } else {
            where_clause += it.first + "=";
            switch(x->getTip()) {
                case TableField::TEXT: where_clause += "'" + it.second + "'"; break;
                default: where_clause += it.second; break;
            }
            where_clause += " AND ";
        }
    }
    for(int i = 0; i < 5; i++) where_clause.pop_back(); // removing last " AND "
    return where_clause;
}
