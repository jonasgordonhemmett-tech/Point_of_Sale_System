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
#include <map>
#include <unordered_map>
#include <fstream>
#include "include/json.hpp"
#include <algorithm>
#include <queue>
#include <optional>
#include "settings.h"
#include "SPass.h"
#ifndef BASEINTERFACE_H
#define BASEINTERFACE_H
/*
  Creates a customer from user input
*/
template <typename dataType>
dataType userInput(string messege);

/*
  Creates a customer from user input
*/
Customer createCustomerInput();

void transactionInput(Transaction& transaction, SPass& sPass);
void menuLogin(SPass& sPass);
int menuGetCustomer(SPass& sPass);
void menu2(SPass& sPass);
void menuTransaction(int customerId, SPass& sPass);
void menu4(SPass& sPass);

string priceFormat(int price);
#endif //BASEINTERFACE_H
