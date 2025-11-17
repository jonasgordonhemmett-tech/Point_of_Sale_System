#include "Base.h"

bool checkTransaction(int id, SPass& sPass){
    if (id < 0 ) return false;

    if (sPass.transactions.count(id) || sPass.transactionArchives.count(id)) return true;
    ifstream in = openInFile("transactions.json", sPass);

    try {
        // Opens JSON file and reads it in.
        nlohmann::json data;
        if (in.peek() == std::ifstream::traits_type::eof()) {
            data = nlohmann::json::array();
        } else {
            in >> data;
        }
        in.close();


       if (!data.is_array()) {
            std::cerr << "JSON is not an array";
            return false;
        }

        // Loops through the transaction file looking for a matching ID.
        for (const auto& t : data){
            if (t.contains("id") && t["id"] == id) return true;
        }
    } catch (...) {
        cerr << "checkTransaction JSON error";
    }


    return false;

}

// I copied some of this from https://json.nlohmann.me and https://www.youtube.com/watch?v=Sa8bdVogGIo, copied lines are marked with *
bool archiveTransaction(int id, SPass& sPass) {
    if (!sPass.transactions.count(id)) return false;

    if (sPass.transactions.at(id).isCompleted()) {
        cerr << "Transaction has already been completed";
        return false;
    }
    // Completes the transaction and moves the customer's points and ids
    sPass.transactions.at(id).completeTransaction();
    for (ItemIn& itemIn : sPass.transactions.at(id).getItems()){
        if (itemIn.item != nullptr){
            archiveItem(itemIn.item->getId(), sPass);
        }
    }


    int customerId = sPass.transactions.at(id).getCustomerId();
    if (customerId != -2 && checkCustomer(customerId, sPass)) {
        if (!sPass.customers.count(customerId)) {
            retrieveCustomer(customerId, sPass);
        }
        sPass.customers.at(customerId).setpendingTransaction(nullptr);
        sPass.customers.at(customerId).addTransaction(id);
        archiveCustomer(customerId, sPass);

    }

    // Commented this out, it does nothing
    // for (ItemIn i : sPass.transactions.at(id).getItems()) {
    //
    // }
    using nlohmann::json; //*

    try {
        json data;
        ifstream in = openInFile("transactions.json", sPass);

        if (in) { //*
            if (in.peek() == ifstream::traits_type::eof()) { //*
                data = json::array(); // empty file //*
            } else {
                in >> data; //*
            } //*
        } //*
        in.close(); //*

        if (!data.is_array()) { //*
            data = json::array(); //*
        } //*

        Transaction &trans = sPass.transactions.at(id); //*
        json t = trans; //*

        // Used to overwrite an existing transaction opposed to adding a new one.
        bool replaced = false;
        for (auto& entry : data) {
            if (entry.contains("id") && entry["id"] == id) {
                entry = t;
                replaced = true;
                break;
            }
        }

        if (!replaced) {
            data.push_back(t); //*
        }

        // Rewrite file
        ofstream out = openOutFile("transactions.json", sPass);
        out << data.dump(4); //*
        out.close(); //*

        // Moves the transaction to the correct map.
        sPass.transactionArchives.emplace(id, move(sPass.transactions.at(id)));
        sPass.transactions.erase(id);
        sPass.transactionArchivesAge.push(id);
        numTransactionLimit(true, sPass);

        return true;
    } catch (...) {
        cerr << "archiveTransaction JSON error";
        return false;
    }
}

