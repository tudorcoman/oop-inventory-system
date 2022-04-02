//
// Created by Tudor Coman on 03.04.2022.
//

#include "../headers/TableField.h"

TableField::TableField(const std::string &name, TableField::TYPE tip) : name(name), tip(tip) {}

TableField::~TableField() {
}

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
