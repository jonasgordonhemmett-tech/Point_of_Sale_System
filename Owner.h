//
// Created by gresh on 9/10/2025.
//

#ifndef OWNER_H
#define OWNER_H

#include "Manager.h"
#include <string>

class Owner : public Manager {
private:
  int credit = -1;

public:
    Owner(const std::string& firstName, const std::string& lastName, const std::string& phoneNumber);
    Owner(const Owner& owner);
    Owner(const Manager& manager);
    Owner(const Employee& employee);
    Owner(const Customer& customer);

    std::string hashID();
    std::string hashID2();

};

#endif //OWNER_H
