//
// Created by Tudor Coman on 01.04.2022.
//

#ifndef OOP_UTILITIES_H
#define OOP_UTILITIES_H

#include <string>
#include <vector>
#include "../../core/headers/Repository.h"

class Utilities {
public:
    static std::vector<std::string> string_split(std::string s, std::string delim) {
        std::vector<std::string> result;

        auto start = 0U;
        auto end = s.find(delim);

        while(end != std::string::npos) {
            result.push_back(s.substr(start, end - start));
            start = end + delim.length();
            end = s.find(delim, start);
        }

        result.push_back(s.substr(start, end));
        return result;
    }

    template<class T>
    static web::json::value getJsonArray(Repository<T>* repo, std::map<std::string, std::string> filters) {
        /*  Thank you https://stackoverflow.com/questions/4984502/how-to-force-template-class-to-be-derived-from-baseclassa */

        static_assert(std::is_base_of<JsonEntity, T>::value, "Entity must be a JsonEntity");
        // static_assert(std::is_base_of<JsonEntity, T>::_v, "Entity must be a JsonEntity"); --> da eroare de compilare (error: no member named '_v' in 'std::__1::is_base_of<JsonEntity, Produs>')
        // static_assert(std::derived_from<T, JsonEntity>, "Entity must be a JsonEntity"); --> da eroare de compilare (error: no member named 'derived_from' in namespace 'std')

        const std::vector<T> obj = repo->opRetrieve(filters);
        web::json::value output = web::json::value::array();

        for (unsigned i = 0; i < obj.size(); i ++) {
            output[i] = obj[i].getJson();
        }
        return output;
    }

    static std::string getStringFromDate(boost::posix_time::ptime timestamp) {
        std::stringstream stream;
        boost::posix_time::time_facet* facet = new boost::posix_time::time_facet();
        facet->format("%Y-%m-%d %T");
        stream.imbue(std::locale(std::locale::classic(), facet));
        stream << timestamp;
        return stream.str();
    }
};
#endif //OOP_UTILITIES_H
