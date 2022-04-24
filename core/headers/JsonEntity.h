//
// Created by Tudor Coman on 03.04.2022.
//

#ifndef OOP_JSONENTITY_H
#define OOP_JSONENTITY_H

#include "cpprest/json.h"

class JsonEntity {
public:
    [[nodiscard]] virtual web::json::value getJson() const = 0;
    virtual void fromJson(web::json::value obj) = 0;
    virtual ~JsonEntity() = default;
};


#endif //OOP_JSONENTITY_H
