#include "Manager.h"
#include <iostream>

using namespace std;
// Must match declaration exactly
Manager::Manager(const std::string& firstName, const std::string& lastName, const std::string& phoneNumber)
    : Employee(firstName, lastName, phoneNumber){}

Manager::Manager(const Manager& manager): Employee(manager){}

Manager::Manager(const Employee& employee): Employee(employee){}

Manager::Manager(const Customer& customer): Employee(customer){}

string Manager::hashID2()
{
    string employeeId = Employee::hashID2();

    employeeId[employeeId.length()-1] = 'm';

    return employeeId;


}
string Manager::hashID(){
    string employeeId = Employee::hashID();

    employeeId[employeeId.length()-1] = 'm';

    return employeeId;


}