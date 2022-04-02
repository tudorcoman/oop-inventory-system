//
// Created by Tudor Coman on 02.04.2022.
//

#ifndef OOP_SQLREPOSITORY_H
#define OOP_SQLREPOSITORY_H

#include <fstream>
#include <iostream>
#include "pqxx/pqxx"
#include "../../core/headers/Angajat.h"
#include "../../core/headers/Tranzactie.h"
#include "../../core/headers/Depozit.h"

using namespace pqxx;

class SQLRepository {
    connection conn;

    SQLRepository();

    template<typename T, typename... Args> friend class Repository;
public:
    static SQLRepository *getInstance();

};
#endif //OOP_SQLREPOSITORY_H
