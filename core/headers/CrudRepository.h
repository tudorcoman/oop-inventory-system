//
// Created by Tudor Coman on 03.04.2022.
//

#ifndef OOP_CRUD_REPOSITORY_H
#define OOP_CRUD_REPOSITORY_H

#include <string>
#include <pqxx/pqxx>
#include "TableField.h"
#include <utility>
#include "../../infra/headers/SQLRepository.h"
#include "Repository.h"

using namespace pqxx;

template<typename T, typename... Types>
class CrudRepository: public Repository<T> {
public:
    CrudRepository(const std::string &table, const std::vector<TableField> &fields) : Repository<T>(table, fields) {}
    virtual bool opCreate(const T& object, Types... args)  = 0;
    virtual bool opUpdate(const int& id, const T& object, Types... args) = 0;
};

#endif //OOP_CRUD_REPOSITORY_H
