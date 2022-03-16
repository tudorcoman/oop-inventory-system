#include <iostream>
#include <memory>
#include <vector>
#include "headers/Produs.h"
#include "headers/Tranzactie.h"
#include "headers/Angajat.h"
#include "headers/Depozit.h"

using namespace std::string_literals;

int main() {
    Depozit d = Depozit("fmi", "Strada Academiei nr. 14, sector 1, Bucuresti");

    const Produs produs_prost = Produs(73, "produs neprofitabil", "papetarie", 20.0, 10.0);
    if(produs_prost.getPretCumparare() > produs_prost.getPretVanzare()) {
        std::cout << produs_prost << "Nu este profitabil" << "\n";
    }

    Angajat eu = Angajat("Tudor", "Coman", 5000000000000, nullptr); // eu sunt seful
    std::vector<Angajat> employees;
    employees.emplace_back("Bill", "Gates", 1000000000000, std::make_shared<Angajat>(eu));
    employees.emplace_back("Sundar", "Pichai", 2000000000000, std::make_shared<Angajat>(eu));

    const Angajat e = employees[1];
    employees[1].setManager(std::make_shared<Angajat>(employees[0])); // se schimba ierarhia

    // comparam cele doua versiuni;
    std::cout << e << "\n";
    std::cout << employees[1] << "\n";

    Tranzactie t = Tranzactie(1, produs_prost, 1.0, Tranzactie::Type::IN); // prima tranzactie
    std::cout << d << "\n\n\n";
    d.executaTranzactie(t);
    std::cout << d;
    return 0;
}
