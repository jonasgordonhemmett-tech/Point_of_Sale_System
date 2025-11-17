#include <vector>
#include <string>
#include "item.h"
#include "ItemIn.h"
#include "Transaction.h"
#include "include/json.hpp"
#include <iostream>

using namespace std;

/*
  Stores transaction information.
 */
Transaction::Transaction(PaymentPortal* portal, int id, int customerId) : completed(false), portal(portal), customerId(customerId), id(id) {}
Transaction::Transaction(PaymentPortal* portal, int customerId) : completed(false), portal(portal), customerId(customerId), id(-2){}
Transaction::Transaction(PaymentPortal* portal) : completed(false), portal(portal), customerId(-2), id(-2){}

// Transaction::Transaction(PaymentPortal* portal, int customerId, int id, vector<ItemIn>& items, int totalCost) : completed(true), totalCost(totalCost), portal(portal), customerId(customerId), id(id) {}
// Transaction::Transaction(PaymentPortal* portal, int id, vector<ItemIn>& items, int totalCost) : completed(true), totalCost(totalCost), portal(portal), customerId(-2), id(id) {}

// If a transaction is canceled, items are restocked
Transaction::~Transaction() {
  if (!completed) {
    for (ItemIn& item : items) {
      item.item->changeHoldQuantity(item.purchaseQuantity);
    }
  }
}

int Transaction::getTotalCost() const {return totalCost;}

vector<ItemIn>& Transaction::getItems() {return items;}
const vector<ItemIn>& Transaction::getItems() const {return items;}

// Items are reserved when they are added to cart.
bool Transaction::addItem(Item& item, int quantity)
{
  if (completed) return false;
  if (quantity <= 0 || item.getQuantity() < quantity) return false;

  ItemIn itemAdd(&item);

  // Something to note that if the price is changed in between things being added, the lower price is kept.
  // This updates existing quantities if the item is already in cart
  for (ItemIn& i : items)
  {
    if (i == itemAdd)
    {
      i.purchaseQuantity += quantity;
      totalCost += quantity * i.purchasePrice;

      item.changeHoldQuantity(quantity);
      return true;
    }
  }

  itemAdd.purchasePrice = item.getPrice();
  itemAdd.purchaseQuantity = quantity;

  items.push_back(itemAdd);

  totalCost += quantity * item.getPrice();

  item.changeHoldQuantity(quantity);

  return true;
}

bool Transaction::addItem(Item& item) {return addItem(item, 1);}



ItemIn* Transaction::getItem(int itemId){
  for (ItemIn& i : items){
    if (i.item->getId() == itemId) {
      return &i;
    }
  }
  return nullptr;
}
ItemIn* Transaction::getItem(const Item& item){
  return getItem(item.getId());
}
bool Transaction::removeItem(Item& item, int quantity)
{
  if (completed) return false;
  if (quantity <= 0) return false;

  // Removes desiered quantity, if entire quantity is removed then the item is removed.
  // https://www.w3schools.com/cpp/ref_vector_erase.asp
  for (auto it = items.begin(); it != items.end();){
    if (it->item == &item){
      if (it->purchaseQuantity <= quantity){
        totalCost -= it->purchaseQuantity * it->purchasePrice;

        item.changeHoldQuantity(it->purchaseQuantity);

        items.erase(it);

        return true;
      } else {
        totalCost -= quantity * it->purchasePrice;
        it->purchaseQuantity -= quantity;

        item.changeHoldQuantity(quantity);

        return true;
      }
    }
    it++;
  }
  return false;
}

bool Transaction::removeItem(Item& item) {return removeItem(item, 1);}


bool Transaction::setItems(vector<ItemIn>& items) {
  // TODO, fix this
  // if (!completed) return false;
  this->items = items;
  return true;
}

int Transaction::getCustomerId() const {return customerId;}

 bool Transaction::isCompleted()  const{return completed;}

void Transaction::completeTransaction(){
  if (id == -2){
    cerr << "Transaction is missing an ID";
    return;
  }
  portal->deposit(totalCost);
  for (ItemIn& i : items){
    i.item->changeQuantity(-i.purchaseQuantity);
  }

  completed = true;
}

void Transaction::cancelTransaction() {
  if (!completed) {
    for (ItemIn& item : items) {
      item.item->changeHoldQuantity(item.purchaseQuantity);
      removeItem(*item.item, item.purchaseQuantity);
    }
    completed = true;
  }
}
int Transaction::getId() const {return id;}


std::ostream& operator<<(std::ostream& os, const Transaction& obj) {
  if (obj.completed) {
    os << "- Shopping Receipt -" << endl;
    os << "CustomerId: " << obj.customerId << endl;
    for (ItemIn item : obj.items){
      os << item.item->getName() << "(" << item.item->getId() << ") " << item.item->getPrice() << "x $" << item.purchasePrice << endl;
    }
    os << "Total: $" << obj.totalCost << endl;
    return os;
  }

  os << "- Shopping Cart -" << endl;
  os << "CustomerId: " << obj.customerId << endl;

  for (ItemIn item : obj.items){
    os << item.item->getName() << "(" << item.item->getId() << ") " << item.purchaseQuantity << "x $" << item.purchasePrice << endl;
  }
  os << "Total: $" << obj.totalCost << endl;
  return os;
  os << "- Shopping Cart -" << endl;

}

void to_json(nlohmann::json& j, const Transaction& t){
  j = nlohmann::json{
    // https://json.nlohmann.me/api/basic_json/array/
      {"id", t.getId()},
    {"items", nlohmann::json::array()},
{"totalCost", t.getTotalCost()},
{"customerId", t.getCustomerId()},
  };

  for (const ItemIn item : t.getItems())
  {

    nlohmann::json itemA = nlohmann::json::array({
      item.item->getId(),
      item.purchaseQuantity,
      item.purchasePrice
    });

    j["items"].push_back(itemA);


  }
}

bool Transaction::setId(int id) {
  if (this->id != -2) return false;
  this->id = id;
  return true;
}
