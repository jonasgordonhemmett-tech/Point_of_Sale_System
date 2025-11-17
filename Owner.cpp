#include "Owner.h"
#include <iostream>

using namespace std;
// Must match declaration exactly
Owner::Owner(const std::string& firstName, const std::string& lastName, const std::string& phoneNumber) :
    Manager(firstName, lastName, phoneNumber){}

Owner::Owner(const Owner& Owner): Manager(Owner){}

Owner::Owner(const Employee& employee): Manager(employee){}

Owner::Owner(const Customer& customer): Manager(customer){}

string Owner::hashID2(){
    string employeeId = Manager::hashID2();

    employeeId[employeeId.length()-1] = 'o';

    return employeeId;


}
string Owner::hashID(){
    string employeeId = Manager::hashID();

    employeeId[employeeId.length()-1] = 'o';

    return employeeId;
}