#include <iostream>
#include <iomanip>
#include <utility>
#include <vector>
#include <map>

class Produs {
    int id{};
    std::string nume, categorie;
    double pretCumparare{}, pretVanzare{};
public:
    Produs() {
        *this = Produs(0, "", "", 0.0, 0.0);
    }

    explicit Produs(int id, std::string nume, std::string categorie, double pretCumparare, double pretVanzare) : id(id), nume(std::move(nume)), categorie(std::move(categorie)), pretCumparare(pretCumparare), pretVanzare(pretVanzare) {}
    friend std::ostream& operator << (std::ostream& os, const Produs& p) {
        os << "Produs #" << p.id << ": " << p.nume << "\n"
        << "Categoria: " << p.categorie << "\n"
        << "Pretul de cumparare: " << p.pretCumparare << " RON\n"
        << "Pretul de vanzare: " << p.pretVanzare << " RON\n";
        return os;
    }

    /*
    [[nodiscard]] int getId() const {
        return id;
    }
    */

    [[nodiscard]] const std::string &getNume() const {
        return nume;
    }

    /*
    [[nodiscard]] const std::string &getCategorie() const {
        return categorie;
    }
    */

    [[nodiscard]] double getPretCumparare() const {
        return pretCumparare;
    }

    [[nodiscard]] double getPretVanzare() const {
        return pretVanzare;
    }

    bool operator<(const Produs &other) const {
        return id < other.id;
    }
};

class Angajat {
    std::string first_name, last_name;
    long long cnp;
    Angajat* manager;
public:
    explicit Angajat(std::string firstName, std::string lastName, long long int cnp, Angajat* manager): first_name(std::move(firstName)), last_name(std::move(lastName)), cnp(cnp), manager(manager) {}

    Angajat(const Angajat& other): first_name(other.first_name), last_name(other.last_name), cnp(other.cnp), manager(other.manager) {
        std::cout << "constr copiere Angajat " << first_name + " " + last_name << "\n";
    }

    friend std::ostream& operator << (std::ostream& os, const Angajat& a) {
        os << "Angajat " << a.first_name << " " << a.last_name << "\n"
        << "CNP " << a.cnp << "\n";

        if (a.manager != nullptr) {
            os << "Manager: " << a.manager->first_name << " " << a.manager->last_name << "\n";
        }
        return os;
    }

    Angajat& operator =(const Angajat& other) {
        first_name = other.first_name;
        last_name = other.last_name;
        manager = other.manager;
        cnp = other.cnp;
        std::cout << "op= Angajat " << first_name + " " + last_name << "\n";
        return *this;
    }

    ~Angajat() {
        std::cout << "destr Angajat " << first_name + " " + last_name << "\n";
    }

    void setManager(Angajat *mgr) {
        this->manager = mgr;
    }
};

class Tranzactie {
public:
    enum Type {IN, OUT};

    explicit Tranzactie(int id, Produs produs, double quantity, Type tip) : id(id), produs(std::move(produs)), quantity(quantity),
                                                                          tip(tip) {}

    friend std::ostream& operator << (std::ostream& os, const Tranzactie& tr) {
        os << "Tranzactia de " << tr.tip << " #" << tr.id << "\n";
        os << std::fixed << std::showpoint << std::setprecision(2);
        os << tr.quantity << " x " << tr.produs.getNume() << "\n";
        //os << "Detalii produs: " << tr.produs;
        return os;
    }

    /*
    [[nodiscard]] int getId() const {
        return id;
    }
     */

    [[nodiscard]] const Produs &getProdus() const {
        return produs;
    }

    [[nodiscard]] double getQuantity() const {
        return quantity;
    }

    [[nodiscard]] Type getTip() const {
        return tip;
    }

private:
    const int id;
    const Produs produs;
    const double quantity;
    const Type tip;
};

class Depozit {
    std::string nume;
    std::string adresa;
    std::map<Produs, double> stoc;
    std::vector<Tranzactie> tranzactii;

public:
    Depozit(std::string nume, std::string adresa) : nume(std::move(nume)), adresa(std::move(adresa)) {}

    explicit Depozit(std::string nume, std::string adresa, std::map<Produs, double> stoc,
            std::vector<Tranzactie> tranzactii) : nume(std::move(nume)), adresa(std::move(adresa)), stoc(std::move(stoc)),
                                                         tranzactii(std::move(tranzactii)) {}
    // momentan sunt functii neutilizate asa ca le-am comentat pentru a trece Cppcheck:
    /*[[nodiscard]] const std::string &getNume() const {
        return nume;
    }

    [[nodiscard]] const std::string &getAdresa() const {
        return adresa;
    }

    [[nodiscard]] const std::map<Produs, double> &getStoc() const {
        return stoc;
    }

    [[nodiscard]] const std::vector<Tranzactie> &getTranzactii() const {
        return tranzactii;
    }*/

    void executaTranzactie(const Tranzactie& t) {
        this->tranzactii.push_back(t);
        const int op = (t.getTip() == Tranzactie::Type::IN) ? 1 : -1;
        this->stoc[t.getProdus()] += op * t.getQuantity();
    }

    friend std::ostream& operator << (std::ostream& os, const Depozit& d) {
        os << "Depozitul " << d.nume<< "\n"
        << "Adresa: " << d.adresa << "\n";

        os << "Stocul la zi: " << "\n";
        for(const auto& it: d.stoc) {
            os << it.first;
            os << std::fixed << std::showpoint << std::setprecision(2);
            os << "(cantitate " <<  it.second << ")\n";
        }
        return os;
    }
};

std::ostream& operator<<(std::ostream& os, const Tranzactie::Type tip) {
    switch(tip) {
        case Tranzactie::Type::IN: os << "intrare"; return os;
        case Tranzactie::Type::OUT: os << "iesire"; return os;
    }
    return os;
}

int main() {
    Depozit d = Depozit("fmi", "Strada Academiei nr. 14, sector 1, Bucuresti");

    const Produs produs_prost = Produs(73, "produs neprofitabil", "papetarie", 20.0, 10.0);
    if(produs_prost.getPretCumparare() > produs_prost.getPretVanzare()) {
        std::cout << produs_prost << "Nu este profitabil" << "\n";
    }

    Angajat eu = Angajat("Tudor", "Coman", 5000000000000, nullptr); // eu sunt seful
    Angajat employees[] = {
            Angajat("Bill", "Gates", 1000000000000, &eu),
            Angajat("Sundar", "Pichai", 2000000000000, &eu)
    };

    const Angajat e = employees[1];
    employees[1].setManager(&employees[0]); // se schimba ierarhia

    // comparam cele doua versiuni;
    std::cout << e << "\n";
    std::cout << employees[1] << "\n";

    Tranzactie t = Tranzactie(1, produs_prost, 1.0, Tranzactie::Type::IN); // prima tranzactie
    std::cout << d << "\n\n\n";
    d.executaTranzactie(t);
    std::cout << d;
    return 0;
}
