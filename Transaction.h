
#include <vector>
#include <string>
#include "Item.h"
#include "ItemIn.h"
#include <iostream>
#include "PaymentPortal.h"
#include "include/json.hpp"



#ifndef TRANSACTION_H
#define TRANSACTION_H


class Transaction {
private:
    // An unordered map might be better, but this works well enough.
    vector<ItemIn> items;
    int totalCost = 0;
    bool completed;
    int id = -2;
    int customerId = -2;
    PaymentPortal* portal;


public:
  Transaction(PaymentPortal* portal, int id, int customerId);
  Transaction(PaymentPortal* portal, int customerId);
  Transaction(PaymentPortal* portal);

  // Transaction(PaymentPortal* portal, int customerId, int id, vector<ItemIn>& items, int totalCost);
  // Transaction(PaymentPortal* portal, int id, vector<ItemIn>& items, int totalCost);

  // If an uncompleted transaction is deleted, items are restocked
  ~Transaction();

  //Getters and setters
  int getTotalCost() const;

  vector<ItemIn>& getItems();
  const vector<ItemIn>& getItems() const;

  bool addItem(Item& item, int quantity);
  bool addItem(Item& item);
  ItemIn* getItem(int itemId);
  ItemIn* getItem(const Item& item);

  bool removeItem(Item& item, int quantity);
  bool removeItem(Item& item);
  bool setItems(vector<ItemIn>& items);

  int getCustomerId() const;
  bool isCompleted() const;
  void completeTransaction();
  void cancelTransaction();

  int getId() const;

  // string archive() const;

  string toJson() const;
  friend std::ostream& operator<<(std::ostream& os, const Transaction& obj);

  bool setId(int id);


};
void to_json(nlohmann::json& j, const Transaction& t);
#endif //TRANSACTION_H
