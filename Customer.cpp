
#include <iostream>
#include "Customer.h"
#include "include/json.hpp"

using namespace std;
//TODO: Require ID

Customer::Customer(string firstName, string lastName, string phoneNumber, int customerId): firstName(firstName), lastName(lastName), phoneNumber(phoneNumber), customerId(customerId){}
Customer::Customer(string firstName, string lastName, string phoneNumber): firstName(firstName), lastName(lastName), phoneNumber(phoneNumber){}

Customer::Customer(const Customer& customer):
    firstName(customer.firstName),
    lastName(customer.lastName),
    phoneNumber(customer.phoneNumber),
    // OMG this one line caused me like an hour of troubleshooting, I forgot to add 'customer.', womp womp
    customerId(customer.customerId),
    credit(customer.credit) {}


string Customer::getFirstName() const {return firstName;}
void Customer::setFirstName(string firstName) {this->firstName = firstName;}

string Customer::getLastName() const {return lastName;}
void Customer::setLastName(string lastName) {this->lastName = lastName;}

string Customer::getPhoneNumber() const{return phoneNumber;}
void Customer::setPhoneNumber(string phoneNumber) {this->phoneNumber = phoneNumber;}

int Customer::getCredit() const{return credit;}
void Customer::setCredit(int credit){this->credit = credit;}
bool Customer::addCredit(int credit){
    if (credit < 0) return false;
    this->credit += credit;
    return true;
}

bool Customer::takeCredit(int credit){
    if (credit > 0) return false;
    this->credit -= credit;
    return true;
}

string Customer::getphoneNumberFancy() const{
    string number = phoneNumber;
    if (number.size() == 10) number.insert(0, "1");
    number.insert(1, " ");
    number.insert(2, "(");
    number.insert(6, ")");
    number.insert(7, " ");
    number.insert(11, "-");
    return number;
}

Transaction* Customer::getPendingTransaction() const {return pendingTransaction;}

void Customer::setpendingTransaction(Transaction* pendingTransaction) {this->pendingTransaction = pendingTransaction;}

int Customer::getCustomerId() const
{
    return customerId;
}

bool Customer::setCustomerId(int customerId) {
    if (this->customerId != -2) return false;
    this->customerId = customerId;

    return true;
}



std::vector<int> Customer::getTransactions() const{
    return transactions;
}

void Customer::setTransactions(std::vector<int>& transactions){
    this->transactions = transactions;
}


void Customer::addTransaction(int transaction) {this->transactions.push_back(transaction);}


std::ostream& operator<<(std::ostream& os, const Customer& obj) {
    os << obj.firstName << " " << obj.lastName << " " << obj.getphoneNumberFancy() << " ID: " << obj.getCustomerId();
    return os;

}


void to_json(nlohmann::json& j, const Customer& t){
    j = nlohmann::json{
        // https://json.nlohmann.me/api/basic_json/array/
    {"customerId", t.getCustomerId()},
    {"firstName", t.getFirstName()},
    {"lastName", t.getLastName()},
    {"phoneNumber", t.getPhoneNumber()},
    {"credit", t.getCredit()},
    {"transactions", nlohmann::json::array()},

      };

    for (int i : t.getTransactions()){
        j["transactions"].push_back(i);
    }

}

// {"items", nlohmann::json::array()},
// {"totalCost", t.getTotalCost()},
// {"customerId", t.getCustomer()->getCustomerId()},
// };
//
// for (const ItemIn item : t.getTransaction())
// {
//     j["items"].push_back(item.item->getId());
//     j["items"].push_back(item.purchaseQuantity);
//     j["items"].push_back(item.purchasePrice);
//
//
// }
