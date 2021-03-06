//
// Created by Tudor Coman on 16.03.2022.
//

#ifndef OOP_TRANZACTIE_H
#define OOP_TRANZACTIE_H

#include <iomanip>
#include <boost/date_time.hpp>
#include "Produs.h"

class Tranzactie: public JsonEntity {
public:
    enum Type {IN, OUT};

    Tranzactie(const Tranzactie& other);
    explicit Tranzactie(int id, const Produs& produs, double quantity, Type tip, boost::posix_time::ptime timestamp) : id(id), produs(produs), quantity(quantity),
                                                                            tip(tip), timestamp(timestamp) {}

    friend std::ostream& operator << (std::ostream& os, const Tranzactie& tr);

    [[nodiscard]] std::shared_ptr<JsonEntity> clone() const override;

    [[nodiscard]] int getId() const;

    [[nodiscard]] const Produs &getProdus() const;

    [[nodiscard]] const boost::posix_time::ptime &getTimestamp() const;

    [[nodiscard]] double getQuantity() const;

    [[nodiscard]] Type getTip() const;

    [[nodiscard]] web::json::value getJson() const override;

    void fromJson(web::json::value obj) override;

    bool operator==(const Tranzactie &rhs) const;

    bool operator!=(const Tranzactie &rhs) const;

    Tranzactie& operator = (const Tranzactie &other);

private:
    int id;
    Produs produs;
    double quantity;
    Type tip;
    boost::posix_time::ptime timestamp;
};


#endif //OOP_TRANZACTIE_H
