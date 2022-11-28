#ifndef MANAGER_H
#define MANAGER_H

#include <optional>
#include "Abonent.h"

class AbonentsManager {
private:
    std::vector<Abonent> abonents_;

    bool inflateAbonents();

public:
    AbonentsManager();
    ~AbonentsManager();

    bool saveAbonents() const;
    [[nodiscard]] const std::vector<Abonent> &getAbonents() const;

    [[nodiscard]] bool isAbonentValid(const Abonent&)const;
    const std::vector<Abonent>::iterator findAbonentByID(const std::string&);
    bool addAbonent(const Abonent&);
    bool removeAbonent(const Abonent&);
};

static const Abonent ROOT {"idroot", "admin", "adminROOT" };

#endif //MANAGER_H
