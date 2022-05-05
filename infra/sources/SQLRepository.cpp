//
// Created by Tudor Coman on 02.04.2022.
//

#include "../headers/SQLRepository.h"
#include "../headers/Utilities.h"

SQLRepository::SQLRepository() {
    try {
        std::ifstream cfg("db.config");
        std::string date;
        std::getline(cfg, date);
        conn = connection(date);
        if (conn.is_open()) {
            std::cout << "Opened database successfully: " << conn.dbname() << std::endl;
        } else {
            std::cout << "Can't open database" << std::endl;
            exit(1);
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

SQLRepository *SQLRepository::getInstance() {
    static SQLRepository instance;
    return &instance;
}