// I copied some of this from https://json.nlohmann.me and https://www.youtube.com/watch?v=Sa8bdVogGIo, copied lines are marked with *
// I used a lot of JSON array parsing stuff as well from https://codesignal.com/learn/courses/parsing-json-with-csharp/lessons/parsing-json-arrays-and-nested-structures and https://www.w3schools.com/js/js_json_arrays.asp
bool retrieveTransaction(int id, SPass& sPass){
     if (id < 0) return false;
     if (!checkTransaction(id, sPass)) return false;
     // Stops it from being brought in twice
     if (sPass.transactionArchives.count(id)) return true;

    ifstream in = openInFile("transactions.json", sPass);

    nlohmann::json data;

    try {
        if (in.peek() == std::ifstream::traits_type::eof()) {
            data = nlohmann::json::array();
        } else {
            in >> data;
        }
        in.close();

        if (!data.is_array()) {
            std::cerr << "JSON is not an array";
            return false;
        }

        for (const auto& t : data){ //*

            // Finds matching transaction if one exists in the save file.
            if (t.contains("id") && t["id"] == id){

                int customerId = t.value("customerId", -1);  //*
                customerId = checkCustomer(customerId, sPass) ? customerId : -2;
                int totalCost = t.value("totalCost", 0.0);  //*

                Transaction transactionAdd(&sPass.paymentPortal, customerId, totalCost);

                vector<ItemIn> items;
                if (t.contains("items") && t["items"].is_array()) {
                    int itemPurchaseQuantity = 0;
                    int itemPurchasePrice = 0;
                    int itemId = 0;

                    // Creates an empty vector which the customer's items will later be added to.
                    auto itemsVec = t["items"].get<std::vector<nlohmann::json>>();

                    for (const auto& itemArr : itemsVec){ //*
                        if (itemArr.size() == 3) {  //*
                            itemId = itemArr[0].get<int>();
                            itemPurchaseQuantity = itemArr[1].get<int>();
                            itemPurchasePrice = itemArr[2].get<int>();

                            // Adds existing items the vector.
                            ItemIn itemIn(nullptr);

                            if (auto itemObj = findItem(itemId, sPass)) {
                                itemIn.item = itemObj;
                            }
                            itemIn.purchaseQuantity = itemPurchaseQuantity;
                            itemIn.purchasePrice = itemPurchasePrice;
                            items.push_back(itemIn);
                        }

                    }
                }
                transactionAdd.setItems(items);

                // Limits the number of completed transactions in memory.
                sPass.transactionArchives.emplace(id, move(transactionAdd));
                sPass.transactionArchivesAge.push(id);
                numTransactionLimit(true, sPass);

                return true;

            }

        }
    } catch (...) {
        cerr << "RetrieveTransaction JSON error";
    }

    return false;
}

void numTransactionLimit(bool futureCheck, SPass& sPass){
    if (sPass.transactionArchives.size() != sPass.transactionArchivesAge.size())
    {
        cerr << "transactionArchivesAge.size() != transactionArchivesAge.size()";
    }

    // If there are too many transactions, we get keep a queue to remove the oldest one
    if (sPass.transactionArchives.size() > sPass.settings.getMaxNumTransactions() - futureCheck && !sPass.transactionArchivesAge.empty()){
        int removeId = sPass.transactionArchivesAge.front();
        sPass.transactionArchivesAge.pop();
        sPass.transactionArchives.erase(removeId);
    }

}
// Very similar to checkTransaction, but with customers instead.
bool checkCustomer(int id, SPass& sPass){
    if (id < 0) return false;
    if (sPass.customers.count(id)) return true;
    ifstream in = openInFile("customers.json", sPass);

    nlohmann::json data;
    try {
        if (in.peek() == std::ifstream::traits_type::eof()) {
            data = nlohmann::json::array();
        } else {
            in >> data;
        }
        in.close();

        if (!data.is_array()) {
            std::cerr << "JSON is not an array";
            return false;
        }

        for (const auto& t : data)
        {
            if (t.contains("customerId") && t["customerId"] == id) return true;
        }
    } catch (...) {
        cout << "checkCustomer JSON error";
    }
    return false;

}
// Very similar to retrieveTransaction, but with customers instead.
bool retrieveCustomer(int id, SPass& sPass){
    if (!checkCustomer(id, sPass)) return false;
    if (id < 0) return false;
    // Stops it from being brought in twice
    if (sPass.customers.count(id)) return true;

    ifstream in = openInFile("customers.json", sPass);

    nlohmann::json data;

    try {
        if (in.peek() == std::ifstream::traits_type::eof()) {
            data = nlohmann::json::array();
        } else {
            in >> data;
        }
        in.close();

        if (!data.is_array()) {
            std::cerr << "JSON is not an array";
            return false;
        }
        vector<int> transactionsAdd;

        for (const auto& t : data){
            if (t.contains("customerId") && t["customerId"] == id){
                int customerId = t.value("customerId", -1);
                string firstName = t.value("firstName", "NULL");
                string lastName = t.value("lastName", "NULL");
                string phoneNumber = t.value("phoneNumber", "0000000000");
                int credit = t.value("credit", 0);
                if (t.contains("transactions") && t["transactions"].is_array()){
                    transactionsAdd = t["transactions"].get<vector<int>>();
                }



                //TODO, add limit check
                sPass.customers.emplace(id, Customer(firstName, lastName, phoneNumber, customerId));
                sPass.customersAge.push(id);
                if (credit > 0) sPass.customers.at(id).setCredit(credit);
                if (transactionsAdd.size()) sPass.customers.at(id).setTransactions(transactionsAdd);
                numCustomerLimit(true, sPass);


                return true;
            }
        }
    } catch (...) {
        cerr << "RetrieveCustomer JSON error";
    }
    return false;
}



