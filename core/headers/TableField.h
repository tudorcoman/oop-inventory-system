//
// Created by Tudor Coman on 03.04.2022.
//

#ifndef OOP_TABLEFIELD_H
#define OOP_TABLEFIELD_H

#include <string>

class TableField {
public:
    enum TYPE{INT, LONG, TEXT, DOUBLE, DATE};

    TableField(std::string name, TYPE tip);

    [[nodiscard]] const std::string &getName() const;

    [[nodiscard]] TYPE getTip() const;

    virtual ~TableField() = default;

    bool operator==(const std::string &rhs) const;

    bool operator!=(const std::string &rhs) const;

    TableField& operator = (const TableField& other) = default;

private:
    std::string name;
    TYPE tip;
};


#endif //OOP_TABLEFIELD_H
