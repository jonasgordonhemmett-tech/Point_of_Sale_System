#ifndef BASE_H
#define BASE_H

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
#include <filesystem>
#include "SPass.h"
using namespace std;

void TransactionTester1();

/*
    Creates a new transaction.

    Input: Index to be added at, transaction to be added.

*/
int createTransaction(int id, Transaction& transaction, SPass& sPass);

/*
    Creates a new transaction.

    Input: Transaction to be added.

*/
int createTransaction(Transaction& transaction, SPass& sPass);


/*
    Stops a pending transaction.

    Output: whether the transaction could be deleted or not.
*/
bool cancelTransaction(int id, SPass& sPass);

/*
    Limits the number of transactions in memory
*/
void numTransactionLimit(bool futureCheck, SPass& sPass);

/*
    Checks if a transaction exists/

    Input: ID to be checked.

    Output: whether the transaction exists
*/
bool checkTransaction(int id, SPass& sPass);

/*
    Moves a transaction from pending to completed and saves it to a file.

    Input: ID to be archived.

    Output: whether the archive could be completed.
*/

bool archiveTransaction(int id, SPass& sPass);

/*
    Reads an archived transaction from a save file to memory.

    Input: ID to be retrieved.

    Output: Whether the retrieval could be completed or not.
*/
bool retrieveTransaction(int id, SPass& sPass);

/*
    Checks if a customer exists

    Input: ID to be checked.

    Output: whether the customer exists
*/
bool checkCustomer(int id, SPass& sPass);

/*
    Saves customer data to a file

    Input: ID to be archived.

    Output: whether the archive could be completed.
*/
bool archiveCustomer(int id, SPass& sPass);

/*
    Reads an archived customer from the save file to memory.

    Input: ID to be archived.

    Output: whether the retrieval could be completed.
*/
bool retrieveCustomer(int id, SPass& sPass);

/*
    TODO, This function needs a lot of work and I'm not sure if it is necessary. Removing a customer will require changing how the customers are stored in memory because the ages are stored in queues, which only really has pop. I think something with iterators might work, we will see.

    Removes a customer's save data

    Input: ID to be deleted.

    Output: Whether the deletion could be completed or not.
*/
bool removeCustomer(int id, SPass& sPass);


/*
    Limits the number of customers in memory.
*/
void numCustomerLimit(bool futureCheck, SPass& sPass);

/*
    Finds the item object to an associated ID.

    Input: ID.

    Output: Item Object.
*/
Item* findItem(int id, SPass& sPass);

/*
  Creates a new customer in Memory.

  Input: Address to be added at, customer to be added.

  Output: Whether the customer could create or not.
*/
int createCustomer(int id, Customer& customer, SPass& sPass);
/*
  Creates a new customer in Memory.

  Input: Customer to be added.

  Output: Whether the customer could create or not.
*/
int createCustomer(Customer& customer, SPass& sPass);

/*
    Reads archived items from a save file to memory.

    Output: Whether the retrieval could be completed or not.
*/
bool retrieveItems(SPass& sPass);

/*
    Saves item data to a file.

    Input: ID to be archived.

    Output: Whether the archive could be completed or not.
*/
bool archiveItem(int id, SPass& sPass);

/*
    Adds a new kind item to inventory.

    Input: ID to be added at, item to be added.

    Output: Whether the addition could be completed or not.
*/
int addItem(int id, Item& item, SPass& sPass);

/*
    Adds a new kind item to inventory.

    Input: Item to be added.

    Output: Whether the addition could be completed or not.
*/
int addItem(Item& item, SPass& sPass);

/*
    Helper functions for file IO.

    Input: File name.

    output: Valid file path.
*/
std::ifstream openInFile(std::string fileName, SPass& sPass);
std::ofstream openOutFile(std::string fileName, SPass& sPass);

#endif