// Very similar to archiveTransaction, but with customers instead.
bool archiveCustomer(int id, SPass& sPass){\
    if (!checkCustomer(id, sPass)) return false;

    using nlohmann::json; //*

    try {
        json data;
        ifstream in = openInFile("customers.json", sPass);
        if (in) { //*
            if (in.peek() == std::ifstream::traits_type::eof()) { //*
                data = json::array(); // empty file //*
            } else {
                in >> data;           // parse existing JSON //*
            } //*
        } //*
        in.close(); //*

        if (!data.is_array()) { //*
            data = json::array(); //*
        } //*


        // Get transaction from map
        Customer &customer = sPass.customers.at(id); //*

        // Convert to JSON
        json c = customer; //*

        // Append

        // Rewrite file
        ofstream out = openOutFile("customers.json", sPass);

        bool replaced = false;
        for (auto& entry : data) {
            if (entry.contains("customerId") && entry["customerId"] == id) {
                entry = c;
                replaced = true;
                break;
            }
        }

        if (!replaced) {
            data.push_back(c); //*
        }

        out << data.dump(4); //*
        // TODO: Find if this causes issues
        // sPass.settings.addCustomerIndex();
        return true;
    } catch (...) {
        cerr << "RetrieveCustomer JSON error";
        return false;
    }
}

//TODO: This will not work with the way customers are currently stored, will probably remove it/
bool removeCustomer(int id, SPass& sPass)
{
    if (id < 0 || id > sPass.settings.getCustomerIndex()) return false;

    // Opens and converts JSON file.
    ifstream in = openInFile("customers.json", sPass);

    nlohmann::json data;
    in >> data;
    in.close();

    if (!data.is_array()) {
        std::cerr << "customers.json is not an array\n";
        return false;
    }

    bool found = false;
    for (auto it = data.begin(); it != data.end(); ++it) {
        if (it->contains("customerId") && (*it)["customerId"] == id) {
            data.erase(it);
            found = true;
            break;
        }
    }

    if (!found) return false;

    ofstream out = openOutFile("customers.json", sPass);

    out << data.dump(5);
    out.close();


    return true;
}

void numCustomerLimit(bool futureCheck, SPass& sPass){
    if (sPass.customers.size() != sPass.customersAge.size()){
        cerr << "customers.size() != customersAge.size()";
    }

    if (sPass.customers.size() > sPass.settings.getMaxNumCustomers() - futureCheck && !sPass.customersAge.empty()){
        int removeId = sPass.customersAge.front();
        sPass.customersAge.pop();
        sPass.customers.erase(removeId);
    }
}

Item* findItem(int id, SPass& sPass){
    if (!sPass.items.count(id)) return nullptr;
    return &sPass.items.at(id);
}


int createCustomer(int id, Customer& customer, SPass& sPass) {

    // -1 is a sentinel value telling us to add it to the end.
    if (id == -1) {
        sPass.settings.addCustomerIndex();
        id = sPass.settings.getCustomerIndex();
    } else if (id < -1) {
        return -2;
    }
    if (checkCustomer(id, sPass)) {
        return -2;
    }

    if (customer.getCustomerId()== -2) {
        customer.setCustomerId(id);
    }

    sPass.customers.emplace(id, customer);
    sPass.customersAge.emplace(id);
    archiveCustomer(id, sPass);
    numCustomerLimit(true, sPass);
    return id;
}

int createCustomer(Customer& customer, SPass& sPass) {
    return createCustomer(-1, customer, sPass);
}


int createTransaction(int id, Transaction& transaction, SPass& sPass) {
    // -1 is a sentinel value telling us to add it to the end.
    if (id == -1) {
        sPass.settings.addTransactionIndex();
        id = sPass.settings.getTransactionIndex();
    } else if (id == -2) {
    } else if (id < -2) {
        return -2;
    }

    if (checkTransaction(id, sPass)) {
        return -2;
    }

    if (transaction.getId() == -2) {
        transaction.setId(id);
    }

    int customerId  = transaction.getCustomerId();

    sPass.transactions.emplace(id,transaction);

    // Attach to customer if possible
    if (customerId != -2) {
        if (checkCustomer(customerId, sPass)) {
            retrieveCustomer(customerId, sPass);


            if (sPass.customers.at(customerId).getPendingTransaction() == nullptr) {
                // Fixed this, no dangling pointers
                sPass.customers.at(customerId).setpendingTransaction(&sPass.transactions.at(id));
            } else {
                cerr << "Customer already has transaction";
                return false;
            }
        }
    }

        return id;

}

int createTransaction(Transaction &transaction, SPass &sPass) {
    return createTransaction(-1, transaction, sPass);
}

bool cancelTransaction(int id, SPass& sPass) {
    if (!sPass.transactions.count(id)) return false;
    sPass.transactions.at(id).cancelTransaction();
    int customerId = sPass.transactions.at(id).getCustomerId();

    // Makes sure there are no dangling pointers
    if (checkCustomer(customerId, sPass)) {
        retrieveCustomer(customerId, sPass);

        sPass.customers.at(customerId).setpendingTransaction(nullptr);
        archiveCustomer(customerId, sPass);
    }
    sPass.transactions.erase(id);
    return true;
}

