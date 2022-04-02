//
// Created by Tudor Coman on 03.04.2022.
//

#ifndef OOP_TABLEFIELD_H
#define OOP_TABLEFIELD_H

#include <string>

class TableField {
public:
    enum TYPE{INT, LONG, TEXT, DOUBLE};

    TableField(const std::string &name, TYPE tip);

    [[nodiscard]] const std::string &getName() const;

    [[nodiscard]] TYPE getTip() const;

    virtual ~TableField();

    bool operator==(const std::string &rhs) const;

    bool operator!=(const std::string &rhs) const;

private:
    std::string name;
    TYPE tip;
};


#endif //OOP_TABLEFIELD_H
