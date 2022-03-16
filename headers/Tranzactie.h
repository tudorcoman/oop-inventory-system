//
// Created by Tudor Coman on 16.03.2022.
//

#ifndef OOP_TRANZACTIE_H
#define OOP_TRANZACTIE_H

#include <iomanip>
#include "Produs.h"

class Tranzactie {
public:
    enum Type {IN, OUT};

    explicit Tranzactie(int id, Produs produs, double quantity, Type tip) : id(id), produs(std::move(produs)), quantity(quantity),
                                                                            tip(tip) {}

    friend std::ostream& operator << (std::ostream& os, const Tranzactie& tr);

    /*
    [[nodiscard]] int getId() const {
        return id;
    }
     */

    [[nodiscard]] const Produs &getProdus() const;

    [[nodiscard]] double getQuantity() const;

    [[nodiscard]] Type getTip() const;

private:
    const int id;
    const Produs produs;
    const double quantity;
    const Type tip;
};


#endif //OOP_TRANZACTIE_H
