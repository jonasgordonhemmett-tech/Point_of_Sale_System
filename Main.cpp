#include <iostream>
#include <string>
#include <limits>
#include "Customer.h"
#include "Employee.h"
#include "Manager.h"
#include "Transaction.h"
#include "Item.h"
#include <list>
#include <algorithm>
#include <string>
#include <iostream>
#include <cctype>
#include <sstream>
#include "ItemIn.h"
#include <memory>
#include <map>
#include <unordered_map>
#include <fstream>
#include "include/json.hpp"
#include <algorithm>
#include <queue>
#include <optional>
#include "settings.h"
#include "Tester.h"

#include "base.cpp"
#include "baseInterface.cpp"

int main(){

    unordered_map<int, Transaction> transactions;
    unordered_map<int, Transaction> transactionArchives;
    queue<int> transactionArchivesAge;

    unordered_map<int, Customer> customers;
    queue<int> customersAge;

    unordered_map<int, Item> items;

    Settings settings("live_files", true);

    PaymentPortal paymentPortal(settings.getFolder());
    SPass sPass(transactions, transactionArchives, transactionArchivesAge, customers, customersAge, settings, paymentPortal, items);

    retrieveItems(sPass);
    menuLogin(sPass);

    return 0;
}