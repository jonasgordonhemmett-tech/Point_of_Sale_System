#ifndef SPASS_H
#define SPASS_H
#include <unordered_map>
#include <queue>
#include "Transaction.h"
#include "Customer.h"
#include "Item.h"
#include "settings.h"
#include "PaymentPortal.h"

struct SPass {
    // Pending transactions
    unordered_map<int, Transaction>& transactions;
    // Completed transactions, in memory
    unordered_map<int, Transaction>& transactionArchives;
    // Ages of the completed transactions, newest to oldest by access
    queue<int>& transactionArchivesAge;
    // Customers, in memory
    unordered_map<int, Customer>& customers;
    // Ages of customer in memory
    queue<int>& customersAge;
    Settings& settings;
    PaymentPortal paymentPortal;
    unordered_map<int, Item>& items;

    // Used to make passing info to funcs easier
    SPass(unordered_map<int, Transaction>& transactions,
        unordered_map<int, Transaction>& transactionArchives,
        queue<int>& transactionArchivesAge,
        unordered_map<int, Customer>& customers,
        queue<int>& customersAge,
        Settings& settings,
        PaymentPortal& paymentPortal,
        unordered_map<int, Item>& items
        ):
        transactions(transactions),
        transactionArchives(transactionArchives),
        transactionArchivesAge(transactionArchivesAge),
        customers(customers), customersAge(customersAge),
        settings(settings),
        paymentPortal(paymentPortal),
        items(items){}
};
#endif