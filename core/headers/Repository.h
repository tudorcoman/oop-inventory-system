//
// Created by Tudor Coman on 03.04.2022.
//

#ifndef OOP_REPOSITORY_H
#define OOP_REPOSITORY_H

#include <string>
#include "TableField.h"
#include <vector>
#include "pqxx/pqxx"

using namespace pqxx;

template<typename T>
class Repository {
    std::string table;
    std::vector<TableField> fields;
protected:
    bool fetched_objects;
    virtual void _fetch_objects() = 0;

    result _run_select(const std::string& sql);
    void _run_working_query(const std::string& query);
    [[nodiscard]] const std::string &getTable() const;
    [[nodiscard]] const std::vector<TableField> &getFields() const;
    [[nodiscard]] std::string _build_where_clause(const std::map<std::string, std::string>& filters) const;
public:
    Repository(std::string table, std::vector<TableField> fields);

    virtual std::vector<T> opRetrieve(std::map<std::string, std::string> filters) = 0;
    virtual bool opDelete(const int& id) = 0;
    virtual T getById(const int& id) = 0;
};

#include "../sources/Repository.tpp"

#endif //OOP_REPOSITORY_H
