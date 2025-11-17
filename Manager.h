//
// Created by gresh on 9/10/2025.
//

#ifndef MANAGER_H
#define MANAGER_H

#include "Employee.h"
#include <string>

class Manager : public Employee {
public:
    Manager(const std::string& firstName, const std::string& lastName, const std::string& phoneNumber);
    Manager(const Manager& manager);
    Manager(const Employee& employee);
    Manager(const Customer& customer);

    std::string hashID();
    std::string hashID2();

};

#endif //MANAGER_H