bool retrieveItems(SPass& sPass) {
    ifstream in = openInFile("items.json", sPass);

    nlohmann::json data;

    try {
        in >> data;
        in.close();

        if (!data.is_array()) {
            std::cerr << "JSON is not an array";
            return false;
        }

        for (const auto& t : data) {
            int id = t.value("id", -2);
            string name = t.value("name", "NULL");
            int price = t.value("price", 0);
            int quantity = t.value("quantity", 0);

                // Stops duplicates
            if (!sPass.items.count(id)) {
                sPass.items.emplace(id, Item(id, name, price, quantity));

            }
        }
        return true;
    } catch (...) {
        cout << "retrieveItems JSON error" << endl;
        return false;
    }

}

bool archiveItem(int id, SPass& sPass){
    if (!findItem(id, sPass)) return false;

    using nlohmann::json; //*

    try {
        json data;
        ifstream in = openInFile("items.json", sPass);
        if (in) { //*
            if (in.peek() == std::ifstream::traits_type::eof()) { //*
                data = json::array(); // empty file //*
            } else {
                in >> data;           // parse existing JSON //*
            } //*
        } //*
        in.close(); //*

        if (!data.is_array()) { //*
            data = json::array(); //*
        } //*


        // Get transaction from map
        Item &item = sPass.items.at(id); //*

        item.setId(id);

        // Convert to JSON
        json c = item; //*

        // Append

        // Rewrite file
        ofstream out = openOutFile("items.json", sPass);

        bool replaced = false;
        for (auto& entry : data) {
            if (entry.contains("id") && entry["id"] == id) {
                entry = c;
                replaced = true;
                break;
            }
        }

        if (!replaced) {
            data.push_back(c); //*
        }

        out << data.dump(4); //*

        // TODO: Find if this causes issues
        // sPass.settings.addItemIndex();

        return true;
    } catch (...) {
        cout << "archiveItem JSON error" << endl;
        return false;
    }
}

int addItem(int id, Item& item, SPass& sPass) {
    // -1 is a sentinel value telling us to add it to the end.

    if (id == -1) {
        sPass.settings.addItemIndex();
        id = sPass.settings.getItemIndex();

    } else if (id < -1) {
        return -2;
    }
    if (findItem(id, sPass)){
        return -2;
    }
    if (item.getId() == -2) {
        item.setId(id);

    }
    sPass.items.emplace(id, item);
    archiveItem(id, sPass);

    return id;

}

int addItem(Item& item, SPass& sPass) {
    return addItem(-1, item, sPass);
}

bool removeItem(int id, SPass& sPass) {
    if (!findItem(id, sPass)) return false;
    using nlohmann::json; //*

    try {
        json data;
        ifstream in = openInFile("items.json", sPass);
        if (in) { //*
            if (in.peek() == std::ifstream::traits_type::eof()) { //*
                data = json::array(); // empty file //*
            } else {
                in >> data;           // parse existing JSON //*
            } //*
        } //*
        in.close(); //*

        if (!data.is_array()) { //*
            data = json::array(); //*
        } //*


        // Get transaction from map
        Item &item = sPass.items.at(id); //*

        // Convert to JSON
        json c = item; //*

        // Append

        // Rewrite file
        ofstream out = openOutFile("items.json", sPass);

        bool removed = false;
        for (auto it = data.begin(); it != data.end(); ++it) {
            if (it->contains("id") && (*it)["id"] == id) {
                data.erase(it);
                removed = true;
                break;
            }
        }


        if (!removed) {
            cerr << "Could not remove JSON item at: " << id << endl;
        }

        out << data.dump(4); //*
        sPass.items.erase(id);

        return true;
    } catch (...) {
        cout << "removeItem JSON error" << endl;
    }
    return false;
}

// https://www.geeksforgeeks.org/cpp/file-system-library-in-cpp-17/
ifstream openInFile(string fileName, SPass& sPass){
    filesystem::path path = filesystem::path("data") / sPass.settings.getFolder() / fileName;

    ifstream in(path);

    if (!in.is_open()){
        cerr << "Could not open the file '" << fileName << "' in the folder 'data/" << sPass.settings.getFolder() << "'" << endl;
    }

    return in;
}

ofstream openOutFile(string fileName, SPass& sPass){
    filesystem::path path = filesystem::path("data") / sPass.settings.getFolder() / fileName;

    ofstream out(path);

    if (!out.is_open()){
        cerr << "Could not open the file '" << fileName << "' in the folder 'data/" << sPass.settings.getFolder() << "'" << endl;
    }

    return out;
}

