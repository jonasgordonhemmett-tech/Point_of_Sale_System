// ADDS MORE CLASSES
#ifndef CUSTOMER_H
#define CUSTOMER_H
#include "include/json.hpp"
#include <memory>
#include <list>
#include <string>
#include <vector>

//TODO Make customer index start at -2, set it when added to database
// https://www.geeksforgeeks.org/cpp/what-are-forward-declarations-in-c/

class Transaction;

class Customer {
private:
    int customerId = -2;
    std::string firstName;
    std::string lastName;
    int credit = 0;
    std::string phoneNumber;
    // Smart pointers were a headache.
    Transaction* pendingTransaction = nullptr;
    std::vector<int> transactions;

public:
    Customer(std::string firstName, std::string lastName, std::string phoneNumber);  // constructor declaration
    Customer(std::string firstName, std::string lastName, std::string phoneNumber, int customerId);  // constructor declaration

    Customer(const Customer& customer);

    std::string getFirstName() const;
    void setFirstName(std::string firstName);

    std::string getLastName() const;
    void setLastName(std::string lastName);


    std::string getPhoneNumber() const;
    std::string getphoneNumberFancy() const;
    void setPhoneNumber(std::string phoneNumber);

    int getCredit() const;
    void setCredit(int credit);
    bool addCredit(int credit);
    bool takeCredit(int credit);

    Transaction* getPendingTransaction() const;
    void setpendingTransaction(Transaction* pendingTransaction);

    bool setCustomerId(int customerId);
    int getCustomerId() const;

    void setTransactions(std::vector<int>& transactions);
    void addTransaction(int transaction);
    std::vector<int> getTransactions() const;

    friend std::ostream& operator<<(std::ostream& os, const Customer& obj);

};
void to_json(nlohmann::json& j, const Customer& t);


#endif // CUSTOMER_H