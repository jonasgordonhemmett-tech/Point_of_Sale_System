//
// Created by gresh on 9/9/2025.
//
#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "Customer.h"
#include <string>

class Employee : public Customer {
private:
    int employeeId;
public:
    Employee(const std::string& firstName, const std::string& lastName, const std::string& phoneNumber);
    Employee(const Employee& employee);
    Employee(const Customer& customer);

    std::string hashID();
    std::string hashID2();
};

#endif // EMPLOYEE_H