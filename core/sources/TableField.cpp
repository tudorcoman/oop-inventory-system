//
// Created by Tudor Coman on 03.04.2022.
//

#include "../headers/TableField.h"

#include <utility>

TableField::TableField(std::string name, TableField::TYPE tip) : name(std::move(name)), tip(tip) {}

const std::string &TableField::getName() const {
    return name;
}

TableField::TYPE TableField::getTip() const {
    return tip;
}

bool TableField::operator==(const std::string &rhs) const {
    return name == rhs;
}

bool TableField::operator!=(const std::string &rhs) const {
    return !(rhs == *this);
}
